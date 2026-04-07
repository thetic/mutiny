#include "MockFailureReporterForTest.hpp"

#include "mutiny/mock/CheckedExpectedCall.hpp"
#include "mutiny/mock/ExpectedCallsList.hpp"
#include "mutiny/mock/Failure.hpp"

#include "mutiny/test.hpp"

TEST_GROUP(ExpectedCallsList)
{
  ExpectedCallsListForTest::ExpectedCallsList* list;
  mu::tiny::mock::CheckedExpectedCall* call1;
  mu::tiny::mock::CheckedExpectedCall* call2;
  mu::tiny::mock::CheckedExpectedCall* call3;
  mu::tiny::mock::CheckedExpectedCall* call4;
  void setup() override
  {
    list = new ExpectedCallsListForTest::ExpectedCallsList;
    call1 = new mu::tiny::mock::CheckedExpectedCall;
    call2 = new mu::tiny::mock::CheckedExpectedCall;
    call3 = new mu::tiny::mock::CheckedExpectedCall;
    call4 = new mu::tiny::mock::CheckedExpectedCall;
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
    FailureReporterForTest::clear_reporter();
  }
};

TEST(ExpectedCallsList, emptyList)
{
  CHECK(!list->has_unfulfilled_expectations());
  CHECK(!list->has_finalized_matching_expectations());
  CHECK_EQUAL(0U, list->size());
}

TEST(ExpectedCallsList, addingCalls)
{
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  CHECK_EQUAL(2U, list->size());
}

TEST(ExpectedCallsList, listWithFulfilledExpectationHasNoUnfulfilledOnes)
{
  call1->call_was_made(1);
  call2->call_was_made(2);
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  CHECK(!list->has_unfulfilled_expectations());
}

TEST(ExpectedCallsList, listWithFulfilledExpectationButOutOfOrder)
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

TEST(ExpectedCallsList, listWithUnFulfilledExpectationHasNoUnfillfilledOnes)
{
  call1->call_was_made(1);
  call3->call_was_made(2);
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  list->add_expected_call(call3);
  CHECK(list->has_unfulfilled_expectations());
}

TEST(ExpectedCallsList, deleteAllExpectationsAndClearList)
{
  list->add_expected_call(new mu::tiny::mock::CheckedExpectedCall);
  list->add_expected_call(new mu::tiny::mock::CheckedExpectedCall);
  list->delete_all_expectations_and_clear_list();
}

TEST(ExpectedCallsList, onlyKeepUnmatchingExpectations)
{
  call1->with_name("relate");
  call2->with_name("unrelate");
  call3->with_name("relate").with_parameter("param", 1);
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  list->add_expected_call(call3);
  list->only_keep_unmatching_expectations();
  CHECK_EQUAL(1U, list->size());
}

TEST(ExpectedCallsList, onlyKeepExpectationsRelatedTo)
{
  call1->with_name("relate");
  call2->with_name("unrelate");
  call3->with_name("unrelate");
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  list->add_expected_call(call3);
  list->only_keep_expectations_related_to("relate");
  CHECK_EQUAL(1U, list->size());
}

TEST(ExpectedCallsList, removeAllExpectationsExceptThisThatRelateToTheWoleList)
{
  call1->with_name("relate");
  call2->with_name("relate");
  call3->with_name("relate");
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  list->add_expected_call(call3);
  list->only_keep_expectations_related_to("unrelate");
  CHECK_EQUAL(0U, list->size());
}

TEST(ExpectedCallsList, removeAllExpectationsExceptThisThatRelateToFirstOne)
{
  call1->with_name("relate");
  call2->with_name("unrelate");
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  list->only_keep_expectations_related_to("unrelate");
  CHECK_EQUAL(1U, list->size());
}

TEST(ExpectedCallsList, removeAllExpectationsExceptThisThatRelateToLastOne)
{
  call1->with_name("unrelate");
  call2->with_name("relate");
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  list->only_keep_expectations_related_to("unrelate");
  CHECK_EQUAL(1U, list->size());
}

TEST(ExpectedCallsList, onlyKeepExpectationsWithInputParameterName)
{
  call1->with_name("func").with_parameter("param", 1);
  call2->with_name("func").with_parameter("diffname", 1);
  call3->with_name("func").with_parameter("diffname", 1);
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  list->add_expected_call(call3);
  list->only_keep_expectations_with_input_parameter_name("diffname");
  CHECK_EQUAL(2U, list->size());
}

TEST(ExpectedCallsList, onlyKeepExpectationsWithInputParameter)
{
  mu::tiny::mock::NamedValue parameter("diffname");
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
  CHECK_EQUAL(2U, list->size());
}

TEST(ExpectedCallsList, addPotentiallyMatchingExpectationsWithEmptyList)
{
  ExpectedCallsListForTest::ExpectedCallsList new_list;
  new_list.add_potentially_matching_expectations(*list);
  CHECK_EQUAL(0U, new_list.size());
}

TEST(ExpectedCallsList, addPotentiallyMatchingExpectationsMultipleUnmatchedExpectations)
{
  call2->call_was_made(1);
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  list->add_expected_call(call3);
  ExpectedCallsListForTest::ExpectedCallsList new_list;
  new_list.add_potentially_matching_expectations(*list);
  CHECK_EQUAL(2U, new_list.size());
}

TEST(ExpectedCallsList, amountOfActualCallsFulfilledFor_HasOneRelated)
{
  call1->with_name("foo");
  call1->call_was_made(1);
  call2->with_name("bar");
  call2->call_was_made(2);
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  CHECK_EQUAL(1U, list->amount_of_actual_calls_fulfilled_for("bar"));
}

TEST(ExpectedCallsList, amountOfActualCallsFulfilledFor_HasNone)
{
  call1->with_name("foo");
  call1->call_was_made(1);
  call2->with_name("bar");
  list->add_expected_call(call1);
  CHECK_EQUAL(0U, list->amount_of_actual_calls_fulfilled_for("bar"));
}

TEST(ExpectedCallsList, callToStringForUnfulfilledFunctions)
{
  call1->with_name("foo");
  call2->with_name("bar");
  call3->with_name("blah");
  call3->call_was_made(1);

  list->add_expected_call(call1);
  list->add_expected_call(call2);
  list->add_expected_call(call3);

  auto expected_string = mu::tiny::string_from_format(
      "%s\n%s", call1->call_to_string().c_str(), call2->call_to_string().c_str()
  );
  STRCMP_EQUAL(
      expected_string.c_str(), list->unfulfilled_calls_to_string().c_str()
  );
}

TEST(ExpectedCallsList, callsWithMissingParametersToString)
{
  call1->with_name("foo").with_parameter("boo", 0);
  call2->with_name("bar").with_parameter("baa", 10).with_parameter(
      "baz", "blah"
  );
  call2->input_parameter_was_passed("baa");

  list->add_expected_call(call1);
  list->add_expected_call(call2);

  auto expected_string = mu::tiny::string_from_format(
      "-%s\n-#%s\n-%s\n-#%s",
      call1->call_to_string().c_str(),
      call1->missing_parameters_to_string().c_str(),
      call2->call_to_string().c_str(),
      call2->missing_parameters_to_string().c_str()
  );
  STRCMP_EQUAL(
      expected_string.c_str(),
      list->calls_with_missing_parameters_to_string("-", "#").c_str()
  );
}

TEST(ExpectedCallsList, callToStringForFulfilledFunctions)
{
  call1->with_name("foo");
  call2->with_name("bar");

  call2->call_was_made(1);
  call1->call_was_made(2);

  list->add_expected_call(call1);
  list->add_expected_call(call2);

  auto expected_string = mu::tiny::string_from_format(
      "%s\n%s", call1->call_to_string().c_str(), call2->call_to_string().c_str()
  );
  STRCMP_EQUAL(
      expected_string.c_str(), list->fulfilled_calls_to_string().c_str()
  );
}

TEST(ExpectedCallsList, removeOneFinalizedMatchingExpectationFromEmptyList)
{
  CHECK_EQUAL(nullptr, list->remove_first_finalized_matching_expectation());
}

TEST(ExpectedCallsList, getOneMatchingExpectationFromEmptyList)
{
  CHECK_EQUAL(nullptr, list->get_first_matching_expectation());
}

TEST(ExpectedCallsList, toStringOnEmptyList)
{
  STRCMP_EQUAL("<none>", list->unfulfilled_calls_to_string().c_str());
}

TEST(ExpectedCallsList, hasFinalizedMatchingExpectations_emptyList)
{
  CHECK(!list->has_finalized_matching_expectations());
}

TEST(ExpectedCallsList, hasFinalizedMatchingExpectations_listHasNonMatchingCalls)
{
  call1->with_parameter("param", 0);
  call2->with_parameter("param", 0);
  call3->with_parameter("param", 0);
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  list->add_expected_call(call3);

  CHECK(!list->has_finalized_matching_expectations());
}

TEST(ExpectedCallsList, hasFinalizedMatchingExpectations_listHasMatchingButNotFinalizedCall)
{
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  call1->ignore_other_parameters();
  call2->with_parameter("param", 0);

  CHECK(!list->has_finalized_matching_expectations());
}

TEST(ExpectedCallsList, hasFinalizedMatchingExpectations_listHasFinalizedCallThatIgnoresParameters)
{
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  call1->ignore_other_parameters();
  call2->with_parameter("param", 0);

  call1->finalize_actual_call_match();

  CHECK(list->has_finalized_matching_expectations());
}

TEST(ExpectedCallsList, hasFinalizedMatchingExpectations_listHasFinalizedCallThatDoesntIgnoreParameters)
{
  list->add_expected_call(call1);
  list->add_expected_call(call2);
  call1->with_parameter("param", 1);
  call2->with_parameter("param", 0);

  call1->input_parameter_was_passed("param");

  CHECK(list->has_finalized_matching_expectations());
}
