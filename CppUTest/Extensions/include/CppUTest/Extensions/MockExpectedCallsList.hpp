#ifndef INCLUDED_CPPUTEST_EXTENSIONS_MOCKEXPECTEDCALLSLIST_HPP
#define INCLUDED_CPPUTEST_EXTENSIONS_MOCKEXPECTEDCALLSLIST_HPP

#include "CppUTest/String.hpp"

namespace cpputest {
namespace extensions {

class MockCheckedExpectedCall;
class MockNamedValue;

class MockExpectedCallsList
{

public:
  MockExpectedCallsList();
  virtual ~MockExpectedCallsList();
  virtual void deleteAllExpectationsAndClearList();

  virtual unsigned int size() const;
  virtual unsigned int amountOfActualCallsFulfilledFor(
      const cpputest::String& name) const;
  virtual unsigned int amountOfUnfulfilledExpectations() const;
  virtual bool hasUnfulfilledExpectations() const;
  virtual bool hasFinalizedMatchingExpectations() const;
  virtual bool hasUnmatchingExpectationsBecauseOfMissingParameters() const;
  virtual bool hasExpectationWithName(const cpputest::String& name) const;
  virtual bool hasCallsOutOfOrder() const;
  virtual bool empty() const;

  virtual void addExpectedCall(MockCheckedExpectedCall* call);
  virtual void addExpectations(const MockExpectedCallsList& list);
  virtual void addExpectationsRelatedTo(const cpputest::String& name,
      const MockExpectedCallsList& list);

  virtual void onlyKeepOutOfOrderExpectations();
  virtual void addPotentiallyMatchingExpectations(
      const MockExpectedCallsList& list);

  virtual void onlyKeepExpectationsRelatedTo(const cpputest::String& name);
  virtual void onlyKeepExpectationsWithInputParameter(
      const MockNamedValue& parameter);
  virtual void onlyKeepExpectationsWithInputParameterName(
      const cpputest::String& name);
  virtual void onlyKeepExpectationsWithOutputParameter(
      const MockNamedValue& parameter);
  virtual void onlyKeepExpectationsWithOutputParameterName(
      const cpputest::String& name);
  virtual void onlyKeepExpectationsOnObject(const void* objectPtr);
  virtual void onlyKeepUnmatchingExpectations();

  virtual MockCheckedExpectedCall* removeFirstFinalizedMatchingExpectation();
  virtual MockCheckedExpectedCall* removeFirstMatchingExpectation();
  virtual MockCheckedExpectedCall* getFirstMatchingExpectation();

  virtual void resetActualCallMatchingState();
  virtual void wasPassedToObject();
  virtual void parameterWasPassed(const cpputest::String& parameterName);
  virtual void outputParameterWasPassed(const cpputest::String& parameterName);

  virtual cpputest::String unfulfilledCallsToString(
      const cpputest::String& linePrefix = "") const;
  virtual cpputest::String fulfilledCallsToString(
      const cpputest::String& linePrefix = "") const;
  virtual cpputest::String callsWithMissingParametersToString(
      const cpputest::String& linePrefix,
      const cpputest::String& missingParametersPrefix) const;

protected:
  virtual void pruneEmptyNodeFromList();

  class MockExpectedCallsListNode
  {
  public:
    MockCheckedExpectedCall* expectedCall_;

    MockExpectedCallsListNode* next_;
    MockExpectedCallsListNode(MockCheckedExpectedCall* expectedCall)
      : expectedCall_(expectedCall)
      , next_(nullptr)
    {
    }
  };

private:
  MockExpectedCallsListNode* head_;

  MockExpectedCallsList(const MockExpectedCallsList&);
};

} // namespace extensions
} // namespace cpputest

#endif
