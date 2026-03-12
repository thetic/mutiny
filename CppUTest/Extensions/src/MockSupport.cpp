#include "CppUTest/Extensions/MockSupport.h"

#include "CppUTest/Extensions/MockActualCallTrace.hpp"
#include "CppUTest/Extensions/MockCheckedActualCall.hpp"
#include "CppUTest/Extensions/MockExpectedCall.hpp"
#include "CppUTest/Extensions/MockFailure.hpp"
#include "CppUTest/Extensions/MockIgnoredActualCall.hpp"
#include "CppUTest/Extensions/MockIgnoredExpectedCall.hpp"
#include "CppUTest/Extensions/MockSupport.hpp"

#include "CppUTest/TestHarness.hpp"

#define MOCK_SUPPORT_SCOPE_PREFIX "!!!$$$MockingSupportScope$$$!!!"

namespace cpputest {
namespace extensions {

static MockSupport global_mock;

MockSupport&
mock(const String& mockName, MockFailureReporter* failureReporterForThisCall)
{
  MockSupport& mock_support = (mockName != "")
                                  ? *global_mock.getMockSupportScope(mockName)
                                  : global_mock;
  mock_support.setActiveReporter(failureReporterForThisCall);
  mock_support.setDefaultComparatorsAndCopiersRepository();
  return mock_support;
}

MockSupport::MockSupport(const String& mockName)
  : actualCallOrder_(0)
  , expectedCallOrder_(0)
  , strictOrdering_(false)
  , activeReporter_(nullptr)
  , standardReporter_(&defaultReporter_)
  , ignoreOtherCalls_(false)
  , enabled_(true)
  , lastActualFunctionCall_(nullptr)
  , mockName_(mockName)
  , tracing_(false)
{
}

MockSupport::~MockSupport() {}

void
MockSupport::crashOnFailure(bool shouldCrash)
{
  activeReporter_->crashOnFailure(shouldCrash);
}

void
MockSupport::setMockFailureStandardReporter(MockFailureReporter* reporter)
{
  standardReporter_ = (reporter != nullptr) ? reporter : &defaultReporter_;

  if (lastActualFunctionCall_)
    lastActualFunctionCall_->setMockFailureReporter(standardReporter_);

  for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
    if (getMockSupport(p))
      getMockSupport(p)->setMockFailureStandardReporter(standardReporter_);
}

void
MockSupport::setActiveReporter(MockFailureReporter* reporter)
{
  activeReporter_ = (reporter) ? reporter : standardReporter_;
}

void
MockSupport::setDefaultComparatorsAndCopiersRepository()
{
  MockNamedValue::setDefaultComparatorsAndCopiersRepository(
      &comparatorsAndCopiersRepository_);
}

void
MockSupport::installComparator(const String& typeName,
    MockNamedValueComparator& comparator)
{
  comparatorsAndCopiersRepository_.installComparator(typeName, comparator);

  for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
    if (getMockSupport(p))
      getMockSupport(p)->installComparator(typeName, comparator);
}

void
MockSupport::installCopier(const String& typeName, MockNamedValueCopier& copier)
{
  comparatorsAndCopiersRepository_.installCopier(typeName, copier);

  for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
    if (getMockSupport(p))
      getMockSupport(p)->installCopier(typeName, copier);
}

void
MockSupport::installComparatorsAndCopiers(
    const MockNamedValueComparatorsAndCopiersRepository& repository)
{
  comparatorsAndCopiersRepository_.installComparatorsAndCopiers(repository);

  for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
    if (getMockSupport(p))
      getMockSupport(p)->installComparatorsAndCopiers(repository);
}

void
MockSupport::removeAllComparatorsAndCopiers()
{
  comparatorsAndCopiersRepository_.clear();
  for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
    if (getMockSupport(p))
      getMockSupport(p)->removeAllComparatorsAndCopiers();
}

void
MockSupport::clear()
{
  delete lastActualFunctionCall_;
  lastActualFunctionCall_ = nullptr;

  tracing_ = false;
  MockActualCallTrace::clearInstance();

  expectations_.deleteAllExpectationsAndClearList();
  ignoreOtherCalls_ = false;
  enabled_ = true;
  actualCallOrder_ = 0;
  expectedCallOrder_ = 0;
  strictOrdering_ = false;

  for (MockNamedValueListNode* p = data_.begin(); p; p = p->next()) {
    MockSupport* support = getMockSupport(p);
    if (support) {
      support->clear();
      delete support;
    }
  }
  data_.clear();
}

void
MockSupport::strictOrder()
{
  strictOrdering_ = true;
}

String
MockSupport::appendScopeToName(const String& functionName)
{
  if (mockName_.empty())
    return functionName;
  return mockName_ + "::" + functionName;
}

MockExpectedCall&
MockSupport::expectOneCall(const String& functionName)
{
  return expectNCalls(1, functionName);
}

void
MockSupport::expectNoCall(const String& functionName)
{
  expectNCalls(0, functionName);
}

MockExpectedCall&
MockSupport::expectNCalls(unsigned int amount, const String& functionName)
{
  if (!enabled_)
    return MockIgnoredExpectedCall::instance();

  countCheck();

  MockCheckedExpectedCall* call = new MockCheckedExpectedCall(amount);
  call->withName(appendScopeToName(functionName));
  if (strictOrdering_) {
    call->withCallOrder(expectedCallOrder_ + 1, expectedCallOrder_ + amount);
    expectedCallOrder_ += amount;
  }
  expectations_.addExpectedCall(call);
  return *call;
}

MockCheckedActualCall*
MockSupport::createActualCall()
{
  lastActualFunctionCall_ = new MockCheckedActualCall(
      ++actualCallOrder_, activeReporter_, expectations_);
  return lastActualFunctionCall_;
}

bool
MockSupport::callIsIgnored(const String& functionName)
{
  return ignoreOtherCalls_ &&
         !expectations_.hasExpectationWithName(functionName);
}

MockActualCall&
MockSupport::actualCall(const char* functionName)
{
  String scopeFunctionName = appendScopeToName(functionName);

  if (lastActualFunctionCall_) {
    lastActualFunctionCall_->checkExpectations();
    delete lastActualFunctionCall_;
    lastActualFunctionCall_ = nullptr;
  }

  if (!enabled_)
    return MockIgnoredActualCall::instance();
  if (tracing_)
    return MockActualCallTrace::instance().withName(scopeFunctionName);

  if (callIsIgnored(scopeFunctionName)) {
    return MockIgnoredActualCall::instance();
  }

  MockCheckedActualCall* call = createActualCall();
  call->setNameAndCheck(static_cast<String&&>(scopeFunctionName));
  return *call;
}

MockActualCall&
MockSupport::actualCall(const String& functionName)
{
  String scopeFunctionName = appendScopeToName(functionName);

  if (lastActualFunctionCall_) {
    lastActualFunctionCall_->checkExpectations();
    delete lastActualFunctionCall_;
    lastActualFunctionCall_ = nullptr;
  }

  if (!enabled_)
    return MockIgnoredActualCall::instance();
  if (tracing_)
    return MockActualCallTrace::instance().withName(scopeFunctionName);

  if (callIsIgnored(scopeFunctionName)) {
    return MockIgnoredActualCall::instance();
  }

  MockCheckedActualCall* call = createActualCall();
  call->setNameAndCheck(static_cast<String&&>(scopeFunctionName));
  return *call;
}

void
MockSupport::ignoreOtherCalls()
{
  ignoreOtherCalls_ = true;

  for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
    if (getMockSupport(p))
      getMockSupport(p)->ignoreOtherCalls();
}

void
MockSupport::disable()
{
  enabled_ = false;

  for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
    if (getMockSupport(p))
      getMockSupport(p)->disable();
}

void
MockSupport::enable()
{
  enabled_ = true;

  for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
    if (getMockSupport(p))
      getMockSupport(p)->enable();
}

void
MockSupport::tracing(bool enabled)
{
  tracing_ = enabled;

  for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
    if (getMockSupport(p))
      getMockSupport(p)->tracing(enabled);
}

const char*
MockSupport::getTraceOutput()
{
  return MockActualCallTrace::instance().getTraceOutput();
}

bool
MockSupport::expectedCallsLeft()
{
  checkExpectationsOfLastActualCall();
  int callsLeft = expectations_.hasUnfulfilledExpectations();

  for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
    if (getMockSupport(p))
      callsLeft += getMockSupport(p)->expectedCallsLeft();

  return callsLeft != 0;
}

bool
MockSupport::wasLastActualCallFulfilled()
{
  if (lastActualFunctionCall_ && !lastActualFunctionCall_->isFulfilled())
    return false;

  for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
    if (getMockSupport(p) && !getMockSupport(p)->wasLastActualCallFulfilled())
      return false;

  return true;
}

void
MockSupport::failTestWithExpectedCallsNotFulfilled()
{
  MockExpectedCallsList expectationsList;
  expectationsList.addExpectations(expectations_);

  for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
    if (getMockSupport(p))
      expectationsList.addExpectations(getMockSupport(p)->expectations_);

  MockExpectedCallsDidntHappenFailure failure(
      activeReporter_->getTestToFail(), expectationsList);
  failTest(failure);
}

void
MockSupport::failTestWithOutOfOrderCalls()
{
  MockExpectedCallsList expectationsList;
  expectationsList.addExpectations(expectations_);

  for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
    if (getMockSupport(p))
      expectationsList.addExpectations(getMockSupport(p)->expectations_);

  MockCallOrderFailure failure(
      activeReporter_->getTestToFail(), expectationsList);
  failTest(failure);
}

void
MockSupport::failTest(MockFailure& failure)
{
  clear();
  activeReporter_->failTest(static_cast<MockFailure&&>(failure));
}

void
MockSupport::countCheck()
{
  cpputest::TestShell::getCurrent()->countCheck();
}

void
MockSupport::checkExpectationsOfLastActualCall()
{
  if (lastActualFunctionCall_)
    lastActualFunctionCall_->checkExpectations();

  for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
    if (getMockSupport(p) && getMockSupport(p)->lastActualFunctionCall_)
      getMockSupport(p)->lastActualFunctionCall_->checkExpectations();
}

bool
MockSupport::hasCallsOutOfOrder()
{
  if (expectations_.hasCallsOutOfOrder()) {
    return true;
  }
  for (MockNamedValueListNode* p = data_.begin(); p; p = p->next())
    if (getMockSupport(p) && getMockSupport(p)->hasCallsOutOfOrder()) {
      return true;
    }
  return false;
}

void
MockSupport::checkExpectations()
{
  checkExpectationsOfLastActualCall();

  if (wasLastActualCallFulfilled() && expectedCallsLeft())
    failTestWithExpectedCallsNotFulfilled();

  if (hasCallsOutOfOrder())
    failTestWithOutOfOrderCalls();
}

bool
MockSupport::hasData(const String& name)
{
  return data_.getValueByName(name) != nullptr;
}

MockNamedValue*
MockSupport::retrieveDataFromStore(const String& name)
{
  MockNamedValue* newData = data_.getValueByName(name);
  if (newData == nullptr) {
    newData = new MockNamedValue(name);
    data_.add(newData);
  }
  return newData;
}

void
MockSupport::setData(const String& name, bool value)
{
  MockNamedValue* newData = retrieveDataFromStore(name);
  newData->setValue(value);
}

void
MockSupport::setData(const String& name, unsigned int value)
{
  MockNamedValue* newData = retrieveDataFromStore(name);
  newData->setValue(value);
}

void
MockSupport::setData(const String& name, int value)
{
  MockNamedValue* newData = retrieveDataFromStore(name);
  newData->setValue(value);
}

void
MockSupport::setData(const String& name, long int value)
{
  MockNamedValue* newData = retrieveDataFromStore(name);
  newData->setValue(value);
}

void
MockSupport::setData(const String& name, unsigned long int value)
{
  MockNamedValue* newData = retrieveDataFromStore(name);
  newData->setValue(value);
}

void
MockSupport::setData(const String& name, const char* value)
{
  MockNamedValue* newData = retrieveDataFromStore(name);
  newData->setValue(value);
}

void
MockSupport::setData(const String& name, double value)
{
  MockNamedValue* newData = retrieveDataFromStore(name);
  newData->setValue(value);
}

void
MockSupport::setData(const String& name, void* value)
{
  MockNamedValue* newData = retrieveDataFromStore(name);
  newData->setValue(value);
}

void
MockSupport::setData(const String& name, const void* value)
{
  MockNamedValue* newData = retrieveDataFromStore(name);
  newData->setValue(value);
}

void
MockSupport::setData(const String& name, void (*value)())
{
  MockNamedValue* newData = retrieveDataFromStore(name);
  newData->setValue(value);
}

void
MockSupport::setDataObject(const String& name, const String& type, void* value)
{
  MockNamedValue* newData = retrieveDataFromStore(name);
  newData->setObjectPointer(type, value);
}

void
MockSupport::setDataConstObject(const String& name,
    const String& type,
    const void* value)
{
  MockNamedValue* newData = retrieveDataFromStore(name);
  newData->setConstObjectPointer(type, value);
}

MockNamedValue
MockSupport::getData(const String& name)
{
  MockNamedValue* value = data_.getValueByName(name);
  if (value == nullptr)
    return MockNamedValue("");
  return *value;
}

MockSupport*
MockSupport::clone(const String& mockName)
{
  MockSupport* newMock = new MockSupport(mockName);
  newMock->setMockFailureStandardReporter(standardReporter_);
  if (ignoreOtherCalls_)
    newMock->ignoreOtherCalls();

  if (!enabled_)
    newMock->disable();

  if (strictOrdering_)
    newMock->strictOrder();

  newMock->tracing(tracing_);
  newMock->installComparatorsAndCopiers(comparatorsAndCopiersRepository_);
  return newMock;
}

MockSupport*
MockSupport::getMockSupportScope(const String& name)
{
  String mockingSupportName = MOCK_SUPPORT_SCOPE_PREFIX;
  mockingSupportName += name;

  if (hasData(mockingSupportName)) {
    STRCMP_EQUAL("MockSupport", getData(mockingSupportName).getType().c_str());
    return static_cast<MockSupport*>(
        getData(mockingSupportName).getObjectPointer());
  }

  MockSupport* newMock = clone(name);

  setDataObject(mockingSupportName, "MockSupport", newMock);
  return newMock;
}

MockSupport*
MockSupport::getMockSupport(MockNamedValueListNode* node)
{
  if (node->getType() == "MockSupport" &&
      node->getName().contains(MOCK_SUPPORT_SCOPE_PREFIX))
    return static_cast<MockSupport*>(node->item()->getObjectPointer());
  return nullptr;
}

MockNamedValue
MockSupport::returnValue()
{
  if (lastActualFunctionCall_)
    return lastActualFunctionCall_->returnValue();
  return MockNamedValue("");
}

bool
MockSupport::boolReturnValue()
{
  return returnValue().getBoolValue();
}

unsigned int
MockSupport::unsignedIntReturnValue()
{
  return returnValue().getUnsignedIntValue();
}

int
MockSupport::intReturnValue()
{
  return returnValue().getIntValue();
}

const char*
MockSupport::returnStringValueOrDefault(const char* defaultValue)
{
  if (hasReturnValue()) {
    return stringReturnValue();
  }
  return defaultValue;
}

double
MockSupport::returnDoubleValueOrDefault(double defaultValue)
{
  if (hasReturnValue()) {
    return doubleReturnValue();
  }
  return defaultValue;
}

long int
MockSupport::returnLongIntValueOrDefault(long int defaultValue)
{
  if (hasReturnValue()) {
    return longIntReturnValue();
  }
  return defaultValue;
}

bool
MockSupport::returnBoolValueOrDefault(bool defaultValue)
{
  if (hasReturnValue()) {
    return boolReturnValue();
  }
  return defaultValue;
}

int
MockSupport::returnIntValueOrDefault(int defaultValue)
{
  if (hasReturnValue()) {
    return intReturnValue();
  }
  return defaultValue;
}

unsigned int
MockSupport::returnUnsignedIntValueOrDefault(unsigned int defaultValue)
{
  if (hasReturnValue()) {
    return unsignedIntReturnValue();
  }
  return defaultValue;
}

unsigned long int
MockSupport::returnUnsignedLongIntValueOrDefault(unsigned long int defaultValue)
{
  if (hasReturnValue()) {
    return unsignedLongIntReturnValue();
  }
  return defaultValue;
}

long int
MockSupport::longIntReturnValue()
{
  return returnValue().getLongIntValue();
}

unsigned long int
MockSupport::unsignedLongIntReturnValue()
{
  return returnValue().getUnsignedLongIntValue();
}

long long
MockSupport::longLongIntReturnValue()
{
  return returnValue().getLongLongIntValue();
}

unsigned long long
MockSupport::unsignedLongLongIntReturnValue()
{
  return returnValue().getUnsignedLongLongIntValue();
}

long long
MockSupport::returnLongLongIntValueOrDefault(long long defaultValue)
{
  if (hasReturnValue()) {
    return longLongIntReturnValue();
  }
  return defaultValue;
}

unsigned long long
MockSupport::returnUnsignedLongLongIntValueOrDefault(
    unsigned long long defaultValue)
{
  if (hasReturnValue()) {
    return unsignedLongLongIntReturnValue();
  }
  return defaultValue;
}

const char*
MockSupport::stringReturnValue()
{
  return returnValue().getStringValue();
}

double
MockSupport::doubleReturnValue()
{
  return returnValue().getDoubleValue();
}

void*
MockSupport::returnPointerValueOrDefault(void* defaultValue)
{
  if (hasReturnValue()) {
    return pointerReturnValue();
  }
  return defaultValue;
}

const void*
MockSupport::returnConstPointerValueOrDefault(const void* defaultValue)
{
  if (hasReturnValue()) {
    return constPointerReturnValue();
  }
  return defaultValue;
}

void (
    *MockSupport::returnFunctionPointerValueOrDefault(void (*defaultValue)()))()
{
  if (hasReturnValue()) {
    return functionPointerReturnValue();
  }
  return defaultValue;
}

void*
MockSupport::pointerReturnValue()
{
  return returnValue().getPointerValue();
}

const void*
MockSupport::constPointerReturnValue()
{
  return returnValue().getConstPointerValue();
}

void (*MockSupport::functionPointerReturnValue())()
{
  return returnValue().getFunctionPointerValue();
}

bool
MockSupport::hasReturnValue()
{
  if (lastActualFunctionCall_)
    return lastActualFunctionCall_->hasReturnValue();
  return false;
}

namespace {

typedef void (*cpputest_cpp_function_pointer)(); /* Cl2000 requires cast to C++
                                                    function */

class MockFailureReporterTestTerminatorForInCOnlyCode : public TestTerminator
{
public:
  MockFailureReporterTestTerminatorForInCOnlyCode(bool crashOnFailure)
    : crashOnFailure_(crashOnFailure)
  {
  }

  virtual void exitCurrentTest() const override
  {
    if (crashOnFailure_)
      cpputest::TestShell::crash();

    TestShell::getCurrentTestTerminatorWithoutExceptions().exitCurrentTest();
  }
  virtual ~MockFailureReporterTestTerminatorForInCOnlyCode() override {}

private:
  bool crashOnFailure_;
};

class MockFailureReporterForInCOnlyCode : public MockFailureReporter
{
public:
  void failTest(MockFailure failure) override
  {
    if (!getTestToFail()->hasFailed()) {
      getTestToFail()->addFailure(failure);
      {
        MockFailure freed(static_cast<MockFailure&&>(failure));
      }
      exitTest();
    }
  }

  void exitTest() override
  {
    MockFailureReporterTestTerminatorForInCOnlyCode(crashOnFailure_)
        .exitCurrentTest();
  }
};

static MockSupport* currentMockSupport = nullptr;
static MockExpectedCall* expectedCall = nullptr;
static MockActualCall* actualCall = nullptr;
static MockFailureReporterForInCOnlyCode failureReporterForC;

class MockCFunctionComparatorNode : public MockNamedValueComparator
{
public:
  MockCFunctionComparatorNode(MockCFunctionComparatorNode* next,
      MockTypeEqualFunction_c equal,
      MockTypeValueToStringFunction_c toString)
    : next_(next)
    , equal_(equal)
    , toString_(toString)
  {
  }
  virtual ~MockCFunctionComparatorNode() override {}

  virtual bool isEqual(const void* object1, const void* object2) override
  {
    return equal_(object1, object2) != 0;
  }
  virtual String valueToString(const void* object) override
  {
    return String(toString_(object));
  }

  MockCFunctionComparatorNode* next_;
  MockTypeEqualFunction_c equal_;
  MockTypeValueToStringFunction_c toString_;
};

static MockCFunctionComparatorNode* comparatorList_ = nullptr;

class MockCFunctionCopierNode : public MockNamedValueCopier
{
public:
  MockCFunctionCopierNode(MockCFunctionCopierNode* next,
      MockTypeCopyFunction_c copier)
    : next_(next)
    , copier_(copier)
  {
  }
  virtual ~MockCFunctionCopierNode() override {}

  virtual void copy(void* dst, const void* src) override { copier_(dst, src); }

  MockCFunctionCopierNode* next_;
  MockTypeCopyFunction_c copier_;
};

static MockCFunctionCopierNode* copierList_ = nullptr;

void
strictOrder_c();
MockExpectedCall_c*
expectOneCall_c(const char* name);
void
expectNoCall_c(const char* name);
MockExpectedCall_c*
expectNCalls_c(const unsigned int number, const char* name);
MockActualCall_c*
actualCall_c(const char* name);
void
disable_c();
void
enable_c();
void
ignoreOtherCalls_c();
void
setBoolData_c(const char* name, int value);
void
setIntData_c(const char* name, int value);
void
setUnsignedIntData_c(const char* name, unsigned int value);
void
setLongIntData_c(const char* name, long int value);
void
setUnsignedLongIntData_c(const char* name, unsigned long int value);
void
setDoubleData_c(const char* name, double value);
void
setStringData_c(const char* name, const char* value);
void
setPointerData_c(const char* name, void* value);
void
setConstPointerData_c(const char* name, const void* value);
void
setFunctionPointerData_c(const char* name, void (*value)());
void
setDataObject_c(const char* name, const char* type, void* value);
void
setDataConstObject_c(const char* name, const char* type, const void* value);
MockValue_c
getData_c(const char* name);
int
hasReturnValue_c();

void
checkExpectations_c();
int
expectedCallsLeft_c();
void
clear_c();
void
crashOnFailure_c(unsigned shouldCrash);

MockExpectedCall_c*
withBoolParameters_c(const char* name, int value);
MockExpectedCall_c*
withIntParameters_c(const char* name, int value);
MockExpectedCall_c*
withUnsignedIntParameters_c(const char* name, unsigned int value);
MockExpectedCall_c*
withLongIntParameters_c(const char* name, long int value);
MockExpectedCall_c*
withUnsignedLongIntParameters_c(const char* name, unsigned long int value);
MockExpectedCall_c*
withLongLongIntParameters_c(const char* name, long long value);
MockExpectedCall_c*
withUnsignedLongLongIntParameters_c(const char* name, unsigned long long value);
MockExpectedCall_c*
withDoubleParameters_c(const char* name, double value);
MockExpectedCall_c*
withDoubleParametersAndTolerance_c(const char* name,
    double value,
    double tolerance);
MockExpectedCall_c*
withStringParameters_c(const char* name, const char* value);
MockExpectedCall_c*
withPointerParameters_c(const char* name, void* value);
MockExpectedCall_c*
withConstPointerParameters_c(const char* name, const void* value);
MockExpectedCall_c*
withFunctionPointerParameters_c(const char* name, void (*value)());
MockExpectedCall_c*
withMemoryBufferParameters_c(const char* name,
    const unsigned char* value,
    size_t size);
MockExpectedCall_c*
withParameterOfType_c(const char* type, const char* name, const void* value);
MockExpectedCall_c*
withOutputParameterReturning_c(const char* name,
    const void* value,
    size_t size);
MockExpectedCall_c*
withOutputParameterOfTypeReturning_c(const char* type,
    const char* name,
    const void* value);
MockExpectedCall_c*
withUnmodifiedOutputParameter_c(const char* name);
MockExpectedCall_c*
ignoreOtherParameters_c();
MockExpectedCall_c*
andReturnBoolValue_c(int value);
MockExpectedCall_c*
andReturnIntValue_c(int value);
MockExpectedCall_c*
andReturnUnsignedIntValue_c(unsigned int value);
MockExpectedCall_c*
andReturnLongIntValue_c(long int value);
MockExpectedCall_c*
andReturnUnsignedLongIntValue_c(unsigned long int value);
MockExpectedCall_c*
andReturnLongLongIntValue_c(long long value);
MockExpectedCall_c*
andReturnUnsignedLongLongIntValue_c(unsigned long long value);
MockExpectedCall_c*
andReturnDoubleValue_c(double value);
MockExpectedCall_c*
andReturnStringValue_c(const char* value);
MockExpectedCall_c*
andReturnPointerValue_c(void* value);
MockExpectedCall_c*
andReturnConstPointerValue_c(const void* value);
MockExpectedCall_c*
andReturnFunctionPointerValue_c(void (*value)());

MockActualCall_c*
withActualBoolParameters_c(const char* name, int value);
MockActualCall_c*
withActualIntParameters_c(const char* name, int value);
MockActualCall_c*
withActualUnsignedIntParameters_c(const char* name, unsigned int value);
MockActualCall_c*
withActualLongIntParameters_c(const char* name, long int value);
MockActualCall_c*
withActualUnsignedLongIntParameters_c(const char* name,
    unsigned long int value);
MockActualCall_c*
withActualLongLongIntParameters_c(const char* name, long long value);
MockActualCall_c*
withActualUnsignedLongLongIntParameters_c(const char* name,
    unsigned long long value);
MockActualCall_c*
withActualDoubleParameters_c(const char* name, double value);
MockActualCall_c*
withActualStringParameters_c(const char* name, const char* value);
MockActualCall_c*
withActualPointerParameters_c(const char* name, void* value);
MockActualCall_c*
withActualConstPointerParameters_c(const char* name, const void* value);
MockActualCall_c*
withActualFunctionPointerParameters_c(const char* name, void (*value)());
MockActualCall_c*
withActualMemoryBufferParameters_c(const char* name,
    const unsigned char* value,
    size_t size);
MockActualCall_c*
withActualParameterOfType_c(const char* type,
    const char* name,
    const void* value);
MockActualCall_c*
withActualOutputParameter_c(const char* name, void* value);
MockActualCall_c*
withActualOutputParameterOfType_c(const char* type,
    const char* name,
    void* value);
MockValue_c
returnValue_c();
int
boolReturnValue_c();
int
returnBoolValueOrDefault_c(int defaultValue);
int
intReturnValue_c();
int
returnIntValueOrDefault_c(int defaultValue);
unsigned int
unsignedIntReturnValue_c();
unsigned int
returnUnsignedIntValueOrDefault_c(unsigned int defaultValue);
long int
longIntReturnValue_c();
long int
returnLongIntValueOrDefault_c(long int defaultValue);
unsigned long int
unsignedLongIntReturnValue_c();
unsigned long int
returnUnsignedLongIntValueOrDefault_c(unsigned long int defaultValue);
long long
longLongIntReturnValue_c();
long long
returnLongLongIntValueOrDefault_c(long long defaultValue);
unsigned long long
unsignedLongLongIntReturnValue_c();
unsigned long long
returnUnsignedLongLongIntValueOrDefault_c(unsigned long long defaultValue);
const char*
stringReturnValue_c();
const char*
returnStringValueOrDefault_c(const char* defaultValue);
double
doubleReturnValue_c();
double
returnDoubleValueOrDefault_c(double defaultValue);
void*
pointerReturnValue_c();
void*
returnPointerValueOrDefault_c(void* defaultValue);
const void*
constPointerReturnValue_c();
const void*
returnConstPointerValueOrDefault_c(const void* defaultValue);
void (*functionPointerReturnValue_c())();
void (*returnFunctionPointerValueOrDefault_c(void (*defaultValue)()))();

static void
installComparator_c(const char* typeName,
    MockTypeEqualFunction_c isEqual,
    MockTypeValueToStringFunction_c valueToString)
{
  comparatorList_ =
      new MockCFunctionComparatorNode(comparatorList_, isEqual, valueToString);
  currentMockSupport->installComparator(typeName, *comparatorList_);
}

static void
installCopier_c(const char* typeName, MockTypeCopyFunction_c copier)
{
  copierList_ = new MockCFunctionCopierNode(copierList_, copier);
  currentMockSupport->installCopier(typeName, *copierList_);
}

static void
removeAllComparatorsAndCopiers_c()
{
  while (comparatorList_) {
    MockCFunctionComparatorNode* next = comparatorList_->next_;
    delete comparatorList_;
    comparatorList_ = next;
  }
  while (copierList_) {
    MockCFunctionCopierNode* next = copierList_->next_;
    delete copierList_;
    copierList_ = next;
  }
  currentMockSupport->removeAllComparatorsAndCopiers();
}

static MockExpectedCall_c gExpectedCall = {
  withBoolParameters_c,
  withIntParameters_c,
  withUnsignedIntParameters_c,
  withLongIntParameters_c,
  withUnsignedLongIntParameters_c,
  withLongLongIntParameters_c,
  withUnsignedLongLongIntParameters_c,
  withDoubleParameters_c,
  withDoubleParametersAndTolerance_c,
  withStringParameters_c,
  withPointerParameters_c,
  withConstPointerParameters_c,
  withFunctionPointerParameters_c,
  withMemoryBufferParameters_c,
  withParameterOfType_c,
  withOutputParameterReturning_c,
  withOutputParameterOfTypeReturning_c,
  withUnmodifiedOutputParameter_c,
  ignoreOtherParameters_c,
  andReturnBoolValue_c,
  andReturnUnsignedIntValue_c,
  andReturnIntValue_c,
  andReturnLongIntValue_c,
  andReturnUnsignedLongIntValue_c,
  andReturnLongLongIntValue_c,
  andReturnUnsignedLongLongIntValue_c,
  andReturnDoubleValue_c,
  andReturnStringValue_c,
  andReturnPointerValue_c,
  andReturnConstPointerValue_c,
  andReturnFunctionPointerValue_c,
};

static MockActualCall_c gActualCall = { withActualBoolParameters_c,
  withActualIntParameters_c,
  withActualUnsignedIntParameters_c,
  withActualLongIntParameters_c,
  withActualUnsignedLongIntParameters_c,
  withActualLongLongIntParameters_c,
  withActualUnsignedLongLongIntParameters_c,
  withActualDoubleParameters_c,
  withActualStringParameters_c,
  withActualPointerParameters_c,
  withActualConstPointerParameters_c,
  withActualFunctionPointerParameters_c,
  withActualMemoryBufferParameters_c,
  withActualParameterOfType_c,
  withActualOutputParameter_c,
  withActualOutputParameterOfType_c,
  hasReturnValue_c,
  returnValue_c,
  boolReturnValue_c,
  returnBoolValueOrDefault_c,
  intReturnValue_c,
  returnIntValueOrDefault_c,
  unsignedIntReturnValue_c,
  returnUnsignedIntValueOrDefault_c,
  longIntReturnValue_c,
  returnLongIntValueOrDefault_c,
  unsignedLongIntReturnValue_c,
  returnUnsignedLongIntValueOrDefault_c,
  longLongIntReturnValue_c,
  returnLongLongIntValueOrDefault_c,
  unsignedLongLongIntReturnValue_c,
  returnUnsignedLongLongIntValueOrDefault_c,
  stringReturnValue_c,
  returnStringValueOrDefault_c,
  doubleReturnValue_c,
  returnDoubleValueOrDefault_c,
  pointerReturnValue_c,
  returnPointerValueOrDefault_c,
  constPointerReturnValue_c,
  returnConstPointerValueOrDefault_c,
  functionPointerReturnValue_c,
  returnFunctionPointerValueOrDefault_c };

MockExpectedCall_c*
withBoolParameters_c(const char* name, int value)
{
  expectedCall = &expectedCall->withParameter(name, (value != 0));
  return &gExpectedCall;
}

MockExpectedCall_c*
withIntParameters_c(const char* name, int value)
{
  expectedCall = &expectedCall->withParameter(name, value);
  return &gExpectedCall;
}

MockExpectedCall_c*
withUnsignedIntParameters_c(const char* name, unsigned int value)
{
  expectedCall = &expectedCall->withParameter(name, value);
  return &gExpectedCall;
}

MockExpectedCall_c*
withLongIntParameters_c(const char* name, long int value)
{
  expectedCall = &expectedCall->withParameter(name, value);
  return &gExpectedCall;
}

MockExpectedCall_c*
withUnsignedLongIntParameters_c(const char* name, unsigned long int value)
{
  expectedCall = &expectedCall->withParameter(name, value);
  return &gExpectedCall;
}

MockExpectedCall_c*
withLongLongIntParameters_c(const char* name, long long value)
{
  expectedCall = &expectedCall->withParameter(name, value);
  return &gExpectedCall;
}

MockExpectedCall_c*
withUnsignedLongLongIntParameters_c(const char* name, unsigned long long value)
{
  expectedCall = &expectedCall->withParameter(name, value);
  return &gExpectedCall;
}

MockExpectedCall_c*
withDoubleParameters_c(const char* name, double value)
{
  expectedCall = &expectedCall->withParameter(name, value);
  return &gExpectedCall;
}

MockExpectedCall_c*
withDoubleParametersAndTolerance_c(const char* name,
    double value,
    double tolerance)
{
  expectedCall = &expectedCall->withParameter(name, value, tolerance);
  return &gExpectedCall;
}

MockExpectedCall_c*
withStringParameters_c(const char* name, const char* value)
{
  expectedCall = &expectedCall->withParameter(name, value);
  return &gExpectedCall;
}

MockExpectedCall_c*
withPointerParameters_c(const char* name, void* value)
{
  expectedCall = &expectedCall->withParameter(name, value);
  return &gExpectedCall;
}

MockExpectedCall_c*
withConstPointerParameters_c(const char* name, const void* value)
{
  expectedCall = &expectedCall->withParameter(name, value);
  return &gExpectedCall;
}

MockExpectedCall_c*
withFunctionPointerParameters_c(const char* name, void (*value)())
{
  expectedCall = &expectedCall->withParameter(name, value);
  return &gExpectedCall;
}

MockExpectedCall_c*
withMemoryBufferParameters_c(const char* name,
    const unsigned char* value,
    size_t size)
{
  expectedCall = &expectedCall->withParameter(name, value, size);
  return &gExpectedCall;
}

MockExpectedCall_c*
withParameterOfType_c(const char* type, const char* name, const void* value)
{
  expectedCall = &expectedCall->withParameterOfType(type, name, value);
  return &gExpectedCall;
}

MockExpectedCall_c*
withOutputParameterReturning_c(const char* name, const void* value, size_t size)
{
  expectedCall = &expectedCall->withOutputParameterReturning(name, value, size);
  return &gExpectedCall;
}

MockExpectedCall_c*
withOutputParameterOfTypeReturning_c(const char* type,
    const char* name,
    const void* value)
{
  expectedCall =
      &expectedCall->withOutputParameterOfTypeReturning(type, name, value);
  return &gExpectedCall;
}

MockExpectedCall_c*
withUnmodifiedOutputParameter_c(const char* name)
{
  expectedCall = &expectedCall->withUnmodifiedOutputParameter(name);
  return &gExpectedCall;
}

MockExpectedCall_c*
ignoreOtherParameters_c()
{
  expectedCall = &expectedCall->ignoreOtherParameters();
  return &gExpectedCall;
}

MockExpectedCall_c*
andReturnBoolValue_c(int value)
{
  expectedCall = &expectedCall->andReturnValue(value != 0);
  return &gExpectedCall;
}

MockExpectedCall_c*
andReturnUnsignedIntValue_c(unsigned int value)
{
  expectedCall = &expectedCall->andReturnValue(value);
  return &gExpectedCall;
}

MockExpectedCall_c*
andReturnIntValue_c(int value)
{
  expectedCall = &expectedCall->andReturnValue(value);
  return &gExpectedCall;
}

MockExpectedCall_c*
andReturnLongIntValue_c(long int value)
{
  expectedCall = &expectedCall->andReturnValue(value);
  return &gExpectedCall;
}

MockExpectedCall_c*
andReturnUnsignedLongIntValue_c(unsigned long int value)
{
  expectedCall = &expectedCall->andReturnValue(value);
  return &gExpectedCall;
}

MockExpectedCall_c*
andReturnLongLongIntValue_c(long long value)
{
  expectedCall = &expectedCall->andReturnValue(value);
  return &gExpectedCall;
}

MockExpectedCall_c*
andReturnUnsignedLongLongIntValue_c(unsigned long long value)
{
  expectedCall = &expectedCall->andReturnValue(value);
  return &gExpectedCall;
}

MockExpectedCall_c*
andReturnDoubleValue_c(double value)
{
  expectedCall = &expectedCall->andReturnValue(value);
  return &gExpectedCall;
}

MockExpectedCall_c*
andReturnStringValue_c(const char* value)
{
  expectedCall = &expectedCall->andReturnValue(value);
  return &gExpectedCall;
}

MockExpectedCall_c*
andReturnPointerValue_c(void* value)
{
  expectedCall = &expectedCall->andReturnValue(value);
  return &gExpectedCall;
}

MockExpectedCall_c*
andReturnConstPointerValue_c(const void* value)
{
  expectedCall = &expectedCall->andReturnValue(value);
  return &gExpectedCall;
}

MockExpectedCall_c*
andReturnFunctionPointerValue_c(void (*value)())
{
  expectedCall = &expectedCall->andReturnValue(value);
  return &gExpectedCall;
}

static MockValue_c
getMockValueCFromNamedValue(const MockNamedValue& namedValue)
{
  MockValue_c returnValue;
  if (StrCmp(namedValue.getType().c_str(), "bool") == 0) {
    returnValue.type = MOCKVALUETYPE_BOOL;
    returnValue.value.boolValue = namedValue.getBoolValue() ? 1 : 0;
  } else if (StrCmp(namedValue.getType().c_str(), "int") == 0) {
    returnValue.type = MOCKVALUETYPE_INTEGER;
    returnValue.value.intValue = namedValue.getIntValue();
  } else if (StrCmp(namedValue.getType().c_str(), "unsigned int") == 0) {
    returnValue.type = MOCKVALUETYPE_UNSIGNED_INTEGER;
    returnValue.value.unsignedIntValue = namedValue.getUnsignedIntValue();
  } else if (StrCmp(namedValue.getType().c_str(), "long int") == 0) {
    returnValue.type = MOCKVALUETYPE_LONG_INTEGER;
    returnValue.value.longIntValue = namedValue.getLongIntValue();
  } else if (StrCmp(namedValue.getType().c_str(), "unsigned long int") == 0) {
    returnValue.type = MOCKVALUETYPE_UNSIGNED_LONG_INTEGER;
    returnValue.value.unsignedLongIntValue =
        namedValue.getUnsignedLongIntValue();
  } else if (StrCmp(namedValue.getType().c_str(), "long long int") == 0) {
    returnValue.type = MOCKVALUETYPE_LONG_LONG_INTEGER;
    returnValue.value.longLongIntValue = namedValue.getLongLongIntValue();
  } else if (StrCmp(namedValue.getType().c_str(), "unsigned long long int") ==
             0) {
    returnValue.type = MOCKVALUETYPE_UNSIGNED_LONG_LONG_INTEGER;
    returnValue.value.unsignedLongLongIntValue =
        namedValue.getUnsignedLongLongIntValue();
  } else if (StrCmp(namedValue.getType().c_str(), "double") == 0) {
    returnValue.type = MOCKVALUETYPE_DOUBLE;
    returnValue.value.doubleValue = namedValue.getDoubleValue();
  } else if (StrCmp(namedValue.getType().c_str(), "const char*") == 0) {
    returnValue.type = MOCKVALUETYPE_STRING;
    returnValue.value.stringValue = namedValue.getStringValue();
  } else if (StrCmp(namedValue.getType().c_str(), "void*") == 0) {
    returnValue.type = MOCKVALUETYPE_POINTER;
    returnValue.value.pointerValue = namedValue.getPointerValue();
  } else if (StrCmp(namedValue.getType().c_str(), "const void*") == 0) {
    returnValue.type = MOCKVALUETYPE_CONST_POINTER;
    returnValue.value.constPointerValue = namedValue.getConstPointerValue();
  } else if (StrCmp(namedValue.getType().c_str(), "void (*)()") == 0) {
    returnValue.type = MOCKVALUETYPE_FUNCTIONPOINTER;
    returnValue.value.functionPointerValue =
        namedValue.getFunctionPointerValue();
  } else if (StrCmp(namedValue.getType().c_str(), "const unsigned char*") ==
             0) {
    returnValue.type = MOCKVALUETYPE_MEMORYBUFFER;
    returnValue.value.memoryBufferValue = namedValue.getMemoryBuffer();
  } else if (namedValue.isConstObject()) {
    returnValue.type = MOCKVALUETYPE_CONST_OBJECT;
    returnValue.value.constObjectValue = namedValue.getConstObjectPointer();
  } else {
    returnValue.type = MOCKVALUETYPE_OBJECT;
    returnValue.value.objectValue = namedValue.getObjectPointer();
  }
  return returnValue;
}

void
strictOrder_c()
{
  currentMockSupport->strictOrder();
}

MockExpectedCall_c*
expectOneCall_c(const char* name)
{
  expectedCall = &currentMockSupport->expectOneCall(name);
  return &gExpectedCall;
}

void
expectNoCall_c(const char* name)
{
  currentMockSupport->expectNoCall(name);
}

MockExpectedCall_c*
expectNCalls_c(const unsigned int number, const char* name)
{
  expectedCall = &currentMockSupport->expectNCalls(number, name);
  return &gExpectedCall;
}

MockActualCall_c*
actualCall_c(const char* name)
{
  actualCall = &currentMockSupport->actualCall(name);
  return &gActualCall;
}

MockActualCall_c*
withActualBoolParameters_c(const char* name, int value)
{
  actualCall = &actualCall->withParameter(name, (value != 0));
  return &gActualCall;
}

MockActualCall_c*
withActualIntParameters_c(const char* name, int value)
{
  actualCall = &actualCall->withParameter(name, value);
  return &gActualCall;
}

MockActualCall_c*
withActualUnsignedIntParameters_c(const char* name, unsigned int value)
{
  actualCall = &actualCall->withParameter(name, value);
  return &gActualCall;
}

MockActualCall_c*
withActualLongIntParameters_c(const char* name, long int value)
{
  actualCall = &actualCall->withParameter(name, value);
  return &gActualCall;
}

MockActualCall_c*
withActualUnsignedLongIntParameters_c(const char* name, unsigned long int value)
{
  actualCall = &actualCall->withParameter(name, value);
  return &gActualCall;
}

MockActualCall_c*
withActualLongLongIntParameters_c(const char* name, long long value)
{
  actualCall = &actualCall->withParameter(name, value);
  return &gActualCall;
}

MockActualCall_c*
withActualUnsignedLongLongIntParameters_c(const char* name,
    unsigned long long value)
{
  actualCall = &actualCall->withParameter(name, value);
  return &gActualCall;
}

MockActualCall_c*
withActualDoubleParameters_c(const char* name, double value)
{
  actualCall = &actualCall->withParameter(name, value);
  return &gActualCall;
}

MockActualCall_c*
withActualStringParameters_c(const char* name, const char* value)
{
  actualCall = &actualCall->withParameter(name, value);
  return &gActualCall;
}

MockActualCall_c*
withActualPointerParameters_c(const char* name, void* value)
{
  actualCall = &actualCall->withParameter(name, value);
  return &gActualCall;
}

MockActualCall_c*
withActualConstPointerParameters_c(const char* name, const void* value)
{
  actualCall = &actualCall->withParameter(name, value);
  return &gActualCall;
}

MockActualCall_c*
withActualFunctionPointerParameters_c(const char* name, void (*value)())
{
  actualCall = &actualCall->withParameter(name, value);
  return &gActualCall;
}

MockActualCall_c*
withActualMemoryBufferParameters_c(const char* name,
    const unsigned char* value,
    size_t size)
{
  actualCall = &actualCall->withParameter(name, value, size);
  return &gActualCall;
}

MockActualCall_c*
withActualParameterOfType_c(const char* type,
    const char* name,
    const void* value)
{
  actualCall = &actualCall->withParameterOfType(type, name, value);
  return &gActualCall;
}

MockActualCall_c*
withActualOutputParameter_c(const char* name, void* value)
{
  actualCall = &actualCall->withOutputParameter(name, value);
  return &gActualCall;
}

MockActualCall_c*
withActualOutputParameterOfType_c(const char* type,
    const char* name,
    void* value)
{
  actualCall = &actualCall->withOutputParameterOfType(type, name, value);
  return &gActualCall;
}

MockValue_c
returnValue_c()
{
  return getMockValueCFromNamedValue(actualCall->returnValue());
}

int
boolReturnValue_c()
{
  return actualCall->returnBoolValue() ? 1 : 0;
}

int
returnBoolValueOrDefault_c(int defaultValue)
{
  if (!hasReturnValue_c()) {
    return defaultValue;
  }
  return boolReturnValue_c();
}

int
intReturnValue_c()
{
  return actualCall->returnIntValue();
}

int
returnIntValueOrDefault_c(int defaultValue)
{
  if (!hasReturnValue_c()) {
    return defaultValue;
  }
  return intReturnValue_c();
}

unsigned int
unsignedIntReturnValue_c()
{
  return actualCall->returnUnsignedIntValue();
}

unsigned int
returnUnsignedIntValueOrDefault_c(unsigned int defaultValue)
{
  if (!hasReturnValue_c()) {
    return defaultValue;
  }
  return unsignedIntReturnValue_c();
}

long int
longIntReturnValue_c()
{
  return actualCall->returnLongIntValue();
}

long int
returnLongIntValueOrDefault_c(long int defaultValue)
{
  if (!hasReturnValue_c()) {
    return defaultValue;
  }
  return longIntReturnValue_c();
}

unsigned long int
unsignedLongIntReturnValue_c()
{
  return actualCall->returnUnsignedLongIntValue();
}

unsigned long int
returnUnsignedLongIntValueOrDefault_c(unsigned long int defaultValue)
{
  if (!hasReturnValue_c()) {
    return defaultValue;
  }
  return unsignedLongIntReturnValue_c();
}

long long
longLongIntReturnValue_c()
{
  return actualCall->returnLongLongIntValue();
}

long long
returnLongLongIntValueOrDefault_c(long long defaultValue)
{
  if (!hasReturnValue_c()) {
    return defaultValue;
  }
  return longLongIntReturnValue_c();
}

unsigned long long
unsignedLongLongIntReturnValue_c()
{
  return actualCall->returnUnsignedLongLongIntValue();
}

unsigned long long
returnUnsignedLongLongIntValueOrDefault_c(unsigned long long defaultValue)
{
  if (!hasReturnValue_c()) {
    return defaultValue;
  }
  return unsignedLongLongIntReturnValue_c();
}

const char*
stringReturnValue_c()
{
  return actualCall->returnStringValue();
}

const char*
returnStringValueOrDefault_c(const char* defaultValue)
{
  if (!hasReturnValue_c()) {
    return defaultValue;
  }
  return stringReturnValue_c();
}

double
doubleReturnValue_c()
{
  return actualCall->returnDoubleValue();
}

double
returnDoubleValueOrDefault_c(double defaultValue)
{
  if (!hasReturnValue_c()) {
    return defaultValue;
  }
  return doubleReturnValue_c();
}

void*
pointerReturnValue_c()
{
  return actualCall->returnPointerValue();
}

void*
returnPointerValueOrDefault_c(void* defaultValue)
{
  if (!hasReturnValue_c()) {
    return defaultValue;
  }
  return pointerReturnValue_c();
}

const void*
constPointerReturnValue_c()
{
  return actualCall->returnConstPointerValue();
}

const void*
returnConstPointerValueOrDefault_c(const void* defaultValue)
{
  if (!hasReturnValue_c()) {
    return defaultValue;
  }
  return constPointerReturnValue_c();
}

void (*functionPointerReturnValue_c())()
{
  return actualCall->returnFunctionPointerValue();
}

void (*returnFunctionPointerValueOrDefault_c(void (*defaultValue)()))()
{
  if (!hasReturnValue_c()) {
    return defaultValue;
  }
  return functionPointerReturnValue_c();
}

void
disable_c()
{
  currentMockSupport->disable();
}

void
enable_c()
{
  currentMockSupport->enable();
}

void
ignoreOtherCalls_c()
{
  currentMockSupport->ignoreOtherCalls();
}

void
setBoolData_c(const char* name, int value)
{
  currentMockSupport->setData(name, (value != 0));
}

void
setIntData_c(const char* name, int value)
{
  currentMockSupport->setData(name, value);
}

void
setUnsignedIntData_c(const char* name, unsigned int value)
{
  currentMockSupport->setData(name, value);
}

void
setLongIntData_c(const char* name, long int value)
{
  currentMockSupport->setData(name, value);
}

void
setUnsignedLongIntData_c(const char* name, unsigned long int value)
{
  currentMockSupport->setData(name, value);
}

void
setDoubleData_c(const char* name, double value)
{
  currentMockSupport->setData(name, value);
}

void
setStringData_c(const char* name, const char* value)
{
  currentMockSupport->setData(name, value);
}

void
setPointerData_c(const char* name, void* value)
{
  currentMockSupport->setData(name, value);
}

void
setConstPointerData_c(const char* name, const void* value)
{
  currentMockSupport->setData(name, value);
}

void
setFunctionPointerData_c(const char* name, void (*value)())
{
  currentMockSupport->setData(name, value);
}

void
setDataObject_c(const char* name, const char* type, void* value)
{
  currentMockSupport->setDataObject(name, type, value);
}

void
setDataConstObject_c(const char* name, const char* type, const void* value)
{
  currentMockSupport->setDataConstObject(name, type, value);
}

MockValue_c
getData_c(const char* name)
{
  return getMockValueCFromNamedValue(currentMockSupport->getData(name));
}

int
hasReturnValue_c()
{
  return currentMockSupport->hasReturnValue();
}

void
checkExpectations_c()
{
  currentMockSupport->checkExpectations();
}

int
expectedCallsLeft_c()
{
  return currentMockSupport->expectedCallsLeft();
}

void
clear_c()
{
  currentMockSupport->clear();
}

void
crashOnFailure_c(unsigned shouldCrash)
{
  currentMockSupport->crashOnFailure(0 != shouldCrash);
}

static MockSupport_c gMockSupport = { strictOrder_c,
  expectOneCall_c,
  expectNoCall_c,
  expectNCalls_c,
  actualCall_c,
  hasReturnValue_c,
  returnValue_c,
  boolReturnValue_c,
  returnBoolValueOrDefault_c,
  intReturnValue_c,
  returnIntValueOrDefault_c,
  unsignedIntReturnValue_c,
  returnUnsignedIntValueOrDefault_c,
  longIntReturnValue_c,
  returnLongIntValueOrDefault_c,
  unsignedLongIntReturnValue_c,
  returnUnsignedLongIntValueOrDefault_c,
  longLongIntReturnValue_c,
  returnLongLongIntValueOrDefault_c,
  unsignedLongLongIntReturnValue_c,
  returnUnsignedLongLongIntValueOrDefault_c,
  stringReturnValue_c,
  returnStringValueOrDefault_c,
  doubleReturnValue_c,
  returnDoubleValueOrDefault_c,
  pointerReturnValue_c,
  returnPointerValueOrDefault_c,
  constPointerReturnValue_c,
  returnConstPointerValueOrDefault_c,
  functionPointerReturnValue_c,
  returnFunctionPointerValueOrDefault_c,
  setBoolData_c,
  setIntData_c,
  setUnsignedIntData_c,
  setLongIntData_c,
  setUnsignedLongIntData_c,
  setStringData_c,
  setDoubleData_c,
  setPointerData_c,
  setConstPointerData_c,
  setFunctionPointerData_c,
  setDataObject_c,
  setDataConstObject_c,
  getData_c,
  disable_c,
  enable_c,
  ignoreOtherCalls_c,
  checkExpectations_c,
  expectedCallsLeft_c,
  clear_c,
  crashOnFailure_c,
  installComparator_c,
  installCopier_c,
  removeAllComparatorsAndCopiers_c };
}
} // namespace extensions
} // namespace cpputest

MockSupport_c*
mock_c()
{
  cpputest::extensions::currentMockSupport =
      &mock("", &cpputest::extensions::failureReporterForC);
  return &cpputest::extensions::gMockSupport;
}

MockSupport_c*
mock_scope_c(const char* scope)
{
  cpputest::extensions::currentMockSupport =
      &mock(scope, &cpputest::extensions::failureReporterForC);
  return &cpputest::extensions::gMockSupport;
}
