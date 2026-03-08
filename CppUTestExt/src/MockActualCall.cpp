/*
 * Copyright (c) 2007, Michael Feathers, James Grenning and Bas Vodde
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE EARLIER MENTIONED AUTHORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockCheckedActualCall.h"
#include "CppUTestExt/MockCheckedExpectedCall.h"
#include "CppUTestExt/MockFailure.h"
#include "CppUTest/PlatformSpecificFunctions.h"

namespace cpputest { namespace extensions {

MockActualCall::MockActualCall()
{
}

MockActualCall::~MockActualCall()
{
}

void MockCheckedActualCall::setName(const SimpleString& name)
{
    functionName_ = name;
}

SimpleString MockCheckedActualCall::getName() const
{
    return functionName_;
}

MockCheckedActualCall::MockCheckedActualCall(unsigned int callOrder, MockFailureReporter* reporter, const MockExpectedCallsList& allExpectations)
    : callOrder_(callOrder), reporter_(reporter), state_(CALL_SUCCEED), expectationsChecked_(false), matchingExpectation_(nullptr),
      allExpectations_(allExpectations), outputParameterExpectations_(nullptr)
{
    potentiallyMatchingExpectations_.addPotentiallyMatchingExpectations(allExpectations);
}

MockCheckedActualCall::~MockCheckedActualCall()
{
    cleanUpOutputParameterList();
}

void MockCheckedActualCall::setMockFailureReporter(MockFailureReporter* reporter)
{
    reporter_ = reporter;
}

cpputest::TestShell* MockCheckedActualCall::getTest() const
{
    return reporter_->getTestToFail();
}

void MockCheckedActualCall::failTest(const MockFailure& failure)
{
    if (!hasFailed()) {
        setState(CALL_FAILED);
        reporter_->failTest(failure);
    }
}

void MockCheckedActualCall::copyOutputParameters(MockCheckedExpectedCall* expectedCall)
{
    for (MockOutputParametersListNode* p = outputParameterExpectations_; p; p = p->next_)
    {
        MockNamedValue outputParameter = expectedCall->getOutputParameter(p->name_);
        MockNamedValueCopier* copier = outputParameter.getCopier();
        if (copier)
        {
            copier->copy(p->ptr_, outputParameter.getConstObjectPointer());
        }
        else if ((outputParameter.getType() == "const void*") && (p->type_ == "void*"))
        {
            const void* data = outputParameter.getConstPointerValue();
            size_t size = outputParameter.getSize();
            PlatformSpecificMemCpy(p->ptr_, data, size);
        }
        else if (outputParameter.getName() != "")
        {
            SimpleString type = expectedCall->getOutputParameter(p->name_).getType();
            failTestWith([&]() { return MockNoWayToCopyCustomTypeFailure(getTest(), type); });
        }
    }
}

void MockCheckedActualCall::completeCallWhenMatchIsFound()
{
    // Expectations that don't ignore parameters have higher fulfillment preference than those that ignore parameters

    matchingExpectation_ = potentiallyMatchingExpectations_.removeFirstFinalizedMatchingExpectation();
    if (matchingExpectation_) {
        copyOutputParameters(matchingExpectation_);
        callHasSucceeded();
    } else {
        MockCheckedExpectedCall* matchingExpectationWithIgnoredParameters = potentiallyMatchingExpectations_.getFirstMatchingExpectation();
        if (matchingExpectationWithIgnoredParameters) {
            copyOutputParameters(matchingExpectationWithIgnoredParameters);
        }
    }
}

void MockCheckedActualCall::callHasSucceeded()
{
    setState(CALL_SUCCEED);
}

void MockCheckedActualCall::discardCurrentlyMatchingExpectations()
{
    if (matchingExpectation_)
    {
        matchingExpectation_->resetActualCallMatchingState();
        matchingExpectation_ = nullptr;
    }
    potentiallyMatchingExpectations_.onlyKeepUnmatchingExpectations();
}

void MockCheckedActualCall::setNameAndCheck(SimpleString name)
{
    functionName_ = static_cast<SimpleString&&>(name);
    setState(CALL_IN_PROGRESS);

    potentiallyMatchingExpectations_.onlyKeepExpectationsRelatedTo(functionName_);
    if (potentiallyMatchingExpectations_.isEmpty()) {
        failTestWith([this]() { return MockUnexpectedCallHappenedFailure(getTest(), functionName_, allExpectations_); });
        return;
    }

    completeCallWhenMatchIsFound();
}

MockActualCall& MockCheckedActualCall::withName(const SimpleString& name)
{
    setNameAndCheck(name);
    return *this;
}

MockActualCall& MockCheckedActualCall::withCallOrder(unsigned int)
{
    return *this;
}

void MockCheckedActualCall::checkInputParameter(MockNamedValue actualParameter)
{
    if(hasFailed())
    {
        return;
    }

    setState(CALL_IN_PROGRESS);
    discardCurrentlyMatchingExpectations();

    potentiallyMatchingExpectations_.onlyKeepExpectationsWithInputParameter(actualParameter);

    if (potentiallyMatchingExpectations_.isEmpty()) {
        failTestWithCleanup(
            [&]() { return MockUnexpectedInputParameterFailure(getTest(), getName(), actualParameter, allExpectations_); },
            [&]() { MockNamedValue freed(static_cast<MockNamedValue&&>(actualParameter)); }
        );
        return;
    }

    potentiallyMatchingExpectations_.parameterWasPassed(actualParameter.getName());
    completeCallWhenMatchIsFound();
}

void MockCheckedActualCall::checkOutputParameter(MockNamedValue outputParameter)
{
    if(hasFailed())
    {
        return;
    }

    setState(CALL_IN_PROGRESS);
    discardCurrentlyMatchingExpectations();

    potentiallyMatchingExpectations_.onlyKeepExpectationsWithOutputParameter(outputParameter);

    if (potentiallyMatchingExpectations_.isEmpty()) {
        failTestWithCleanup(
            [&]() { return MockUnexpectedOutputParameterFailure(getTest(), getName(), outputParameter, allExpectations_); },
            [&]() { MockNamedValue freed(static_cast<MockNamedValue&&>(outputParameter)); }
        );
        return;
    }

    potentiallyMatchingExpectations_.outputParameterWasPassed(outputParameter.getName());
    completeCallWhenMatchIsFound();
}

MockActualCall& MockCheckedActualCall::withBoolParameter(const SimpleString& name, bool value)
{
    MockNamedValue actualParameter(name);
    actualParameter.setValue(value);
    checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
    return *this;
}

MockActualCall& MockCheckedActualCall::withUnsignedIntParameter(const SimpleString& name, unsigned int value)
{
    MockNamedValue actualParameter(name);
    actualParameter.setValue(value);
    checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
    return *this;
}

MockActualCall& MockCheckedActualCall::withIntParameter(const SimpleString& name, int value)
{
    MockNamedValue actualParameter(name);
    actualParameter.setValue(value);
    checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
    return *this;
}

MockActualCall& MockCheckedActualCall::withUnsignedLongIntParameter(const SimpleString& name, unsigned long int value)
{
    MockNamedValue actualParameter(name);
    actualParameter.setValue(value);
    checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
    return *this;
}

MockActualCall& MockCheckedActualCall::withLongIntParameter(const SimpleString& name, long int value)
{
    MockNamedValue actualParameter(name);
    actualParameter.setValue(value);
    checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
    return *this;
}

MockActualCall& MockCheckedActualCall::withUnsignedLongLongIntParameter(const SimpleString& name, unsigned long long value)
{
    MockNamedValue actualParameter(name);
    actualParameter.setValue(value);
    checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
    return *this;
}

MockActualCall& MockCheckedActualCall::withLongLongIntParameter(const SimpleString& name, long long value)
{
    MockNamedValue actualParameter(name);
    actualParameter.setValue(value);
    checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
    return *this;
}

MockActualCall& MockCheckedActualCall::withDoubleParameter(const SimpleString& name, double value)
{
    MockNamedValue actualParameter(name);
    actualParameter.setValue(value);
    checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
    return *this;
}

MockActualCall& MockCheckedActualCall::withStringParameter(const SimpleString& name, const char* value)
{
    MockNamedValue actualParameter(name);
    actualParameter.setValue(value);
    checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
    return *this;
}

MockActualCall& MockCheckedActualCall::withPointerParameter(const SimpleString& name, void* value)
{
    MockNamedValue actualParameter(name);
    actualParameter.setValue(value);
    checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
    return *this;
}

MockActualCall& MockCheckedActualCall::withConstPointerParameter(const SimpleString& name, const void* value)
{
    MockNamedValue actualParameter(name);
    actualParameter.setValue(value);
    checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
    return *this;
}

MockActualCall& MockCheckedActualCall::withFunctionPointerParameter(const SimpleString& name, void (*value)())
{
    MockNamedValue actualParameter(name);
    actualParameter.setValue(value);
    checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
    return *this;
}

MockActualCall& MockCheckedActualCall::withMemoryBufferParameter(const SimpleString& name, const unsigned char* value, size_t size)
{
    MockNamedValue actualParameter(name);
    actualParameter.setMemoryBuffer(value, size);
    checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
    return *this;
}

MockActualCall& MockCheckedActualCall::withParameterOfType(const SimpleString& type, const SimpleString& name, const void* value)
{
    MockNamedValue actualParameter(name);
    actualParameter.setConstObjectPointer(type, value);

    if (actualParameter.getComparator() == nullptr) {
        failTestWith([&]() { return MockNoWayToCompareCustomTypeFailure(getTest(), type); });
        return *this;
    }
    checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
    return *this;
}

MockActualCall& MockCheckedActualCall::withOutputParameter(const SimpleString& name, void* output)
{
    addOutputParameter(name, "void*", output);

    MockNamedValue outputParameter(name);
    outputParameter.setValue(output);
    checkOutputParameter(static_cast<MockNamedValue&&>(outputParameter));

    return *this;
}

MockActualCall& MockCheckedActualCall::withOutputParameterOfType(const SimpleString& type, const SimpleString& name, void* output)
{
    addOutputParameter(name, type, output);

    MockNamedValue outputParameter(name);
    outputParameter.setConstObjectPointer(type, output);
    checkOutputParameter(static_cast<MockNamedValue&&>(outputParameter));

    return *this;
}

MockActualCall& MockCheckedActualCall::withMemoryBufferParameter(const char* name, const unsigned char* value, size_t size)
{
    MockNamedValue actualParameter(name);
    actualParameter.setMemoryBuffer(value, size);
    checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
    return *this;
}

MockActualCall& MockCheckedActualCall::withParameterOfType(const char* typeName, const char* name, const void* value)
{
    MockNamedValue actualParameter(name);
    actualParameter.setConstObjectPointer(typeName, value);

    if (actualParameter.getComparator() == nullptr) {
        failTestWithCleanup(
            [&]() { return MockNoWayToCompareCustomTypeFailure(getTest(), actualParameter.getType()); },
            [&]() { MockNamedValue freed(static_cast<MockNamedValue&&>(actualParameter)); }
        );
        return *this;
    }
    checkInputParameter(static_cast<MockNamedValue&&>(actualParameter));
    return *this;
}

bool MockCheckedActualCall::isFulfilled() const
{
    return state_ == CALL_SUCCEED;
}

bool MockCheckedActualCall::hasFailed() const
{
    return state_ == CALL_FAILED;
}

void MockCheckedActualCall::checkExpectations()
{
    if(expectationsChecked_) {
        return;
    }

    expectationsChecked_ = true;

    if (state_ != CALL_IN_PROGRESS) {
        if(state_ == CALL_SUCCEED) {
            matchingExpectation_->callWasMade(callOrder_);
        }
        potentiallyMatchingExpectations_.resetActualCallMatchingState();
        return;
    }

    if (potentiallyMatchingExpectations_.hasFinalizedMatchingExpectations())
        FAIL("Actual call is in progress, but there are finalized matching expectations when checking expectations. This cannot happen.");

    matchingExpectation_ = potentiallyMatchingExpectations_.removeFirstMatchingExpectation();
    if (matchingExpectation_) {
        matchingExpectation_->finalizeActualCallMatch();
        callHasSucceeded();
        matchingExpectation_->callWasMade(callOrder_);
        potentiallyMatchingExpectations_.resetActualCallMatchingState();
        return;
    }

    if (potentiallyMatchingExpectations_.hasUnmatchingExpectationsBecauseOfMissingParameters()) {
        failTestWith([&]() { return MockExpectedParameterDidntHappenFailure(getTest(), getName(), allExpectations_, potentiallyMatchingExpectations_); });
    }
    else {
        failTestWith([&]() { return MockExpectedObjectDidntHappenFailure(getTest(), getName(), allExpectations_); });
    }
}

void MockCheckedActualCall::setState(ActualCallState state)
{
    state_ = state;
}

MockNamedValue MockCheckedActualCall::returnValue()
{
    checkExpectations();
    if (matchingExpectation_)
        return matchingExpectation_->returnValue();
    return MockNamedValue("no return value");
}

bool MockCheckedActualCall::returnBoolValueOrDefault(bool default_value)
{
    if (!hasReturnValue()) {
        return default_value;
    }
    return returnBoolValue();
}

bool MockCheckedActualCall::returnBoolValue()
{
    return returnValue().getBoolValue();
}

int MockCheckedActualCall::returnIntValueOrDefault(int default_value)
{
    if (!hasReturnValue()) {
        return default_value;
    }
    return returnIntValue();
}

int MockCheckedActualCall::returnIntValue()
{
    return returnValue().getIntValue();
}

unsigned long int MockCheckedActualCall::returnUnsignedLongIntValue()
{
    return returnValue().getUnsignedLongIntValue();
}

unsigned long int MockCheckedActualCall::returnUnsignedLongIntValueOrDefault(unsigned long int default_value)
{
    if (!hasReturnValue()) {
        return default_value;
    }
    return returnUnsignedLongIntValue();
}

long int MockCheckedActualCall::returnLongIntValue()
{
    return returnValue().getLongIntValue();
}

long int MockCheckedActualCall::returnLongIntValueOrDefault(long int default_value)
{
    if (!hasReturnValue()) {
        return default_value;
    }
    return returnLongIntValue();
}

unsigned long long MockCheckedActualCall::returnUnsignedLongLongIntValue()
{
    return returnValue().getUnsignedLongLongIntValue();
}

unsigned long long MockCheckedActualCall::returnUnsignedLongLongIntValueOrDefault(unsigned long long default_value)
{
    if (!hasReturnValue()) {
        return default_value;
    }
    return returnUnsignedLongLongIntValue();
}

long long MockCheckedActualCall::returnLongLongIntValue()
{
    return returnValue().getLongLongIntValue();
}

long long MockCheckedActualCall::returnLongLongIntValueOrDefault(long long default_value)
{
    if (!hasReturnValue()) {
        return default_value;
    }
    return returnLongLongIntValue();
}

double MockCheckedActualCall::returnDoubleValue()
{
    return returnValue().getDoubleValue();
}

double MockCheckedActualCall::returnDoubleValueOrDefault(double default_value)
{
    if (!hasReturnValue()) {
        return default_value;
    }
    return returnDoubleValue();
}

unsigned int MockCheckedActualCall::returnUnsignedIntValue()
{
    return returnValue().getUnsignedIntValue();
}

unsigned int MockCheckedActualCall::returnUnsignedIntValueOrDefault(unsigned int default_value)
{
    if (!hasReturnValue()) {
        return default_value;
    }
    return returnUnsignedIntValue();
}

void * MockCheckedActualCall::returnPointerValueOrDefault(void * default_value)
{
    if (!hasReturnValue()) {
        return default_value;
    }
    return returnPointerValue();
}

void * MockCheckedActualCall::returnPointerValue()
{
    return returnValue().getPointerValue();
}

const void * MockCheckedActualCall::returnConstPointerValue()
{
    return returnValue().getConstPointerValue();
}

const void * MockCheckedActualCall::returnConstPointerValueOrDefault(const void * default_value)
{
    if (!hasReturnValue()) {
        return default_value;
    }
    return returnConstPointerValue();
}

void (*MockCheckedActualCall::returnFunctionPointerValue())()
{
    return returnValue().getFunctionPointerValue();
}

void (*MockCheckedActualCall::returnFunctionPointerValueOrDefault(void (*default_value)()))()
{
    if (!hasReturnValue()) {
        return default_value;
    }
    return returnFunctionPointerValue();
}

const char * MockCheckedActualCall::returnStringValueOrDefault(const char * default_value)
{
    if (!hasReturnValue()) {
        return default_value;
    }
    return returnStringValue();
}

const char * MockCheckedActualCall::returnStringValue()
{
    return returnValue().getStringValue();
}

bool MockCheckedActualCall::hasReturnValue()
{
    return ! returnValue().getName().isEmpty();
}

MockActualCall& MockCheckedActualCall::onObject(const void* objectPtr)
{
    if(hasFailed()) {
        return *this;
    }

    // Currently matching expectations are not discarded because the passed object
    // is ignored if not specifically set in the expectation

    potentiallyMatchingExpectations_.onlyKeepExpectationsOnObject(objectPtr);

    if ((!matchingExpectation_) && potentiallyMatchingExpectations_.isEmpty()) {
        failTestWith([&]() { return MockUnexpectedObjectFailure(getTest(), getName(), objectPtr, allExpectations_); });
        return *this;
    }

    potentiallyMatchingExpectations_.wasPassedToObject();

    if (!matchingExpectation_) {
        completeCallWhenMatchIsFound();
    }

    return *this;
}

void MockCheckedActualCall::addOutputParameter(const SimpleString& name, const SimpleString& type, void* ptr)
{
    MockOutputParametersListNode* newNode = new MockOutputParametersListNode(name, type, ptr);

    if (outputParameterExpectations_ == nullptr)
        outputParameterExpectations_ = newNode;
    else {
        MockOutputParametersListNode* lastNode = outputParameterExpectations_;
        while (lastNode->next_) lastNode = lastNode->next_;
        lastNode->next_ = newNode;
    }
}

void MockCheckedActualCall::cleanUpOutputParameterList()
{
    MockOutputParametersListNode* current = outputParameterExpectations_;
    MockOutputParametersListNode* toBeDeleted = nullptr;

    while (current) {
        toBeDeleted = current;
        outputParameterExpectations_ = current = current->next_;
        delete toBeDeleted;
    }
}


MockActualCallTrace::MockActualCallTrace()
{
}

MockActualCallTrace::~MockActualCallTrace()
{
}

MockActualCall& MockActualCallTrace::withName(const SimpleString& name)
{
    traceBuffer_ += "\nFunction name:";
    traceBuffer_ += name;
    return *this;
}

MockActualCall& MockActualCallTrace::withCallOrder(unsigned int callOrder)
{
    traceBuffer_ += " withCallOrder:";
    traceBuffer_ += cpputest::StringFrom(callOrder);
    return *this;
}

void MockActualCallTrace::addParameterName(const SimpleString& name)
{
    traceBuffer_ += " ";
    traceBuffer_ += name;
    traceBuffer_ += ":";
}

MockActualCall& MockActualCallTrace::withBoolParameter(const SimpleString& name, bool value)
{
    addParameterName(name);
    traceBuffer_ += cpputest::StringFrom(value);
    return *this;
}

MockActualCall& MockActualCallTrace::withUnsignedIntParameter(const SimpleString& name, unsigned int value)
{
    addParameterName(name);
    traceBuffer_ += cpputest::StringFrom(value) + " " + BracketsFormattedHexStringFrom(value);
    return *this;
}

MockActualCall& MockActualCallTrace::withIntParameter(const SimpleString& name, int value)
{
    addParameterName(name);
    traceBuffer_ += cpputest::StringFrom(value) + " " + BracketsFormattedHexStringFrom(value);
    return *this;
}

MockActualCall& MockActualCallTrace::withUnsignedLongIntParameter(const SimpleString& name, unsigned long int value)
{
    addParameterName(name);
    traceBuffer_ += cpputest::StringFrom(value) + " " + BracketsFormattedHexStringFrom(value);
    return *this;
}

MockActualCall& MockActualCallTrace::withLongIntParameter(const SimpleString& name, long int value)
{
    addParameterName(name);
    traceBuffer_ += cpputest::StringFrom(value) + " " + BracketsFormattedHexStringFrom(value);
    return *this;
}

MockActualCall& MockActualCallTrace::withUnsignedLongLongIntParameter(const SimpleString& name, unsigned long long value)
{
    addParameterName(name);
    traceBuffer_ += cpputest::StringFrom(value) + " " + BracketsFormattedHexStringFrom(value);
    return *this;
}

MockActualCall& MockActualCallTrace::withLongLongIntParameter(const SimpleString& name, long long value)
{
    addParameterName(name);
    traceBuffer_ += cpputest::StringFrom(value) + " " + BracketsFormattedHexStringFrom(value);
    return *this;
}

MockActualCall& MockActualCallTrace::withDoubleParameter(const SimpleString& name, double value)
{
    addParameterName(name);
    traceBuffer_ += cpputest::StringFrom(value);
    return *this;
}

MockActualCall& MockActualCallTrace::withStringParameter(const SimpleString& name, const char* value)
{
    addParameterName(name);
    traceBuffer_ += cpputest::StringFrom(value);
    return *this;
}

MockActualCall& MockActualCallTrace::withPointerParameter(const SimpleString& name, void* value)
{
    addParameterName(name);
    traceBuffer_ += cpputest::StringFrom(value);
    return *this;
}

MockActualCall& MockActualCallTrace::withConstPointerParameter(const SimpleString& name, const void* value)
{
    addParameterName(name);
    traceBuffer_ += cpputest::StringFrom(value);
    return *this;
}

MockActualCall& MockActualCallTrace::withFunctionPointerParameter(const SimpleString& name, void (*value)())
{
    addParameterName(name);
    traceBuffer_ += cpputest::StringFrom(value);
    return *this;
}

MockActualCall& MockActualCallTrace::withMemoryBufferParameter(const SimpleString& name, const unsigned char* value, size_t size)
{
    addParameterName(name);
    traceBuffer_ += StringFromBinaryWithSizeOrNull(value, size);
    return *this;
}

MockActualCall& MockActualCallTrace::withParameterOfType(const SimpleString& typeName, const SimpleString& name, const void* value)
{
    traceBuffer_ += " ";
    traceBuffer_ += typeName;
    addParameterName(name);
    traceBuffer_ += cpputest::StringFrom(value);
    return *this;
}

MockActualCall& MockActualCallTrace::withMemoryBufferParameter(const char* name, const unsigned char* value, size_t size)
{
    return withMemoryBufferParameter(SimpleString(name), value, size);
}

MockActualCall& MockActualCallTrace::withParameterOfType(const char* typeName, const char* name, const void* value)
{
    return withParameterOfType(SimpleString(typeName), SimpleString(name), value);
}

MockActualCall& MockActualCallTrace::withOutputParameter(const SimpleString& name, void* output)
{
    addParameterName(name);
    traceBuffer_ += cpputest::StringFrom(output);
    return *this;
}

MockActualCall& MockActualCallTrace::withOutputParameterOfType(const SimpleString& typeName, const SimpleString& name, void* output)
{
    traceBuffer_ += " ";
    traceBuffer_ += typeName;
    addParameterName(name);
    traceBuffer_ += cpputest::StringFrom(output);
    return *this;
}

bool MockActualCallTrace::hasReturnValue()
{
    return false;
}

MockNamedValue MockActualCallTrace::returnValue()
{
    return MockNamedValue("");
}

long int MockActualCallTrace::returnLongIntValue()
{
    return 0;
}

unsigned long int MockActualCallTrace::returnUnsignedLongIntValue()
{
    return 0;
}

unsigned long int MockActualCallTrace::returnUnsignedLongIntValueOrDefault(unsigned long)
{
    return 0;
}

long int MockActualCallTrace::returnLongIntValueOrDefault(long int)
{
    return 0;
}

long long MockActualCallTrace::returnLongLongIntValue()
{
    return 0;
}

unsigned long long MockActualCallTrace::returnUnsignedLongLongIntValue()
{
    return 0;
}

unsigned long long MockActualCallTrace::returnUnsignedLongLongIntValueOrDefault(unsigned long long)
{
    return 0;
}

long long MockActualCallTrace::returnLongLongIntValueOrDefault(long long)
{
    return 0;
}

bool MockActualCallTrace::returnBoolValue()
{
    return false;
}

bool MockActualCallTrace::returnBoolValueOrDefault(bool)
{
    return false;
}

int MockActualCallTrace::returnIntValue()
{
    return 0;
}

double MockActualCallTrace::returnDoubleValue()
{
    return 0.0;
}

double MockActualCallTrace::returnDoubleValueOrDefault(double)
{
    return returnDoubleValue();
}

unsigned int MockActualCallTrace::returnUnsignedIntValue()
{
    return 0;
}

void * MockActualCallTrace::returnPointerValue()
{
    return nullptr;
}

const void * MockActualCallTrace::returnConstPointerValue()
{
    return nullptr;
}

void (*MockActualCallTrace::returnFunctionPointerValue())()
{
    return nullptr;
}

const void * MockActualCallTrace::returnConstPointerValueOrDefault(const void *)
{
    return returnConstPointerValue();
}

void * MockActualCallTrace::returnPointerValueOrDefault(void *)
{
    return returnPointerValue();
}

void (*MockActualCallTrace::returnFunctionPointerValueOrDefault(void (*)()))()
{
    return returnFunctionPointerValue();
}

const char * MockActualCallTrace::returnStringValue()
{
    return "";
}

const char * MockActualCallTrace::returnStringValueOrDefault(const char *)
{
    return returnStringValue();
}

int MockActualCallTrace::returnIntValueOrDefault(int)
{
    return 0;
}

unsigned int MockActualCallTrace::returnUnsignedIntValueOrDefault(unsigned int)
{
    return returnUnsignedIntValue();
}

MockActualCall& MockActualCallTrace::onObject(const void* objectPtr)
{
    traceBuffer_ += " onObject:";
    traceBuffer_ += cpputest::StringFrom(objectPtr);
    return *this;
}

void MockActualCallTrace::clear()
{
    traceBuffer_ = "";
}

const char* MockActualCallTrace::getTraceOutput()
{
    return traceBuffer_.asCharString();
}

MockActualCallTrace* MockActualCallTrace::instance_ = nullptr;

MockActualCallTrace& MockActualCallTrace::instance()
{
    if (instance_ == nullptr)
        instance_ = new MockActualCallTrace;
    return *instance_;
}

void MockActualCallTrace::clearInstance()
{
    delete instance_;
    instance_ = nullptr;
}

MockIgnoredActualCall& MockIgnoredActualCall::instance()
{
    static MockIgnoredActualCall call;
    return call;
}

} // namespace extensions
} // namespace cpputest
