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
      stringContains(node->getName(), MOCK_SUPPORT_SCOPE_PREFIX))
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

} // namespace extensions
} // namespace cpputest

namespace c {

class MockCFunctionComparatorNode
  : public cpputest::extensions::MockNamedValueComparator
{
public:
  MockCFunctionComparatorNode(MockCFunctionComparatorNode* next,
      MockTypeEqualFunction equal,
      MockTypeValueToStringFunction toString)
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
  virtual cpputest::String valueToString(const void* object) override
  {
    return cpputest::String(toString_(object));
  }

  MockCFunctionComparatorNode* next_;
  MockTypeEqualFunction equal_;
  MockTypeValueToStringFunction toString_;
};

static MockCFunctionComparatorNode* comparatorList_ = nullptr;

class MockCFunctionCopierNode
  : public cpputest::extensions::MockNamedValueCopier
{
public:
  MockCFunctionCopierNode(MockCFunctionCopierNode* next,
      MockTypeCopyFunction copier)
    : next_(next)
    , copier_(copier)
  {
  }
  virtual ~MockCFunctionCopierNode() override {}

  virtual void copy(void* dst, const void* src) override { copier_(dst, src); }

  MockCFunctionCopierNode* next_;
  MockTypeCopyFunction copier_;
};

static MockCFunctionCopierNode* copierList_ = nullptr;

void
strictOrder();
struct MockExpectedCall*
expectOneCall(const char* name);
void
expectNoCall(const char* name);
struct MockExpectedCall*
expectNCalls(const unsigned int number, const char* name);
struct MockActualCall*
actualCall(const char* name);
void
disable();
void
enable();
void
ignoreOtherCalls();
void
setBoolData(const char* name, int value);
void
setIntData(const char* name, int value);
void
setUnsignedIntData(const char* name, unsigned int value);
void
setLongIntData(const char* name, long int value);
void
setUnsignedLongIntData(const char* name, unsigned long int value);
void
setDoubleData(const char* name, double value);
void
setStringData(const char* name, const char* value);
void
setPointerData(const char* name, void* value);
void
setConstPointerData(const char* name, const void* value);
void
setFunctionPointerData(const char* name, void (*value)());
void
setDataObject(const char* name, const char* type, void* value);
void
setDataConstObject(const char* name, const char* type, const void* value);
struct MockValue
getData(const char* name);
int
hasReturnValue();

void
checkExpectations();
int
expectedCallsLeft();
void
clear();
void
crashOnFailure(unsigned shouldCrash);

struct MockExpectedCall*
withBoolParameters(const char* name, int value);
struct MockExpectedCall*
withIntParameters(const char* name, int value);
struct MockExpectedCall*
withUnsignedIntParameters(const char* name, unsigned int value);
struct MockExpectedCall*
withLongIntParameters(const char* name, long int value);
struct MockExpectedCall*
withUnsignedLongIntParameters(const char* name, unsigned long int value);
struct MockExpectedCall*
withLongLongIntParameters(const char* name, long long value);
struct MockExpectedCall*
withUnsignedLongLongIntParameters(const char* name, unsigned long long value);
struct MockExpectedCall*
withDoubleParameters(const char* name, double value);
struct MockExpectedCall*
withDoubleParametersAndTolerance(const char* name,
    double value,
    double tolerance);
struct MockExpectedCall*
withStringParameters(const char* name, const char* value);
struct MockExpectedCall*
withPointerParameters(const char* name, void* value);
struct MockExpectedCall*
withConstPointerParameters(const char* name, const void* value);
struct MockExpectedCall*
withFunctionPointerParameters(const char* name, void (*value)());
struct MockExpectedCall*
withMemoryBufferParameters(const char* name,
    const unsigned char* value,
    size_t size);
struct MockExpectedCall*
withParameterOfType(const char* type, const char* name, const void* value);
struct MockExpectedCall*
withOutputParameterReturning(const char* name, const void* value, size_t size);
struct MockExpectedCall*
withOutputParameterOfTypeReturning(const char* type,
    const char* name,
    const void* value);
struct MockExpectedCall*
withUnmodifiedOutputParameter(const char* name);
struct MockExpectedCall*
ignoreOtherParameters();
struct MockExpectedCall*
andReturnBoolValue(int value);
struct MockExpectedCall*
andReturnIntValue(int value);
struct MockExpectedCall*
andReturnUnsignedIntValue(unsigned int value);
struct MockExpectedCall*
andReturnLongIntValue(long int value);
struct MockExpectedCall*
andReturnUnsignedLongIntValue(unsigned long int value);
struct MockExpectedCall*
andReturnLongLongIntValue(long long value);
struct MockExpectedCall*
andReturnUnsignedLongLongIntValue(unsigned long long value);
struct MockExpectedCall*
andReturnDoubleValue(double value);
struct MockExpectedCall*
andReturnStringValue(const char* value);
struct MockExpectedCall*
andReturnPointerValue(void* value);
struct MockExpectedCall*
andReturnConstPointerValue(const void* value);
struct MockExpectedCall*
andReturnFunctionPointerValue(void (*value)());

struct MockActualCall*
withActualBoolParameters(const char* name, int value);
struct MockActualCall*
withActualIntParameters(const char* name, int value);
struct MockActualCall*
withActualUnsignedIntParameters(const char* name, unsigned int value);
struct MockActualCall*
withActualLongIntParameters(const char* name, long int value);
struct MockActualCall*
withActualUnsignedLongIntParameters(const char* name, unsigned long int value);
struct MockActualCall*
withActualLongLongIntParameters(const char* name, long long value);
struct MockActualCall*
withActualUnsignedLongLongIntParameters(const char* name,
    unsigned long long value);
struct MockActualCall*
withActualDoubleParameters(const char* name, double value);
struct MockActualCall*
withActualStringParameters(const char* name, const char* value);
struct MockActualCall*
withActualPointerParameters(const char* name, void* value);
struct MockActualCall*
withActualConstPointerParameters(const char* name, const void* value);
struct MockActualCall*
withActualFunctionPointerParameters(const char* name, void (*value)());
struct MockActualCall*
withActualMemoryBufferParameters(const char* name,
    const unsigned char* value,
    size_t size);
struct MockActualCall*
withActualParameterOfType(const char* type,
    const char* name,
    const void* value);
struct MockActualCall*
withActualOutputParameter(const char* name, void* value);
struct MockActualCall*
withActualOutputParameterOfType(const char* type,
    const char* name,
    void* value);
struct MockValue
returnValue();
int
boolReturnValue();
int
returnBoolValueOrDefault(int defaultValue);
int
intReturnValue();
int
returnIntValueOrDefault(int defaultValue);
unsigned int
unsignedIntReturnValue();
unsigned int
returnUnsignedIntValueOrDefault(unsigned int defaultValue);
long int
longIntReturnValue();
long int
returnLongIntValueOrDefault(long int defaultValue);
unsigned long int
unsignedLongIntReturnValue();
unsigned long int
returnUnsignedLongIntValueOrDefault(unsigned long int defaultValue);
long long
longLongIntReturnValue();
long long
returnLongLongIntValueOrDefault(long long defaultValue);
unsigned long long
unsignedLongLongIntReturnValue();
unsigned long long
returnUnsignedLongLongIntValueOrDefault(unsigned long long defaultValue);
const char*
stringReturnValue();
const char*
returnStringValueOrDefault(const char* defaultValue);
double
doubleReturnValue();
double
returnDoubleValueOrDefault(double defaultValue);
void*
pointerReturnValue();
void*
returnPointerValueOrDefault(void* defaultValue);
const void*
constPointerReturnValue();
const void*
returnConstPointerValueOrDefault(const void* defaultValue);
void (*functionPointerReturnValue())();
void (*returnFunctionPointerValueOrDefault(void (*defaultValue)()))();

static void
installComparator(const char* typeName,
    MockTypeEqualFunction isEqual,
    MockTypeValueToStringFunction valueToString)
{
  comparatorList_ =
      new MockCFunctionComparatorNode(comparatorList_, isEqual, valueToString);
  cpputest::extensions::currentMockSupport->installComparator(
      typeName, *comparatorList_);
}

static void
installCopier(const char* typeName, MockTypeCopyFunction copier)
{
  copierList_ = new MockCFunctionCopierNode(copierList_, copier);
  cpputest::extensions::currentMockSupport->installCopier(
      typeName, *copierList_);
}

static void
removeAllComparatorsAndCopiers()
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
  cpputest::extensions::currentMockSupport->removeAllComparatorsAndCopiers();
}

static struct MockExpectedCall gExpectedCall = {
  withBoolParameters,
  withIntParameters,
  withUnsignedIntParameters,
  withLongIntParameters,
  withUnsignedLongIntParameters,
  withLongLongIntParameters,
  withUnsignedLongLongIntParameters,
  withDoubleParameters,
  withDoubleParametersAndTolerance,
  withStringParameters,
  withPointerParameters,
  withConstPointerParameters,
  withFunctionPointerParameters,
  withMemoryBufferParameters,
  withParameterOfType,
  withOutputParameterReturning,
  withOutputParameterOfTypeReturning,
  withUnmodifiedOutputParameter,
  ignoreOtherParameters,
  andReturnBoolValue,
  andReturnUnsignedIntValue,
  andReturnIntValue,
  andReturnLongIntValue,
  andReturnUnsignedLongIntValue,
  andReturnLongLongIntValue,
  andReturnUnsignedLongLongIntValue,
  andReturnDoubleValue,
  andReturnStringValue,
  andReturnPointerValue,
  andReturnConstPointerValue,
  andReturnFunctionPointerValue,
};

static struct MockActualCall gActualCall = { withActualBoolParameters,
  withActualIntParameters,
  withActualUnsignedIntParameters,
  withActualLongIntParameters,
  withActualUnsignedLongIntParameters,
  withActualLongLongIntParameters,
  withActualUnsignedLongLongIntParameters,
  withActualDoubleParameters,
  withActualStringParameters,
  withActualPointerParameters,
  withActualConstPointerParameters,
  withActualFunctionPointerParameters,
  withActualMemoryBufferParameters,
  withActualParameterOfType,
  withActualOutputParameter,
  withActualOutputParameterOfType,
  hasReturnValue,
  returnValue,
  boolReturnValue,
  returnBoolValueOrDefault,
  intReturnValue,
  returnIntValueOrDefault,
  unsignedIntReturnValue,
  returnUnsignedIntValueOrDefault,
  longIntReturnValue,
  returnLongIntValueOrDefault,
  unsignedLongIntReturnValue,
  returnUnsignedLongIntValueOrDefault,
  longLongIntReturnValue,
  returnLongLongIntValueOrDefault,
  unsignedLongLongIntReturnValue,
  returnUnsignedLongLongIntValueOrDefault,
  stringReturnValue,
  returnStringValueOrDefault,
  doubleReturnValue,
  returnDoubleValueOrDefault,
  pointerReturnValue,
  returnPointerValueOrDefault,
  constPointerReturnValue,
  returnConstPointerValueOrDefault,
  functionPointerReturnValue,
  returnFunctionPointerValueOrDefault };

struct MockExpectedCall*
withBoolParameters(const char* name, int value)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->withParameter(name, (value != 0));
  return &gExpectedCall;
}

struct MockExpectedCall*
withIntParameters(const char* name, int value)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->withParameter(name, value);
  return &gExpectedCall;
}

struct MockExpectedCall*
withUnsignedIntParameters(const char* name, unsigned int value)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->withParameter(name, value);
  return &gExpectedCall;
}

struct MockExpectedCall*
withLongIntParameters(const char* name, long int value)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->withParameter(name, value);
  return &gExpectedCall;
}

struct MockExpectedCall*
withUnsignedLongIntParameters(const char* name, unsigned long int value)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->withParameter(name, value);
  return &gExpectedCall;
}

struct MockExpectedCall*
withLongLongIntParameters(const char* name, long long value)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->withParameter(name, value);
  return &gExpectedCall;
}

struct MockExpectedCall*
withUnsignedLongLongIntParameters(const char* name, unsigned long long value)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->withParameter(name, value);
  return &gExpectedCall;
}

struct MockExpectedCall*
withDoubleParameters(const char* name, double value)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->withParameter(name, value);
  return &gExpectedCall;
}

struct MockExpectedCall*
withDoubleParametersAndTolerance(const char* name,
    double value,
    double tolerance)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->withParameter(
          name, value, tolerance);
  return &gExpectedCall;
}

struct MockExpectedCall*
withStringParameters(const char* name, const char* value)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->withParameter(name, value);
  return &gExpectedCall;
}

struct MockExpectedCall*
withPointerParameters(const char* name, void* value)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->withParameter(name, value);
  return &gExpectedCall;
}

struct MockExpectedCall*
withConstPointerParameters(const char* name, const void* value)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->withParameter(name, value);
  return &gExpectedCall;
}

struct MockExpectedCall*
withFunctionPointerParameters(const char* name, void (*value)())
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->withParameter(name, value);
  return &gExpectedCall;
}

struct MockExpectedCall*
withMemoryBufferParameters(const char* name,
    const unsigned char* value,
    size_t size)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->withParameter(name, value, size);
  return &gExpectedCall;
}

struct MockExpectedCall*
withParameterOfType(const char* type, const char* name, const void* value)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->withParameterOfType(
          type, name, value);
  return &gExpectedCall;
}

struct MockExpectedCall*
withOutputParameterReturning(const char* name, const void* value, size_t size)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->withOutputParameterReturning(
          name, value, size);
  return &gExpectedCall;
}

struct MockExpectedCall*
withOutputParameterOfTypeReturning(const char* type,
    const char* name,
    const void* value)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->withOutputParameterOfTypeReturning(
          type, name, value);
  return &gExpectedCall;
}

struct MockExpectedCall*
withUnmodifiedOutputParameter(const char* name)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->withUnmodifiedOutputParameter(name);
  return &gExpectedCall;
}

struct MockExpectedCall*
ignoreOtherParameters()
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->ignoreOtherParameters();
  return &gExpectedCall;
}

struct MockExpectedCall*
andReturnBoolValue(int value)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->andReturnValue(value != 0);
  return &gExpectedCall;
}

struct MockExpectedCall*
andReturnUnsignedIntValue(unsigned int value)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->andReturnValue(value);
  return &gExpectedCall;
}

struct MockExpectedCall*
andReturnIntValue(int value)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->andReturnValue(value);
  return &gExpectedCall;
}

struct MockExpectedCall*
andReturnLongIntValue(long int value)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->andReturnValue(value);
  return &gExpectedCall;
}

struct MockExpectedCall*
andReturnUnsignedLongIntValue(unsigned long int value)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->andReturnValue(value);
  return &gExpectedCall;
}

struct MockExpectedCall*
andReturnLongLongIntValue(long long value)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->andReturnValue(value);
  return &gExpectedCall;
}

struct MockExpectedCall*
andReturnUnsignedLongLongIntValue(unsigned long long value)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->andReturnValue(value);
  return &gExpectedCall;
}

struct MockExpectedCall*
andReturnDoubleValue(double value)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->andReturnValue(value);
  return &gExpectedCall;
}

struct MockExpectedCall*
andReturnStringValue(const char* value)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->andReturnValue(value);
  return &gExpectedCall;
}

struct MockExpectedCall*
andReturnPointerValue(void* value)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->andReturnValue(value);
  return &gExpectedCall;
}

struct MockExpectedCall*
andReturnConstPointerValue(const void* value)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->andReturnValue(value);
  return &gExpectedCall;
}

struct MockExpectedCall*
andReturnFunctionPointerValue(void (*value)())
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::expectedCall->andReturnValue(value);
  return &gExpectedCall;
}

static struct MockValue
getMockValueCFromNamedValue(
    const cpputest::extensions::MockNamedValue& namedValue)
{
  using cpputest::StrCmp;
  struct MockValue returnValue;
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
strictOrder()
{
  cpputest::extensions::currentMockSupport->strictOrder();
}

struct MockExpectedCall*
expectOneCall(const char* name)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::currentMockSupport->expectOneCall(name);
  return &gExpectedCall;
}

void
expectNoCall(const char* name)
{
  cpputest::extensions::currentMockSupport->expectNoCall(name);
}

struct MockExpectedCall*
expectNCalls(const unsigned int number, const char* name)
{
  cpputest::extensions::expectedCall =
      &cpputest::extensions::currentMockSupport->expectNCalls(number, name);
  return &gExpectedCall;
}

struct MockActualCall*
actualCall(const char* name)
{
  cpputest::extensions::actualCall =
      &cpputest::extensions::currentMockSupport->actualCall(name);
  return &gActualCall;
}

struct MockActualCall*
withActualBoolParameters(const char* name, int value)
{
  cpputest::extensions::actualCall =
      &cpputest::extensions::actualCall->withParameter(name, (value != 0));
  return &gActualCall;
}

struct MockActualCall*
withActualIntParameters(const char* name, int value)
{
  cpputest::extensions::actualCall =
      &cpputest::extensions::actualCall->withParameter(name, value);
  return &gActualCall;
}

struct MockActualCall*
withActualUnsignedIntParameters(const char* name, unsigned int value)
{
  cpputest::extensions::actualCall =
      &cpputest::extensions::actualCall->withParameter(name, value);
  return &gActualCall;
}

struct MockActualCall*
withActualLongIntParameters(const char* name, long int value)
{
  cpputest::extensions::actualCall =
      &cpputest::extensions::actualCall->withParameter(name, value);
  return &gActualCall;
}

struct MockActualCall*
withActualUnsignedLongIntParameters(const char* name, unsigned long int value)
{
  cpputest::extensions::actualCall =
      &cpputest::extensions::actualCall->withParameter(name, value);
  return &gActualCall;
}

struct MockActualCall*
withActualLongLongIntParameters(const char* name, long long value)
{
  cpputest::extensions::actualCall =
      &cpputest::extensions::actualCall->withParameter(name, value);
  return &gActualCall;
}

struct MockActualCall*
withActualUnsignedLongLongIntParameters(const char* name,
    unsigned long long value)
{
  cpputest::extensions::actualCall =
      &cpputest::extensions::actualCall->withParameter(name, value);
  return &gActualCall;
}

struct MockActualCall*
withActualDoubleParameters(const char* name, double value)
{
  cpputest::extensions::actualCall =
      &cpputest::extensions::actualCall->withParameter(name, value);
  return &gActualCall;
}

struct MockActualCall*
withActualStringParameters(const char* name, const char* value)
{
  cpputest::extensions::actualCall =
      &cpputest::extensions::actualCall->withParameter(name, value);
  return &gActualCall;
}

struct MockActualCall*
withActualPointerParameters(const char* name, void* value)
{
  cpputest::extensions::actualCall =
      &cpputest::extensions::actualCall->withParameter(name, value);
  return &gActualCall;
}

struct MockActualCall*
withActualConstPointerParameters(const char* name, const void* value)
{
  cpputest::extensions::actualCall =
      &cpputest::extensions::actualCall->withParameter(name, value);
  return &gActualCall;
}

struct MockActualCall*
withActualFunctionPointerParameters(const char* name, void (*value)())
{
  cpputest::extensions::actualCall =
      &cpputest::extensions::actualCall->withParameter(name, value);
  return &gActualCall;
}

struct MockActualCall*
withActualMemoryBufferParameters(const char* name,
    const unsigned char* value,
    size_t size)
{
  cpputest::extensions::actualCall =
      &cpputest::extensions::actualCall->withParameter(name, value, size);
  return &gActualCall;
}

struct MockActualCall*
withActualParameterOfType(const char* type, const char* name, const void* value)
{
  cpputest::extensions::actualCall =
      &cpputest::extensions::actualCall->withParameterOfType(type, name, value);
  return &gActualCall;
}

struct MockActualCall*
withActualOutputParameter(const char* name, void* value)
{
  cpputest::extensions::actualCall =
      &cpputest::extensions::actualCall->withOutputParameter(name, value);
  return &gActualCall;
}

struct MockActualCall*
withActualOutputParameterOfType(const char* type, const char* name, void* value)
{
  cpputest::extensions::actualCall =
      &cpputest::extensions::actualCall->withOutputParameterOfType(
          type, name, value);
  return &gActualCall;
}

struct MockValue
returnValue()
{
  return getMockValueCFromNamedValue(
      cpputest::extensions::actualCall->returnValue());
}

int
boolReturnValue()
{
  return cpputest::extensions::actualCall->returnBoolValue() ? 1 : 0;
}

int
returnBoolValueOrDefault(int defaultValue)
{
  if (!hasReturnValue()) {
    return defaultValue;
  }
  return boolReturnValue();
}

int
intReturnValue()
{
  return cpputest::extensions::actualCall->returnIntValue();
}

int
returnIntValueOrDefault(int defaultValue)
{
  if (!hasReturnValue()) {
    return defaultValue;
  }
  return intReturnValue();
}

unsigned int
unsignedIntReturnValue()
{
  return cpputest::extensions::actualCall->returnUnsignedIntValue();
}

unsigned int
returnUnsignedIntValueOrDefault(unsigned int defaultValue)
{
  if (!hasReturnValue()) {
    return defaultValue;
  }
  return unsignedIntReturnValue();
}

long int
longIntReturnValue()
{
  return cpputest::extensions::actualCall->returnLongIntValue();
}

long int
returnLongIntValueOrDefault(long int defaultValue)
{
  if (!hasReturnValue()) {
    return defaultValue;
  }
  return longIntReturnValue();
}

unsigned long int
unsignedLongIntReturnValue()
{
  return cpputest::extensions::actualCall->returnUnsignedLongIntValue();
}

unsigned long int
returnUnsignedLongIntValueOrDefault(unsigned long int defaultValue)
{
  if (!hasReturnValue()) {
    return defaultValue;
  }
  return unsignedLongIntReturnValue();
}

long long
longLongIntReturnValue()
{
  return cpputest::extensions::actualCall->returnLongLongIntValue();
}

long long
returnLongLongIntValueOrDefault(long long defaultValue)
{
  if (!hasReturnValue()) {
    return defaultValue;
  }
  return longLongIntReturnValue();
}

unsigned long long
unsignedLongLongIntReturnValue()
{
  return cpputest::extensions::actualCall->returnUnsignedLongLongIntValue();
}

unsigned long long
returnUnsignedLongLongIntValueOrDefault(unsigned long long defaultValue)
{
  if (!hasReturnValue()) {
    return defaultValue;
  }
  return unsignedLongLongIntReturnValue();
}

const char*
stringReturnValue()
{
  return cpputest::extensions::actualCall->returnStringValue();
}

const char*
returnStringValueOrDefault(const char* defaultValue)
{
  if (!hasReturnValue()) {
    return defaultValue;
  }
  return stringReturnValue();
}

double
doubleReturnValue()
{
  return cpputest::extensions::actualCall->returnDoubleValue();
}

double
returnDoubleValueOrDefault(double defaultValue)
{
  if (!hasReturnValue()) {
    return defaultValue;
  }
  return doubleReturnValue();
}

void*
pointerReturnValue()
{
  return cpputest::extensions::actualCall->returnPointerValue();
}

void*
returnPointerValueOrDefault(void* defaultValue)
{
  if (!hasReturnValue()) {
    return defaultValue;
  }
  return pointerReturnValue();
}

const void*
constPointerReturnValue()
{
  return cpputest::extensions::actualCall->returnConstPointerValue();
}

const void*
returnConstPointerValueOrDefault(const void* defaultValue)
{
  if (!hasReturnValue()) {
    return defaultValue;
  }
  return constPointerReturnValue();
}

void (*functionPointerReturnValue())()
{
  return cpputest::extensions::actualCall->returnFunctionPointerValue();
}

void (*returnFunctionPointerValueOrDefault(void (*defaultValue)()))()
{
  if (!hasReturnValue()) {
    return defaultValue;
  }
  return functionPointerReturnValue();
}

void
disable()
{
  cpputest::extensions::currentMockSupport->disable();
}

void
enable()
{
  cpputest::extensions::currentMockSupport->enable();
}

void
ignoreOtherCalls()
{
  cpputest::extensions::currentMockSupport->ignoreOtherCalls();
}

void
setBoolData(const char* name, int value)
{
  cpputest::extensions::currentMockSupport->setData(name, (value != 0));
}

void
setIntData(const char* name, int value)
{
  cpputest::extensions::currentMockSupport->setData(name, value);
}

void
setUnsignedIntData(const char* name, unsigned int value)
{
  cpputest::extensions::currentMockSupport->setData(name, value);
}

void
setLongIntData(const char* name, long int value)
{
  cpputest::extensions::currentMockSupport->setData(name, value);
}

void
setUnsignedLongIntData(const char* name, unsigned long int value)
{
  cpputest::extensions::currentMockSupport->setData(name, value);
}

void
setDoubleData(const char* name, double value)
{
  cpputest::extensions::currentMockSupport->setData(name, value);
}

void
setStringData(const char* name, const char* value)
{
  cpputest::extensions::currentMockSupport->setData(name, value);
}

void
setPointerData(const char* name, void* value)
{
  cpputest::extensions::currentMockSupport->setData(name, value);
}

void
setConstPointerData(const char* name, const void* value)
{
  cpputest::extensions::currentMockSupport->setData(name, value);
}

void
setFunctionPointerData(const char* name, void (*value)())
{
  cpputest::extensions::currentMockSupport->setData(name, value);
}

void
setDataObject(const char* name, const char* type, void* value)
{
  cpputest::extensions::currentMockSupport->setDataObject(name, type, value);
}

void
setDataConstObject(const char* name, const char* type, const void* value)
{
  cpputest::extensions::currentMockSupport->setDataConstObject(
      name, type, value);
}

struct MockValue
getData(const char* name)
{
  return getMockValueCFromNamedValue(
      cpputest::extensions::currentMockSupport->getData(name));
}

int
hasReturnValue()
{
  return cpputest::extensions::currentMockSupport->hasReturnValue();
}

void
checkExpectations()
{
  cpputest::extensions::currentMockSupport->checkExpectations();
}

int
expectedCallsLeft()
{
  return cpputest::extensions::currentMockSupport->expectedCallsLeft();
}

void
clear()
{
  cpputest::extensions::currentMockSupport->clear();
}

void
crashOnFailure(unsigned shouldCrash)
{
  cpputest::extensions::currentMockSupport->crashOnFailure(0 != shouldCrash);
}

static struct MockSupport gMockSupport = { strictOrder,
  expectOneCall,
  expectNoCall,
  expectNCalls,
  actualCall,
  hasReturnValue,
  returnValue,
  boolReturnValue,
  returnBoolValueOrDefault,
  intReturnValue,
  returnIntValueOrDefault,
  unsignedIntReturnValue,
  returnUnsignedIntValueOrDefault,
  longIntReturnValue,
  returnLongIntValueOrDefault,
  unsignedLongIntReturnValue,
  returnUnsignedLongIntValueOrDefault,
  longLongIntReturnValue,
  returnLongLongIntValueOrDefault,
  unsignedLongLongIntReturnValue,
  returnUnsignedLongLongIntValueOrDefault,
  stringReturnValue,
  returnStringValueOrDefault,
  doubleReturnValue,
  returnDoubleValueOrDefault,
  pointerReturnValue,
  returnPointerValueOrDefault,
  constPointerReturnValue,
  returnConstPointerValueOrDefault,
  functionPointerReturnValue,
  returnFunctionPointerValueOrDefault,
  setBoolData,
  setIntData,
  setUnsignedIntData,
  setLongIntData,
  setUnsignedLongIntData,
  setStringData,
  setDoubleData,
  setPointerData,
  setConstPointerData,
  setFunctionPointerData,
  setDataObject,
  setDataConstObject,
  getData,
  disable,
  enable,
  ignoreOtherCalls,
  checkExpectations,
  expectedCallsLeft,
  clear,
  crashOnFailure,
  installComparator,
  installCopier,
  removeAllComparatorsAndCopiers };
}

struct MockSupport*
mock()
{
  cpputest::extensions::currentMockSupport =
      &mock("", &cpputest::extensions::failureReporterForC);
  return &c::gMockSupport;
}

struct MockSupport*
mock_scope(const char* scope)
{
  cpputest::extensions::currentMockSupport =
      &mock(scope, &cpputest::extensions::failureReporterForC);
  return &c::gMockSupport;
}
