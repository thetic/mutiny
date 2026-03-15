#include "CppUTest/Extensions/MockExpectedCallsList.hpp"

#include "MockFailureReporterForTest.hpp"

#include "CppUTest/Extensions/MockCheckedExpectedCall.hpp"
#include "CppUTest/Extensions/MockFailure.hpp"

#include "CppUTest/TestHarness.hpp"

TEST_GROUP(MockExpectedCallsList)
{
  MockExpectedCallsListForTest::MockExpectedCallsList* list;
  cpputest::extensions::MockCheckedExpectedCall* call1;
  cpputest::extensions::MockCheckedExpectedCall* call2;
  cpputest::extensions::MockCheckedExpectedCall* call3;
  cpputest::extensions::MockCheckedExpectedCall* call4;
  void setup() override
  {
    list = new MockExpectedCallsListForTest::MockExpectedCallsList;
    call1 = new cpputest::extensions::MockCheckedExpectedCall;
    call2 = new cpputest::extensions::MockCheckedExpectedCall;
    call3 = new cpputest::extensions::MockCheckedExpectedCall;
    call4 = new cpputest::extensions::MockCheckedExpectedCall;
    call1->with_name("foo");
    call2->with_name("bar");
    call3->with_name("boo");
  }
  void teardown() override
  {
    delete call1;
    delete call2;
    delete call3;
    delete call4;
    delete list;
    CHECK_NO_MOCK_FAILURE();
    MockFailureReporterForTest::clear_reporter();
  }
};

TEST(MockExpectedCallsList, emptyList)
{
  CHECK(!list->has_unfulfilled_expectations());
  CHECK(!list->has_finalized_matching_expectations());
  LONGS_EQUAL(0, list->size());
}

TEST(MockExpectedCallsList, addingCalls)
{
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  LONGS_EQUAL(2, list->size());
}

TEST(MockExpectedCallsList, listWithFulfilledExpectationHasNoUnfulfilledOnes)
{
  call1->call_was_made(1);
  call2->call_was_made(2);
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  CHECK(!list->has_unfulfilled_expectations());
}

TEST(MockExpectedCallsList, listWithFulfilledExpectationButOutOfOrder)
{
  call1->with_call_order(1);
  call2->with_call_order(2);
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  call2->call_was_made(1);
  call1->call_was_made(2);
  CHECK(!list->has_unfulfilled_expectations());
  CHECK(list->has_calls_out_of_order());
}

TEST(MockExpectedCallsList, listWithUnFulfilledExpectationHasNoUnfillfilledOnes)
{
  call1->call_was_made(1);
  call3->call_was_made(2);
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  list->add_expected_call(call3);
  CHECK(list->has_unfulfilled_expectations());
}

TEST(MockExpectedCallsList, deleteAllExpectationsAndClearList)
{
  list->add_expected_call(new cpputest::extensions::MockCheckedExpectedCall);
  list->add_expected_call(new cpputest::extensions::MockCheckedExpectedCall);
  list->delete_all_expectations_and_clear_list();
}

TEST(MockExpectedCallsList, onlyKeepUnmatchingExpectations)
{
  call1->with_name("relate");
  call2->with_name("unrelate");
  call3->with_name("relate").with_parameter("param", 1);
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  list->add_expected_call(call3);
  list->only_keep_unmatching_expectations();
  LONGS_EQUAL(1, list->size());
}

TEST(MockExpectedCallsList, onlyKeepExpectationsRelatedTo)
{
  call1->with_name("relate");
  call2->with_name("unrelate");
  call3->with_name("unrelate");
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  list->add_expected_call(call3);
  list->only_keep_expectations_related_to("relate");
  LONGS_EQUAL(1, list->size());
}

TEST(MockExpectedCallsList,
    removeAllExpectationsExceptThisThatRelateToTheWoleList)
{
  call1->with_name("relate");
  call2->with_name("relate");
  call3->with_name("relate");
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  list->add_expected_call(call3);
  list->only_keep_expectations_related_to("unrelate");
  LONGS_EQUAL(0, list->size());
}

TEST(MockExpectedCallsList, removeAllExpectationsExceptThisThatRelateToFirstOne)
{
  call1->with_name("relate");
  call2->with_name("unrelate");
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  list->only_keep_expectations_related_to("unrelate");
  LONGS_EQUAL(1, list->size());
}

TEST(MockExpectedCallsList, removeAllExpectationsExceptThisThatRelateToLastOne)
{
  call1->with_name("unrelate");
  call2->with_name("relate");
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  list->only_keep_expectations_related_to("unrelate");
  LONGS_EQUAL(1, list->size());
}

TEST(MockExpectedCallsList, onlyKeepExpectationsWithInputParameterName)
{
  call1->with_name("func").with_parameter("param", 1);
  call2->with_name("func").with_parameter("diffname", 1);
  call3->with_name("func").with_parameter("diffname", 1);
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  list->add_expected_call(call3);
  list->only_keep_expectations_with_input_parameter_name("diffname");
  LONGS_EQUAL(2, list->size());
}

TEST(MockExpectedCallsList, onlyKeepExpectationsWithInputParameter)
{
  cpputest::extensions::MockNamedValue parameter("diffname");
  parameter.set_value(1);
  call1->with_name("func").with_parameter("param", 1);
  call2->with_name("func").with_parameter("diffname", 1);
  call3->with_name("func").with_parameter("diffname", 1);
  call4->with_name("func").with_parameter("diffname", 2);
  call3->call_was_made(1);
  call3->input_parameter_was_passed("diffname");
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  list->add_expected_call(call3);
  list->add_expected_call(call4);
  list->only_keep_expectations_with_input_parameter(parameter);
  LONGS_EQUAL(2, list->size());
}

TEST(MockExpectedCallsList, addPotentiallyMatchingExpectationsWithEmptyList)
{
  MockExpectedCallsListForTest::MockExpectedCallsList new_list;
  new_list.add_potentially_matching_expectations(*list);
  LONGS_EQUAL(0, new_list.size());
}

TEST(MockExpectedCallsList,
    addPotentiallyMatchingExpectationsMultipleUnmatchedExpectations)
{
  call2->call_was_made(1);
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  list->add_expected_call(call3);
  MockExpectedCallsListForTest::MockExpectedCallsList new_list;
  new_list.add_potentially_matching_expectations(*list);
  LONGS_EQUAL(2, new_list.size());
}

TEST(MockExpectedCallsList, amountOfActualCallsFulfilledFor_HasOneRelated)
{
  call1->with_name("foo");
  call1->call_was_made(1);
  call2->with_name("bar");
  call2->call_was_made(2);
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  LONGS_EQUAL(1, list->amount_of_actual_calls_fulfilled_for("bar"));
}

TEST(MockExpectedCallsList, amountOfActualCallsFulfilledFor_HasNone)
{
  call1->with_name("foo");
  call1->call_was_made(1);
  call2->with_name("bar");
  list->add_expected_call(call1);
  LONGS_EQUAL(0, list->amount_of_actual_calls_fulfilled_for("bar"));
}

TEST(MockExpectedCallsList, callToStringForUnfulfilledFunctions)
{
  call1->with_name("foo");
  call2->with_name("bar");
  call3->with_name("blah");
  call3->call_was_made(1);

  list->add_expected_call(call1);
  list->add_expected_call(call2);
  list->add_expected_call(call3);

  cpputest::String expected_string;
  expected_string = cpputest::string_from_format("%s\n%s",
      call1->call_to_string().c_str(),
      call2->call_to_string().c_str());
  STRCMP_EQUAL(
      expected_string.c_str(), list->unfulfilled_calls_to_string().c_str());
}

TEST(MockExpectedCallsList, callsWithMissingParametersToString)
{
  call1->with_name("foo").with_parameter("boo", 0);
  call2->with_name("bar").with_parameter("baa", 10).with_parameter(
      "baz", "blah");
  call2->input_parameter_was_passed("baa");

  list->add_expected_call(call1);
  list->add_expected_call(call2);

  cpputest::String expected_string;
  expected_string = cpputest::string_from_format("-%s\n-#%s\n-%s\n-#%s",
      call1->call_to_string().c_str(),
      call1->missing_parameters_to_string().c_str(),
      call2->call_to_string().c_str(),
      call2->missing_parameters_to_string().c_str());
  STRCMP_EQUAL(expected_string.c_str(),
      list->calls_with_missing_parameters_to_string("-", "#").c_str());
}

TEST(MockExpectedCallsList, callToStringForFulfilledFunctions)
{
  call1->with_name("foo");
  call2->with_name("bar");

  call2->call_was_made(1);
  call1->call_was_made(2);

  list->add_expected_call(call1);
  list->add_expected_call(call2);

  cpputest::String expected_string;
  expected_string = cpputest::string_from_format("%s\n%s",
      call1->call_to_string().c_str(),
      call2->call_to_string().c_str());
  STRCMP_EQUAL(
      expected_string.c_str(), list->fulfilled_calls_to_string().c_str());
}

TEST(MockExpectedCallsList, removeOneFinalizedMatchingExpectationFromEmptyList)
{
  POINTERS_EQUAL(nullptr, list->remove_first_finalized_matching_expectation());
}

TEST(MockExpectedCallsList, getOneMatchingExpectationFromEmptyList)
{
  POINTERS_EQUAL(nullptr, list->get_first_matching_expectation());
}

TEST(MockExpectedCallsList, toStringOnEmptyList)
{
  STRCMP_EQUAL("<none>", list->unfulfilled_calls_to_string().c_str());
}

TEST(MockExpectedCallsList, hasFinalizedMatchingExpectations_emptyList)
{
  CHECK(!list->has_finalized_matching_expectations());
}

TEST(MockExpectedCallsList,
    hasFinalizedMatchingExpectations_listHasNonMatchingCalls)
{
  call1->with_parameter("param", 0);
  call2->with_parameter("param", 0);
  call3->with_parameter("param", 0);
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  list->add_expected_call(call3);

  CHECK(!list->has_finalized_matching_expectations());
}

TEST(MockExpectedCallsList,
    hasFinalizedMatchingExpectations_listHasMatchingButNotFinalizedCall)
{
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  call1->ignore_other_parameters();
  call2->with_parameter("param", 0);

  CHECK(!list->has_finalized_matching_expectations());
}

TEST(MockExpectedCallsList,
    hasFinalizedMatchingExpectations_listHasFinalizedCallThatIgnoresParameters)
{
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  call1->ignore_other_parameters();
  call2->with_parameter("param", 0);

  call1->finalize_actual_call_match();

  CHECK(list->has_finalized_matching_expectations());
}

TEST(MockExpectedCallsList,
    hasFinalizedMatchingExpectations_listHasFinalizedCallThatDoesntIgnoreParameters)
{
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  call1->with_parameter("param", 1);
  call2->with_parameter("param", 0);

  call1->input_parameter_was_passed("param");

  CHECK(list->has_finalized_matching_expectations());
}
