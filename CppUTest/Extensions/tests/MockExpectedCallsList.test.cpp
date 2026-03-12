#include "CppUTest/Extensions/MockExpectedCallsList.hpp"

#include "MockFailureReporterForTest.h"

#include "CppUTest/Extensions/MockCheckedExpectedCall.hpp"
#include "CppUTest/Extensions/MockFailure.hpp"

#include "CppUTest/TestHarness.hpp"

using namespace cpputest;
using namespace cpputest::extensions;

TEST_GROUP(MockExpectedCallsList)
{
  MockExpectedCallsList* list;
  MockCheckedExpectedCall* call1;
  MockCheckedExpectedCall* call2;
  MockCheckedExpectedCall* call3;
  MockCheckedExpectedCall* call4;
  void setup() override
  {
    list = new MockExpectedCallsList;
    call1 = new MockCheckedExpectedCall;
    call2 = new MockCheckedExpectedCall;
    call3 = new MockCheckedExpectedCall;
    call4 = new MockCheckedExpectedCall;
    call1->withName("foo");
    call2->withName("bar");
    call3->withName("boo");
  }
  void teardown() override
  {
    delete call1;
    delete call2;
    delete call3;
    delete call4;
    delete list;
    CHECK_NO_MOCK_FAILURE();
    MockFailureReporterForTest::clearReporter();
  }
};

TEST(MockExpectedCallsList, emptyList)
{
  CHECK(!list->hasUnfulfilledExpectations());
  CHECK(!list->hasFinalizedMatchingExpectations());
  LONGS_EQUAL(0, list->size());
}

TEST(MockExpectedCallsList, addingCalls)
{
  list->addExpectedCall(call1);
  list->addExpectedCall(call2);
  LONGS_EQUAL(2, list->size());
}

TEST(MockExpectedCallsList, listWithFulfilledExpectationHasNoUnfulfilledOnes)
{
  call1->callWasMade(1);
  call2->callWasMade(2);
  list->addExpectedCall(call1);
  list->addExpectedCall(call2);
  CHECK(!list->hasUnfulfilledExpectations());
}

TEST(MockExpectedCallsList, listWithFulfilledExpectationButOutOfOrder)
{
  call1->withCallOrder(1);
  call2->withCallOrder(2);
  list->addExpectedCall(call1);
  list->addExpectedCall(call2);
  call2->callWasMade(1);
  call1->callWasMade(2);
  CHECK(!list->hasUnfulfilledExpectations());
  CHECK(list->hasCallsOutOfOrder());
}

TEST(MockExpectedCallsList, listWithUnFulfilledExpectationHasNoUnfillfilledOnes)
{
  call1->callWasMade(1);
  call3->callWasMade(2);
  list->addExpectedCall(call1);
  list->addExpectedCall(call2);
  list->addExpectedCall(call3);
  CHECK(list->hasUnfulfilledExpectations());
}

TEST(MockExpectedCallsList, deleteAllExpectationsAndClearList)
{
  list->addExpectedCall(new MockCheckedExpectedCall);
  list->addExpectedCall(new MockCheckedExpectedCall);
  list->deleteAllExpectationsAndClearList();
}

TEST(MockExpectedCallsList, onlyKeepUnmatchingExpectations)
{
  call1->withName("relate");
  call2->withName("unrelate");
  call3->withName("relate").withParameter("param", 1);
  list->addExpectedCall(call1);
  list->addExpectedCall(call2);
  list->addExpectedCall(call3);
  list->onlyKeepUnmatchingExpectations();
  LONGS_EQUAL(1, list->size());
}

TEST(MockExpectedCallsList, onlyKeepExpectationsRelatedTo)
{
  call1->withName("relate");
  call2->withName("unrelate");
  call3->withName("unrelate");
  list->addExpectedCall(call1);
  list->addExpectedCall(call2);
  list->addExpectedCall(call3);
  list->onlyKeepExpectationsRelatedTo("relate");
  LONGS_EQUAL(1, list->size());
}

TEST(MockExpectedCallsList,
    removeAllExpectationsExceptThisThatRelateToTheWoleList)
{
  call1->withName("relate");
  call2->withName("relate");
  call3->withName("relate");
  list->addExpectedCall(call1);
  list->addExpectedCall(call2);
  list->addExpectedCall(call3);
  list->onlyKeepExpectationsRelatedTo("unrelate");
  LONGS_EQUAL(0, list->size());
}

TEST(MockExpectedCallsList, removeAllExpectationsExceptThisThatRelateToFirstOne)
{
  call1->withName("relate");
  call2->withName("unrelate");
  list->addExpectedCall(call1);
  list->addExpectedCall(call2);
  list->onlyKeepExpectationsRelatedTo("unrelate");
  LONGS_EQUAL(1, list->size());
}

TEST(MockExpectedCallsList, removeAllExpectationsExceptThisThatRelateToLastOne)
{
  call1->withName("unrelate");
  call2->withName("relate");
  list->addExpectedCall(call1);
  list->addExpectedCall(call2);
  list->onlyKeepExpectationsRelatedTo("unrelate");
  LONGS_EQUAL(1, list->size());
}

TEST(MockExpectedCallsList, onlyKeepExpectationsWithInputParameterName)
{
  call1->withName("func").withParameter("param", 1);
  call2->withName("func").withParameter("diffname", 1);
  call3->withName("func").withParameter("diffname", 1);
  list->addExpectedCall(call1);
  list->addExpectedCall(call2);
  list->addExpectedCall(call3);
  list->onlyKeepExpectationsWithInputParameterName("diffname");
  LONGS_EQUAL(2, list->size());
}

TEST(MockExpectedCallsList, onlyKeepExpectationsWithInputParameter)
{
  MockNamedValue parameter("diffname");
  parameter.setValue(1);
  call1->withName("func").withParameter("param", 1);
  call2->withName("func").withParameter("diffname", 1);
  call3->withName("func").withParameter("diffname", 1);
  call4->withName("func").withParameter("diffname", 2);
  call3->callWasMade(1);
  call3->inputParameterWasPassed("diffname");
  list->addExpectedCall(call1);
  list->addExpectedCall(call2);
  list->addExpectedCall(call3);
  list->addExpectedCall(call4);
  list->onlyKeepExpectationsWithInputParameter(parameter);
  LONGS_EQUAL(2, list->size());
}

TEST(MockExpectedCallsList, addPotentiallyMatchingExpectationsWithEmptyList)
{
  MockExpectedCallsList newList;
  newList.addPotentiallyMatchingExpectations(*list);
  LONGS_EQUAL(0, newList.size());
}

TEST(MockExpectedCallsList,
    addPotentiallyMatchingExpectationsMultipleUnmatchedExpectations)
{
  call2->callWasMade(1);
  list->addExpectedCall(call1);
  list->addExpectedCall(call2);
  list->addExpectedCall(call3);
  MockExpectedCallsList newList;
  newList.addPotentiallyMatchingExpectations(*list);
  LONGS_EQUAL(2, newList.size());
}

TEST(MockExpectedCallsList, amountOfActualCallsFulfilledFor_HasOneRelated)
{
  call1->withName("foo");
  call1->callWasMade(1);
  call2->withName("bar");
  call2->callWasMade(2);
  list->addExpectedCall(call1);
  list->addExpectedCall(call2);
  LONGS_EQUAL(1, list->amountOfActualCallsFulfilledFor("bar"));
}

TEST(MockExpectedCallsList, amountOfActualCallsFulfilledFor_HasNone)
{
  call1->withName("foo");
  call1->callWasMade(1);
  call2->withName("bar");
  list->addExpectedCall(call1);
  LONGS_EQUAL(0, list->amountOfActualCallsFulfilledFor("bar"));
}

TEST(MockExpectedCallsList, callToStringForUnfulfilledFunctions)
{
  call1->withName("foo");
  call2->withName("bar");
  call3->withName("blah");
  call3->callWasMade(1);

  list->addExpectedCall(call1);
  list->addExpectedCall(call2);
  list->addExpectedCall(call3);

  String expectedString;
  expectedString = StringFromFormat(
      "%s\n%s", call1->callToString().c_str(), call2->callToString().c_str());
  STRCMP_EQUAL(
      expectedString.c_str(), list->unfulfilledCallsToString().c_str());
}

TEST(MockExpectedCallsList, callsWithMissingParametersToString)
{
  call1->withName("foo").withParameter("boo", 0);
  call2->withName("bar").withParameter("baa", 10).withParameter("baz", "blah");
  call2->inputParameterWasPassed("baa");

  list->addExpectedCall(call1);
  list->addExpectedCall(call2);

  String expectedString;
  expectedString = StringFromFormat("-%s\n-#%s\n-%s\n-#%s",
      call1->callToString().c_str(),
      call1->missingParametersToString().c_str(),
      call2->callToString().c_str(),
      call2->missingParametersToString().c_str());
  STRCMP_EQUAL(expectedString.c_str(),
      list->callsWithMissingParametersToString("-", "#").c_str());
}

TEST(MockExpectedCallsList, callToStringForFulfilledFunctions)
{
  call1->withName("foo");
  call2->withName("bar");

  call2->callWasMade(1);
  call1->callWasMade(2);

  list->addExpectedCall(call1);
  list->addExpectedCall(call2);

  String expectedString;
  expectedString = StringFromFormat(
      "%s\n%s", call1->callToString().c_str(), call2->callToString().c_str());
  STRCMP_EQUAL(expectedString.c_str(), list->fulfilledCallsToString().c_str());
}

TEST(MockExpectedCallsList, removeOneFinalizedMatchingExpectationFromEmptyList)
{
  POINTERS_EQUAL(nullptr, list->removeFirstFinalizedMatchingExpectation());
}

TEST(MockExpectedCallsList, getOneMatchingExpectationFromEmptyList)
{
  POINTERS_EQUAL(nullptr, list->getFirstMatchingExpectation());
}

TEST(MockExpectedCallsList, toStringOnEmptyList)
{
  STRCMP_EQUAL("<none>", list->unfulfilledCallsToString().c_str());
}

TEST(MockExpectedCallsList, hasFinalizedMatchingExpectations_emptyList)
{
  CHECK(!list->hasFinalizedMatchingExpectations());
}

TEST(MockExpectedCallsList,
    hasFinalizedMatchingExpectations_listHasNonMatchingCalls)
{
  call1->withParameter("param", 0);
  call2->withParameter("param", 0);
  call3->withParameter("param", 0);
  list->addExpectedCall(call1);
  list->addExpectedCall(call2);
  list->addExpectedCall(call3);

  CHECK(!list->hasFinalizedMatchingExpectations());
}

TEST(MockExpectedCallsList,
    hasFinalizedMatchingExpectations_listHasMatchingButNotFinalizedCall)
{
  list->addExpectedCall(call1);
  list->addExpectedCall(call2);
  call1->ignoreOtherParameters();
  call2->withParameter("param", 0);

  CHECK(!list->hasFinalizedMatchingExpectations());
}

TEST(MockExpectedCallsList,
    hasFinalizedMatchingExpectations_listHasFinalizedCallThatIgnoresParameters)
{
  list->addExpectedCall(call1);
  list->addExpectedCall(call2);
  call1->ignoreOtherParameters();
  call2->withParameter("param", 0);

  call1->finalizeActualCallMatch();

  CHECK(list->hasFinalizedMatchingExpectations());
}

TEST(MockExpectedCallsList,
    hasFinalizedMatchingExpectations_listHasFinalizedCallThatDoesntIgnoreParameters)
{
  list->addExpectedCall(call1);
  list->addExpectedCall(call2);
  call1->withParameter("param", 1);
  call2->withParameter("param", 0);

  call1->inputParameterWasPassed("param");

  CHECK(list->hasFinalizedMatchingExpectations());
}
