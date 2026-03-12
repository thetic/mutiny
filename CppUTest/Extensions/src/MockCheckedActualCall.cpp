#include "CppUTest/Extensions/MockCheckedActualCall.hpp"

#include "CppUTest/Extensions/MockCheckedExpectedCall.hpp"
#include "CppUTest/Extensions/MockFailure.hpp"

#include "CppUTest/PlatformSpecificFunctions.hpp"
#include "CppUTest/TestHarness.hpp"

namespace cpputest {
namespace extensions {

void
MockCheckedActualCall::setName(const String& name)
{
  functionName_ = name;
}

const String&
MockCheckedActualCall::getName() const
{
  return functionName_;
}

MockCheckedActualCall::MockCheckedActualCall(unsigned int callOrder,
    MockFailureReporter* reporter,
    const MockExpectedCallsList& allExpectations)
  : callOrder_(callOrder)
  , reporter_(reporter)
  , state_(CALL_SUCCEED)
  , expectationsChecked_(false)
  , matchingExpectation_(nullptr)
  , allExpectations_(allExpectations)
  , outputParameterExpectations_(nullptr)
{
  potentiallyMatchingExpectations_.addPotentiallyMatchingExpectations(
      allExpectations);
}

MockCheckedActualCall::~MockCheckedActualCall()
{
  cleanUpOutputParameterList();
}

void
MockCheckedActualCall::setMockFailureReporter(MockFailureReporter* reporter)
{
  reporter_ = reporter;
}

cpputest::TestShell*
MockCheckedActualCall::getTest() const
{
  return reporter_->getTestToFail();
}

void
MockCheckedActualCall::failWith(MockFailure failure)
{
  if (!hasFailed()) {
    setState(CALL_FAILED);
    reporter_->failTest(static_cast<MockFailure&&>(failure));
  }
}

void
MockCheckedActualCall::copyOutputParameters(
    MockCheckedExpectedCall* expectedCall)
{
  for (MockOutputParametersListNode* p = outputParameterExpectations_; p;
      p = p->next_) {
    MockNamedValue outputParameter = expectedCall->getOutputParameter(p->name_);
    MockNamedValueCopier* copier = outputParameter.getCopier();
    if (copier) {
      copier->copy(p->ptr_, outputParameter.getConstObjectPointer());
    } else if ((outputParameter.getType() == "const void*") &&
               (p->type_ == "void*")) {
      const void* data = outputParameter.getConstPointerValue();
      size_t size = outputParameter.getSize();
      PlatformSpecificMemCpy(p->ptr_, data, size);
    } else if (outputParameter.getName() != "") {
      failWith(MockNoWayToCopyCustomTypeFailure(
          getTest(), expectedCall->getOutputParameter(p->name_).getType()));
    }
  }
}

void
MockCheckedActualCall::completeCallWhenMatchIsFound()
{
  // Expectations that don't ignore parameters have higher fulfillment
  // preference than those that ignore parameters

  matchingExpectation_ = potentiallyMatchingExpectations_
                             .removeFirstFinalizedMatchingExpectation();
  if (matchingExpectation_) {
    copyOutputParameters(matchingExpectation_);
    callHasSucceeded();
  } else {
    MockCheckedExpectedCall* matchingExpectationWithIgnoredParameters =
        potentiallyMatchingExpectations_.getFirstMatchingExpectation();
    if (matchingExpectationWithIgnoredParameters) {
      copyOutputParameters(matchingExpectationWithIgnoredParameters);
    }
  }
}

void
MockCheckedActualCall::callHasSucceeded()
{
  setState(CALL_SUCCEED);
}

void
MockCheckedActualCall::discardCurrentlyMatchingExpectations()
{
  if (matchingExpectation_) {
    matchingExpectation_->resetActualCallMatchingState();
    matchingExpectation_ = nullptr;
  }
  potentiallyMatchingExpectations_.onlyKeepUnmatchingExpectations();
}

void
MockCheckedActualCall::setNameAndCheck(String name)
{
  functionName_ = static_cast<String&&>(name);
  setState(CALL_IN_PROGRESS);

  potentiallyMatchingExpectations_.onlyKeepExpectationsRelatedTo(functionName_);
  if (potentiallyMatchingExpectations_.empty()) {
    failWith(MockUnexpectedCallHappenedFailure(
        getTest(), functionName_, allExpectations_));
    return;
  }

  completeCallWhenMatchIsFound();
}

MockActualCall&
MockCheckedActualCall::withName(const String& name)
{
  setNameAndCheck(name);
  return *this;
}

MockActualCall&
MockCheckedActualCall::withCallOrder(unsigned int)
{
  return *this;
}

void
MockCheckedActualCall::checkInputParameter(MockNamedValue actualParameter)
{
  if (hasFailed()) {
    return;
  }

  setState(CALL_IN_PROGRESS);
  discardCurrentlyMatchingExpectations();

  potentiallyMatchingExpectations_.onlyKeepExpectationsWithInputParameter(
      actualParameter);

  if (potentiallyMatchingExpectations_.empty()) {
    MockUnexpectedInputParameterFailure failure(getTest(),
        getName(),
        static_cast<MockNamedValue&&>(actualParameter),
        allExpectations_);
    failWith(static_cast<MockFailure&&>(failure));
    return;
  }

  potentiallyMatchingExpectations_.parameterWasPassed(
      actualParameter.getName());
  completeCallWhenMatchIsFound();
}

void
MockCheckedActualCall::checkOutputParameter(MockNamedValue outputParameter)
{
  if (hasFailed()) {
    return;
  }

  setState(CALL_IN_PROGRESS);
  discardCurrentlyMatchingExpectations();

  potentiallyMatchingExpectations_.onlyKeepExpectationsWithOutputParameter(
      outputParameter);

  if (potentiallyMatchingExpectations_.empty()) {
    MockUnexpectedOutputParameterFailure failure(getTest(),
        getName(),
        static_cast<MockNamedValue&&>(outputParameter),
        allExpectations_);
    failWith(static_cast<MockFailure&&>(failure));
    return;
  }

  potentiallyMatchingExpectations_.outputParameterWasPassed(
      outputParameter.getName());
  completeCallWhenMatchIsFound();
}

MockActualCall&
MockCheckedActualCall::withBoolParameter(const String& name, bool value)
{
  MockNamedValue actualParameter(name);
  actualParameter.setValue(value);
  checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::withUnsignedIntParameter(const String& name,
    unsigned int value)
{
  MockNamedValue actualParameter(name);
  actualParameter.setValue(value);
  checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::withIntParameter(const String& name, int value)
{
  MockNamedValue actualParameter(name);
  actualParameter.setValue(value);
  checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::withUnsignedLongIntParameter(const String& name,
    unsigned long int value)
{
  MockNamedValue actualParameter(name);
  actualParameter.setValue(value);
  checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::withLongIntParameter(const String& name, long int value)
{
  MockNamedValue actualParameter(name);
  actualParameter.setValue(value);
  checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::withUnsignedLongLongIntParameter(const String& name,
    unsigned long long value)
{
  MockNamedValue actualParameter(name);
  actualParameter.setValue(value);
  checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::withLongLongIntParameter(const String& name,
    long long value)
{
  MockNamedValue actualParameter(name);
  actualParameter.setValue(value);
  checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::withDoubleParameter(const String& name, double value)
{
  MockNamedValue actualParameter(name);
  actualParameter.setValue(value);
  checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::withStringParameter(const String& name,
    const char* value)
{
  MockNamedValue actualParameter(name);
  actualParameter.setValue(value);
  checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::withPointerParameter(const String& name, void* value)
{
  MockNamedValue actualParameter(name);
  actualParameter.setValue(value);
  checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::withConstPointerParameter(const String& name,
    const void* value)
{
  MockNamedValue actualParameter(name);
  actualParameter.setValue(value);
  checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::withFunctionPointerParameter(const String& name,
    void (*value)())
{
  MockNamedValue actualParameter(name);
  actualParameter.setValue(value);
  checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::withMemoryBufferParameter(const String& name,
    const unsigned char* value,
    size_t size)
{
  MockNamedValue actualParameter(name);
  actualParameter.setMemoryBuffer(value, size);
  checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::withParameterOfType(const String& type,
    const String& name,
    const void* value)
{
  MockNamedValue actualParameter(name);
  actualParameter.setConstObjectPointer(type, value);

  if (actualParameter.getComparator() == nullptr) {
    {
      MockNamedValue freed(static_cast<MockNamedValue&&>(actualParameter));
    }
    failWith(MockNoWayToCompareCustomTypeFailure(getTest(), type));
    return *this;
  }
  checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::withOutputParameter(const String& name, void* output)
{
  addOutputParameter(name, "void*", output);

  MockNamedValue outputParameter(name);
  outputParameter.setValue(output);
  checkOutputParameter(static_cast<MockNamedValue&&>(outputParameter));

  return *this;
}

MockActualCall&
MockCheckedActualCall::withOutputParameterOfType(const String& type,
    const String& name,
    void* output)
{
  addOutputParameter(name, type, output);

  MockNamedValue outputParameter(name);
  outputParameter.setConstObjectPointer(type, output);
  checkOutputParameter(static_cast<MockNamedValue&&>(outputParameter));

  return *this;
}

MockActualCall&
MockCheckedActualCall::withMemoryBufferParameter(const char* name,
    const unsigned char* value,
    size_t size)
{
  MockNamedValue actualParameter(name);
  actualParameter.setMemoryBuffer(value, size);
  checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
  return *this;
}

MockActualCall&
MockCheckedActualCall::withParameterOfType(const char* typeName,
    const char* name,
    const void* value)
{
  MockNamedValue actualParameter(name);
  actualParameter.setConstObjectPointer(typeName, value);

  if (actualParameter.getComparator() == nullptr) {
    String type = actualParameter.getType();
    {
      MockNamedValue freed(static_cast<MockNamedValue&&>(actualParameter));
    }
    failWith(MockNoWayToCompareCustomTypeFailure(
        getTest(), static_cast<String&&>(type)));
    return *this;
  }
  checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
  return *this;
}

bool
MockCheckedActualCall::isFulfilled() const
{
  return state_ == CALL_SUCCEED;
}

bool
MockCheckedActualCall::hasFailed() const
{
  return state_ == CALL_FAILED;
}

void
MockCheckedActualCall::checkExpectations()
{
  if (expectationsChecked_) {
    return;
  }

  expectationsChecked_ = true;

  if (state_ != CALL_IN_PROGRESS) {
    if (state_ == CALL_SUCCEED) {
      matchingExpectation_->callWasMade(callOrder_);
    }
    potentiallyMatchingExpectations_.resetActualCallMatchingState();
    return;
  }

  if (potentiallyMatchingExpectations_.hasFinalizedMatchingExpectations())
    FAIL("Actual call is in progress, but there are finalized matching "
         "expectations when checking expectations. This cannot happen.");

  matchingExpectation_ =
      potentiallyMatchingExpectations_.removeFirstMatchingExpectation();
  if (matchingExpectation_) {
    matchingExpectation_->finalizeActualCallMatch();
    callHasSucceeded();
    matchingExpectation_->callWasMade(callOrder_);
    potentiallyMatchingExpectations_.resetActualCallMatchingState();
    return;
  }

  if (potentiallyMatchingExpectations_
          .hasUnmatchingExpectationsBecauseOfMissingParameters()) {
    failWith(MockExpectedParameterDidntHappenFailure(getTest(),
        getName(),
        allExpectations_,
        potentiallyMatchingExpectations_));
  } else {
    failWith(MockExpectedObjectDidntHappenFailure(
        getTest(), getName(), allExpectations_));
  }
}

void
MockCheckedActualCall::setState(ActualCallState state)
{
  state_ = state;
}

MockNamedValue
MockCheckedActualCall::returnValue()
{
  checkExpectations();
  if (matchingExpectation_)
    return matchingExpectation_->returnValue();
  return MockNamedValue("no return value");
}

bool
MockCheckedActualCall::returnBoolValueOrDefault(bool default_value)
{
  if (!hasReturnValue()) {
    return default_value;
  }
  return returnBoolValue();
}

bool
MockCheckedActualCall::returnBoolValue()
{
  return returnValue().getBoolValue();
}

int
MockCheckedActualCall::returnIntValueOrDefault(int default_value)
{
  if (!hasReturnValue()) {
    return default_value;
  }
  return returnIntValue();
}

int
MockCheckedActualCall::returnIntValue()
{
  return returnValue().getIntValue();
}

unsigned long int
MockCheckedActualCall::returnUnsignedLongIntValue()
{
  return returnValue().getUnsignedLongIntValue();
}

unsigned long int
MockCheckedActualCall::returnUnsignedLongIntValueOrDefault(
    unsigned long int default_value)
{
  if (!hasReturnValue()) {
    return default_value;
  }
  return returnUnsignedLongIntValue();
}

long int
MockCheckedActualCall::returnLongIntValue()
{
  return returnValue().getLongIntValue();
}

long int
MockCheckedActualCall::returnLongIntValueOrDefault(long int default_value)
{
  if (!hasReturnValue()) {
    return default_value;
  }
  return returnLongIntValue();
}

unsigned long long
MockCheckedActualCall::returnUnsignedLongLongIntValue()
{
  return returnValue().getUnsignedLongLongIntValue();
}

unsigned long long
MockCheckedActualCall::returnUnsignedLongLongIntValueOrDefault(
    unsigned long long default_value)
{
  if (!hasReturnValue()) {
    return default_value;
  }
  return returnUnsignedLongLongIntValue();
}

long long
MockCheckedActualCall::returnLongLongIntValue()
{
  return returnValue().getLongLongIntValue();
}

long long
MockCheckedActualCall::returnLongLongIntValueOrDefault(long long default_value)
{
  if (!hasReturnValue()) {
    return default_value;
  }
  return returnLongLongIntValue();
}

double
MockCheckedActualCall::returnDoubleValue()
{
  return returnValue().getDoubleValue();
}

double
MockCheckedActualCall::returnDoubleValueOrDefault(double default_value)
{
  if (!hasReturnValue()) {
    return default_value;
  }
  return returnDoubleValue();
}

unsigned int
MockCheckedActualCall::returnUnsignedIntValue()
{
  return returnValue().getUnsignedIntValue();
}

unsigned int
MockCheckedActualCall::returnUnsignedIntValueOrDefault(
    unsigned int default_value)
{
  if (!hasReturnValue()) {
    return default_value;
  }
  return returnUnsignedIntValue();
}

void*
MockCheckedActualCall::returnPointerValueOrDefault(void* default_value)
{
  if (!hasReturnValue()) {
    return default_value;
  }
  return returnPointerValue();
}

void*
MockCheckedActualCall::returnPointerValue()
{
  return returnValue().getPointerValue();
}

const void*
MockCheckedActualCall::returnConstPointerValue()
{
  return returnValue().getConstPointerValue();
}

const void*
MockCheckedActualCall::returnConstPointerValueOrDefault(
    const void* default_value)
{
  if (!hasReturnValue()) {
    return default_value;
  }
  return returnConstPointerValue();
}

MockActualCall::FunctionPointerReturnValue
MockCheckedActualCall::returnFunctionPointerValue()
{
  return returnValue().getFunctionPointerValue();
}

MockActualCall::FunctionPointerReturnValue
MockCheckedActualCall::returnFunctionPointerValueOrDefault(
    void (*default_value)())
{
  if (!hasReturnValue()) {
    return default_value;
  }
  return returnFunctionPointerValue();
}

const char*
MockCheckedActualCall::returnStringValueOrDefault(const char* default_value)
{
  if (!hasReturnValue()) {
    return default_value;
  }
  return returnStringValue();
}

const char*
MockCheckedActualCall::returnStringValue()
{
  return returnValue().getStringValue();
}

bool
MockCheckedActualCall::hasReturnValue()
{
  return !returnValue().getName().empty();
}

MockActualCall&
MockCheckedActualCall::onObject(const void* objectPtr)
{
  if (hasFailed()) {
    return *this;
  }

  // Currently matching expectations are not discarded because the passed object
  // is ignored if not specifically set in the expectation

  potentiallyMatchingExpectations_.onlyKeepExpectationsOnObject(objectPtr);

  if ((!matchingExpectation_) && potentiallyMatchingExpectations_.empty()) {
    failWith(MockUnexpectedObjectFailure(
        getTest(), getName(), objectPtr, allExpectations_));
    return *this;
  }

  potentiallyMatchingExpectations_.wasPassedToObject();

  if (!matchingExpectation_) {
    completeCallWhenMatchIsFound();
  }

  return *this;
}

void
MockCheckedActualCall::addOutputParameter(const String& name,
    const String& type,
    void* ptr)
{
  MockOutputParametersListNode* newNode =
      new MockOutputParametersListNode(name, type, ptr);

  if (outputParameterExpectations_ == nullptr)
    outputParameterExpectations_ = newNode;
  else {
    MockOutputParametersListNode* lastNode = outputParameterExpectations_;
    while (lastNode->next_)
      lastNode = lastNode->next_;
    lastNode->next_ = newNode;
  }
}

void
MockCheckedActualCall::cleanUpOutputParameterList()
{
  MockOutputParametersListNode* current = outputParameterExpectations_;
  MockOutputParametersListNode* toBeDeleted = nullptr;

  while (current) {
    toBeDeleted = current;
    outputParameterExpectations_ = current = current->next_;
    delete toBeDeleted;
  }
}

} // namespace extensions
} // namespace cpputest
