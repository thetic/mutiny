#include "CppUTest/Extensions/MockExpectedCallsList.hpp"

#include "CppUTest/Extensions/MockCheckedExpectedCall.hpp"

#include "CppUTest/TestHarness.hpp"

namespace cpputest {
namespace extensions {

MockExpectedCallsList::MockExpectedCallsList()
  : head_(nullptr)
{
}

MockExpectedCallsList::~MockExpectedCallsList()
{
  while (head_) {
    MockExpectedCallsListNode* next = head_->next_;
    delete head_;
    head_ = next;
  }
}

bool
MockExpectedCallsList::hasCallsOutOfOrder() const
{
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
    if (p->expectedCall_->isOutOfOrder())
      return true;
  return false;
}

unsigned int
MockExpectedCallsList::size() const
{
  unsigned int count = 0;
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
    count++;
  return count;
}

bool
MockExpectedCallsList::empty() const
{
  return head_ == nullptr;
}

unsigned int
MockExpectedCallsList::amountOfActualCallsFulfilledFor(const String& name) const
{
  unsigned int count = 0;
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_) {
    if (p->expectedCall_->relatesTo(name)) {
      count += p->expectedCall_->getActualCallsFulfilled();
    }
  }
  return count;
}

unsigned int
MockExpectedCallsList::amountOfUnfulfilledExpectations() const
{
  unsigned int count = 0;
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
    if (!p->expectedCall_->isFulfilled())
      count++;
  return count;
}

bool
MockExpectedCallsList::hasFinalizedMatchingExpectations() const
{
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_) {
    if (p->expectedCall_->isMatchingActualCallAndFinalized()) {
      return true;
    }
  }
  return false;
}

bool
MockExpectedCallsList::hasUnfulfilledExpectations() const
{
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_) {
    if (!p->expectedCall_->isFulfilled()) {
      return true;
    }
  }
  return false;
}

bool
MockExpectedCallsList::hasExpectationWithName(const String& name) const
{
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
    if (p->expectedCall_->relatesTo(name))
      return true;
  return false;
}

void
MockExpectedCallsList::addExpectedCall(MockCheckedExpectedCall* call)
{
  MockExpectedCallsListNode* newCall = new MockExpectedCallsListNode(call);

  if (head_ == nullptr)
    head_ = newCall;
  else {
    MockExpectedCallsListNode* lastCall = head_;
    while (lastCall->next_)
      lastCall = lastCall->next_;
    lastCall->next_ = newCall;
  }
}

void
MockExpectedCallsList::addPotentiallyMatchingExpectations(
    const MockExpectedCallsList& list)
{
  for (MockExpectedCallsListNode* p = list.head_; p; p = p->next_)
    if (p->expectedCall_->canMatchActualCalls())
      addExpectedCall(p->expectedCall_);
}

void
MockExpectedCallsList::addExpectationsRelatedTo(const String& name,
    const MockExpectedCallsList& list)
{
  for (MockExpectedCallsListNode* p = list.head_; p; p = p->next_)
    if (p->expectedCall_->relatesTo(name))
      addExpectedCall(p->expectedCall_);
}

void
MockExpectedCallsList::addExpectations(const MockExpectedCallsList& list)
{
  for (MockExpectedCallsListNode* p = list.head_; p; p = p->next_)
    addExpectedCall(p->expectedCall_);
}

void
MockExpectedCallsList::onlyKeepExpectationsRelatedTo(const String& name)
{
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
    if (!p->expectedCall_->relatesTo(name))
      p->expectedCall_ = nullptr;

  pruneEmptyNodeFromList();
}

void
MockExpectedCallsList::onlyKeepOutOfOrderExpectations()
{
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
    if (!p->expectedCall_->isOutOfOrder())
      p->expectedCall_ = nullptr;
  pruneEmptyNodeFromList();
}

void
MockExpectedCallsList::onlyKeepUnmatchingExpectations()
{
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
    if (p->expectedCall_->isMatchingActualCallAndFinalized()) {
      p->expectedCall_->resetActualCallMatchingState();
      p->expectedCall_ = nullptr;
    }

  pruneEmptyNodeFromList();
}

void
MockExpectedCallsList::onlyKeepExpectationsWithInputParameterName(
    const String& name)
{
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
    if (!p->expectedCall_->hasInputParameterWithName(name))
      p->expectedCall_ = nullptr;
  pruneEmptyNodeFromList();
}

void
MockExpectedCallsList::onlyKeepExpectationsWithOutputParameterName(
    const String& name)
{
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
    if (!p->expectedCall_->hasOutputParameterWithName(name))
      p->expectedCall_ = nullptr;
  pruneEmptyNodeFromList();
}

void
MockExpectedCallsList::onlyKeepExpectationsWithInputParameter(
    const MockNamedValue& parameter)
{
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
    if (!p->expectedCall_->hasInputParameter(parameter))
      p->expectedCall_ = nullptr;
  pruneEmptyNodeFromList();
}

void
MockExpectedCallsList::onlyKeepExpectationsWithOutputParameter(
    const MockNamedValue& parameter)
{
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
    if (!p->expectedCall_->hasOutputParameter(parameter))
      p->expectedCall_ = nullptr;
  pruneEmptyNodeFromList();
}

void
MockExpectedCallsList::onlyKeepExpectationsOnObject(const void* objectPtr)
{
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
    if (!p->expectedCall_->relatesToObject(objectPtr))
      p->expectedCall_ = nullptr;
  pruneEmptyNodeFromList();
}

MockCheckedExpectedCall*
MockExpectedCallsList::removeFirstFinalizedMatchingExpectation()
{
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_) {
    if (p->expectedCall_->isMatchingActualCallAndFinalized()) {
      MockCheckedExpectedCall* matchingCall = p->expectedCall_;
      p->expectedCall_ = nullptr;
      pruneEmptyNodeFromList();
      return matchingCall;
    }
  }
  return nullptr;
}

MockCheckedExpectedCall*
MockExpectedCallsList::getFirstMatchingExpectation()
{
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_) {
    if (p->expectedCall_->isMatchingActualCall()) {
      return p->expectedCall_;
    }
  }
  return nullptr;
}

MockCheckedExpectedCall*
MockExpectedCallsList::removeFirstMatchingExpectation()
{
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_) {
    if (p->expectedCall_->isMatchingActualCall()) {
      MockCheckedExpectedCall* matchingCall = p->expectedCall_;
      p->expectedCall_ = nullptr;
      pruneEmptyNodeFromList();
      return matchingCall;
    }
  }
  return nullptr;
}

void
MockExpectedCallsList::pruneEmptyNodeFromList()
{
  MockExpectedCallsListNode* current = head_;
  MockExpectedCallsListNode* previous = nullptr;
  MockExpectedCallsListNode* toBeDeleted = nullptr;

  while (current) {
    if (current->expectedCall_ == nullptr) {
      toBeDeleted = current;
      if (previous == nullptr)
        head_ = current = current->next_;
      else
        current = previous->next_ = current->next_;
      delete toBeDeleted;
    } else {
      previous = current;
      current = current->next_;
    }
  }
}

void
MockExpectedCallsList::deleteAllExpectationsAndClearList()
{
  while (head_) {
    MockExpectedCallsListNode* next = head_->next_;
    delete head_->expectedCall_;
    delete head_;
    head_ = next;
  }
}

void
MockExpectedCallsList::resetActualCallMatchingState()
{
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
    p->expectedCall_->resetActualCallMatchingState();
}

void
MockExpectedCallsList::wasPassedToObject()
{
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
    p->expectedCall_->wasPassedToObject();
}

void
MockExpectedCallsList::parameterWasPassed(const String& parameterName)
{
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
    p->expectedCall_->inputParameterWasPassed(parameterName);
}

void
MockExpectedCallsList::outputParameterWasPassed(const String& parameterName)
{
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
    p->expectedCall_->outputParameterWasPassed(parameterName);
}

static String
stringOrNoneTextWhenEmpty(const String& inputString, const String& linePrefix)
{
  String str = inputString;
  if (str == "") {
    str += linePrefix;
    str += "<none>";
  }
  return str;
}

static String
appendStringOnANewLine(const String& inputString,
    const String& linePrefix,
    const String& stringToAppend)
{
  String str = inputString;
  if (str != "")
    str += "\n";
  str += linePrefix;
  str += stringToAppend;
  return str;
}

String
MockExpectedCallsList::unfulfilledCallsToString(const String& linePrefix) const
{
  String str;
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
    if (!p->expectedCall_->isFulfilled())
      str = appendStringOnANewLine(
          str, linePrefix, p->expectedCall_->callToString());
  return stringOrNoneTextWhenEmpty(str, linePrefix);
}

String
MockExpectedCallsList::fulfilledCallsToString(const String& linePrefix) const
{
  String str;

  for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
    if (p->expectedCall_->isFulfilled())
      str = appendStringOnANewLine(
          str, linePrefix, p->expectedCall_->callToString());

  return stringOrNoneTextWhenEmpty(str, linePrefix);
}

String
MockExpectedCallsList::callsWithMissingParametersToString(
    const String& linePrefix,
    const String& missingParametersPrefix) const
{
  String str;
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_) {
    str = appendStringOnANewLine(
        str, linePrefix, p->expectedCall_->callToString());
    str = appendStringOnANewLine(str,
        linePrefix + missingParametersPrefix,
        p->expectedCall_->missingParametersToString());
  }

  return stringOrNoneTextWhenEmpty(str, linePrefix);
}

bool
MockExpectedCallsList::hasUnmatchingExpectationsBecauseOfMissingParameters()
    const
{
  for (MockExpectedCallsListNode* p = head_; p; p = p->next_)
    if (!p->expectedCall_->areParametersMatchingActualCall())
      return true;
  return false;
}

} // namespace extensions
} // namespace cpputest
