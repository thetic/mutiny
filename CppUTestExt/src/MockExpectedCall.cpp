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

#include "CppUTestExt/MockCheckedExpectedCall.h"

#include "CppUTest/TestHarness.h"

namespace cpputest {
namespace extensions {

MockExpectedCall::MockExpectedCall() {}

MockExpectedCall::~MockExpectedCall() {}

String
StringFrom(const MockNamedValue& parameter)
{
  return parameter.toString();
}

void
MockCheckedExpectedCall::setName(const String& name)
{
  functionName_ = name;
}

String
MockCheckedExpectedCall::getName() const
{
  return functionName_;
}

MockCheckedExpectedCall::MockCheckedExpectedCall()
  : ignoreOtherParameters_(false)
  , isActualCallMatchFinalized_(false)
  , initialExpectedCallOrder_(NO_EXPECTED_CALL_ORDER)
  , finalExpectedCallOrder_(NO_EXPECTED_CALL_ORDER)
  , outOfOrder_(false)
  , returnValue_("")
  , objectPtr_(nullptr)
  , isSpecificObjectExpected_(false)
  , wasPassedToObject_(true)
  , actualCalls_(0)
  , expectedCalls_(1)
{
  inputParameters_ = new MockNamedValueList();
  outputParameters_ = new MockNamedValueList();
}

MockCheckedExpectedCall::MockCheckedExpectedCall(unsigned int numCalls)
  : ignoreOtherParameters_(false)
  , isActualCallMatchFinalized_(false)
  , initialExpectedCallOrder_(NO_EXPECTED_CALL_ORDER)
  , finalExpectedCallOrder_(NO_EXPECTED_CALL_ORDER)
  , outOfOrder_(false)
  , returnValue_("")
  , objectPtr_(nullptr)
  , isSpecificObjectExpected_(false)
  , wasPassedToObject_(true)
  , actualCalls_(0)
  , expectedCalls_(numCalls)
{
  inputParameters_ = new MockNamedValueList();
  outputParameters_ = new MockNamedValueList();
}

MockCheckedExpectedCall::~MockCheckedExpectedCall()
{
  inputParameters_->clear();
  delete inputParameters_;
  outputParameters_->clear();
  delete outputParameters_;
}

MockExpectedCall&
MockCheckedExpectedCall::withName(const String& name)
{
  setName(name);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::withBoolParameter(const String& name, bool value)
{
  MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
  inputParameters_->add(newParameter);
  newParameter->setValue(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::withUnsignedIntParameter(const String& name,
    unsigned int value)
{
  MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
  inputParameters_->add(newParameter);
  newParameter->setValue(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::withIntParameter(const String& name, int value)
{
  MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
  inputParameters_->add(newParameter);
  newParameter->setValue(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::withLongIntParameter(const String& name,
    long int value)
{
  MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
  inputParameters_->add(newParameter);
  newParameter->setValue(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::withUnsignedLongIntParameter(const String& name,
    unsigned long int value)
{
  MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
  inputParameters_->add(newParameter);
  newParameter->setValue(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::withLongLongIntParameter(const String& name,
    long long value)
{
  MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
  inputParameters_->add(newParameter);
  newParameter->setValue(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::withUnsignedLongLongIntParameter(const String& name,
    unsigned long long value)
{
  MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
  inputParameters_->add(newParameter);
  newParameter->setValue(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::withDoubleParameter(const String& name, double value)
{
  MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
  inputParameters_->add(newParameter);
  newParameter->setValue(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::withDoubleParameter(const String& name,
    double value,
    double tolerance)
{
  MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
  inputParameters_->add(newParameter);
  newParameter->setValue(value, tolerance);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::withStringParameter(const String& name,
    const char* value)
{
  MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
  inputParameters_->add(newParameter);
  newParameter->setValue(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::withPointerParameter(const String& name, void* value)
{
  MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
  inputParameters_->add(newParameter);
  newParameter->setValue(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::withConstPointerParameter(const String& name,
    const void* value)
{
  MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
  inputParameters_->add(newParameter);
  newParameter->setValue(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::withFunctionPointerParameter(const String& name,
    void (*value)())
{
  MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
  inputParameters_->add(newParameter);
  newParameter->setValue(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::withMemoryBufferParameter(const String& name,
    const unsigned char* value,
    size_t size)
{
  MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
  inputParameters_->add(newParameter);
  newParameter->setMemoryBuffer(value, size);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::withParameterOfType(const String& type,
    const String& name,
    const void* value)
{
  MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
  inputParameters_->add(newParameter);
  newParameter->setConstObjectPointer(type, value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::withOutputParameterReturning(const String& name,
    const void* value,
    size_t size)
{
  MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
  outputParameters_->add(newParameter);
  newParameter->setValue(value);
  newParameter->setSize(size);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::withOutputParameterOfTypeReturning(const String& type,
    const String& name,
    const void* value)
{
  MockNamedValue* newParameter = new MockExpectedFunctionParameter(name);
  outputParameters_->add(newParameter);
  newParameter->setConstObjectPointer(type, value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::withUnmodifiedOutputParameter(const String& name)
{
  return withOutputParameterReturning(name, nullptr, 0);
}

String
MockCheckedExpectedCall::getInputParameterType(const String& name)
{
  MockNamedValue* p = inputParameters_->getValueByName(name);
  return (p) ? p->getType() : cpputest::StringFrom("");
}

bool
MockCheckedExpectedCall::hasInputParameterWithName(const String& name)
{
  MockNamedValue* p = inputParameters_->getValueByName(name);
  return p != nullptr;
}

bool
MockCheckedExpectedCall::hasOutputParameterWithName(const String& name)
{
  MockNamedValue* p = outputParameters_->getValueByName(name);
  return p != nullptr;
}

MockNamedValue
MockCheckedExpectedCall::getInputParameter(const String& name)
{
  MockNamedValue* p = inputParameters_->getValueByName(name);
  return (p) ? *p : MockNamedValue("");
}

MockNamedValue
MockCheckedExpectedCall::getOutputParameter(const String& name)
{
  MockNamedValue* p = outputParameters_->getValueByName(name);
  return (p) ? *p : MockNamedValue("");
}

bool
MockCheckedExpectedCall::areParametersMatchingActualCall()
{
  MockNamedValueListNode* p;
  for (p = inputParameters_->begin(); p; p = p->next())
    if (!item(p)->isMatchingActualCall())
      return false;
  for (p = outputParameters_->begin(); p; p = p->next())
    if (!item(p)->isMatchingActualCall())
      return false;
  return true;
}

MockExpectedCall&
MockCheckedExpectedCall::ignoreOtherParameters()
{
  ignoreOtherParameters_ = true;
  return *this;
}

bool
MockCheckedExpectedCall::isFulfilled()
{
  return (actualCalls_ == expectedCalls_);
}

bool
MockCheckedExpectedCall::canMatchActualCalls()
{
  return (actualCalls_ < expectedCalls_);
}

bool
MockCheckedExpectedCall::isMatchingActualCallAndFinalized()
{
  return isMatchingActualCall() &&
         (!ignoreOtherParameters_ || isActualCallMatchFinalized_);
}

bool
MockCheckedExpectedCall::isMatchingActualCall()
{
  return areParametersMatchingActualCall() && wasPassedToObject_;
}

void
MockCheckedExpectedCall::callWasMade(unsigned int callOrder)
{
  actualCalls_++;

  if ((initialExpectedCallOrder_ != NO_EXPECTED_CALL_ORDER) &&
      ((callOrder < initialExpectedCallOrder_) ||
          (callOrder > finalExpectedCallOrder_))) {
    outOfOrder_ = true;
  }

  resetActualCallMatchingState();
}

void
MockCheckedExpectedCall::finalizeActualCallMatch()
{
  isActualCallMatchFinalized_ = true;
}

void
MockCheckedExpectedCall::wasPassedToObject()
{
  wasPassedToObject_ = true;
}

void
MockCheckedExpectedCall::resetActualCallMatchingState()
{
  wasPassedToObject_ = !isSpecificObjectExpected_;
  isActualCallMatchFinalized_ = false;

  MockNamedValueListNode* p;

  for (p = inputParameters_->begin(); p; p = p->next())
    item(p)->setMatchesActualCall(false);
  for (p = outputParameters_->begin(); p; p = p->next())
    item(p)->setMatchesActualCall(false);
}

void
MockCheckedExpectedCall::inputParameterWasPassed(const String& name)
{
  for (MockNamedValueListNode* p = inputParameters_->begin(); p;
      p = p->next()) {
    if (p->getName() == name)
      item(p)->setMatchesActualCall(true);
  }
}

void
MockCheckedExpectedCall::outputParameterWasPassed(const String& name)
{
  for (MockNamedValueListNode* p = outputParameters_->begin(); p;
      p = p->next()) {
    if (p->getName() == name)
      item(p)->setMatchesActualCall(true);
  }
}

String
MockCheckedExpectedCall::getInputParameterValueString(const String& name)
{
  MockNamedValue* p = inputParameters_->getValueByName(name);
  return (p) ? StringFrom(*p) : cpputest::StringFrom("failed");
}

bool
MockCheckedExpectedCall::hasInputParameter(const MockNamedValue& parameter)
{
  MockNamedValue* p = inputParameters_->getValueByName(parameter.getName());
  return (p) ? p->equals(parameter) : ignoreOtherParameters_;
}

bool
MockCheckedExpectedCall::hasOutputParameter(const MockNamedValue& parameter)
{
  MockNamedValue* p = outputParameters_->getValueByName(parameter.getName());
  return (p) ? p->compatibleForCopying(parameter) : ignoreOtherParameters_;
}

String
MockCheckedExpectedCall::callToString()
{
  String str;
  if (isSpecificObjectExpected_)
    str = StringFromFormat("(object address: %p)::", objectPtr_);

  str += getName();
  str += " -> ";
  if (initialExpectedCallOrder_ != NO_EXPECTED_CALL_ORDER) {
    if (initialExpectedCallOrder_ == finalExpectedCallOrder_) {
      str += StringFromFormat(
          "expected call order: <%u> -> ", initialExpectedCallOrder_);
    } else {
      str += StringFromFormat("expected calls order: <%u..%u> -> ",
          initialExpectedCallOrder_,
          finalExpectedCallOrder_);
    }
  }

  if (inputParameters_->begin() == nullptr &&
      outputParameters_->begin() == nullptr) {
    str +=
        (ignoreOtherParameters_) ? "all parameters ignored" : "no parameters";
  } else {
    MockNamedValueListNode* p;

    for (p = inputParameters_->begin(); p; p = p->next()) {
      str += StringFromFormat("%s %s: <%s>",
          p->getType().c_str(),
          p->getName().c_str(),
          getInputParameterValueString(p->getName()).c_str());
      if (p->next())
        str += ", ";
    }

    if (inputParameters_->begin() && outputParameters_->begin()) {
      str += ", ";
    }

    for (p = outputParameters_->begin(); p; p = p->next()) {
      str += StringFromFormat(
          "%s %s: <output>", p->getType().c_str(), p->getName().c_str());
      if (p->next())
        str += ", ";
    }

    if (ignoreOtherParameters_)
      str += ", other parameters are ignored";
  }

  str += StringFromFormat(" (expected %u call%s, called %u time%s)",
      expectedCalls_,
      (expectedCalls_ == 1) ? "" : "s",
      actualCalls_,
      (actualCalls_ == 1) ? "" : "s");

  return str;
}

String
MockCheckedExpectedCall::missingParametersToString()
{
  String str;
  MockNamedValueListNode* p;

  for (p = inputParameters_->begin(); p; p = p->next()) {
    if (!item(p)->isMatchingActualCall()) {
      if (str != "")
        str += ", ";
      str +=
          StringFromFormat("%s %s", p->getType().c_str(), p->getName().c_str());
    }
  }
  for (p = outputParameters_->begin(); p; p = p->next()) {
    if (!item(p)->isMatchingActualCall()) {
      if (str != "")
        str += ", ";
      str +=
          StringFromFormat("%s %s", p->getType().c_str(), p->getName().c_str());
    }
  }
  return str;
}

bool
MockCheckedExpectedCall::relatesTo(const String& functionName)
{
  return functionName == getName();
}

bool
MockCheckedExpectedCall::relatesToObject(const void* objectPtr) const
{
  return (!isSpecificObjectExpected_) || (objectPtr_ == objectPtr);
}

MockCheckedExpectedCall::MockExpectedFunctionParameter*
MockCheckedExpectedCall::item(MockNamedValueListNode* node)
{
  return static_cast<MockExpectedFunctionParameter*>(node->item());
}

MockCheckedExpectedCall::MockExpectedFunctionParameter::
    MockExpectedFunctionParameter(const String& name)
  : MockNamedValue(name)
  , matchesActualCall_(false)
{
}

void
MockCheckedExpectedCall::MockExpectedFunctionParameter::setMatchesActualCall(
    bool b)
{
  matchesActualCall_ = b;
}

bool
MockCheckedExpectedCall::MockExpectedFunctionParameter::isMatchingActualCall()
    const
{
  return matchesActualCall_;
}

MockExpectedCall&
MockCheckedExpectedCall::andReturnValue(bool value)
{
  returnValue_.setName("returnValue");
  returnValue_.setValue(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::andReturnValue(unsigned int value)
{
  returnValue_.setName("returnValue");
  returnValue_.setValue(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::andReturnValue(int value)
{
  returnValue_.setName("returnValue");
  returnValue_.setValue(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::andReturnValue(long int value)
{
  returnValue_.setName("returnValue");
  returnValue_.setValue(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::andReturnValue(unsigned long int value)
{
  returnValue_.setName("returnValue");
  returnValue_.setValue(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::andReturnValue(long long value)
{
  returnValue_.setName("returnValue");
  returnValue_.setValue(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::andReturnValue(unsigned long long value)
{
  returnValue_.setName("returnValue");
  returnValue_.setValue(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::andReturnValue(const char* value)
{
  returnValue_.setName("returnValue");
  returnValue_.setValue(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::andReturnValue(double value)
{
  returnValue_.setName("returnValue");
  returnValue_.setValue(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::andReturnValue(void* value)
{
  returnValue_.setName("returnValue");
  returnValue_.setValue(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::andReturnValue(const void* value)
{
  returnValue_.setName("returnValue");
  returnValue_.setValue(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::andReturnValue(void (*value)())
{
  returnValue_.setName("returnValue");
  returnValue_.setValue(value);
  return *this;
}

MockExpectedCall&
MockCheckedExpectedCall::onObject(void* objectPtr)
{
  isSpecificObjectExpected_ = true;
  wasPassedToObject_ = false;
  objectPtr_ = objectPtr;
  return *this;
}

MockNamedValue
MockCheckedExpectedCall::returnValue()
{
  return returnValue_;
}

MockExpectedCall&
MockCheckedExpectedCall::withCallOrder(unsigned int initialCallOrder,
    unsigned int finalCallOrder)
{
  initialExpectedCallOrder_ = initialCallOrder;
  finalExpectedCallOrder_ = finalCallOrder;
  return *this;
}

bool
MockCheckedExpectedCall::isOutOfOrder() const
{
  return outOfOrder_;
}

unsigned int
MockCheckedExpectedCall::getActualCallsFulfilled() const
{
  return actualCalls_;
}

MockExpectedCall&
MockIgnoredExpectedCall::instance()
{
  static MockIgnoredExpectedCall call;
  return call;
}

} // namespace extensions
} // namespace cpputest
