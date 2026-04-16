#include "MockFailureReporterForTest.hpp"

#include "mutiny/mock/CheckedActualCall.hpp"

#include "mutiny/test/TestingFixture.hpp"

#include "mutiny/mock.hpp"
#include "mutiny/test.hpp"

using mu::tiny::mock::CheckedActualCall;
using mu::tiny::mock::mock;

TEST_GROUP(MockCall)
{
  void teardown() override
  {
    mock().check_expectations();
    mock().clear();
  }
};

TEST(MockCall, clear)
{
  mock().expect_one_call("func");
  mock().clear();
  CHECK(!mock().expected_calls_left());
}

TEST(MockCall, checkExpectationsDoesntFail)
{
  mock().check_expectations();
}

TEST(MockCall, expectASingleCallThatHappens)
{
  mock().expect_one_call("func");
  auto& actual_call = static_cast<mu::tiny::mock::CheckedActualCall&>(
      mock().actual_call("func")
  );
  actual_call.check_expectations();
  CHECK(!mock().expected_calls_left());
}

TEST(MockCall, expectASingleCallThatDoesntHappen)
{
  mock().expect_one_call("func");
  CHECK(mock().expected_calls_left());
  mock().clear();
}

TEST(MockCall, expectAMultiCallThatHappensTheExpectedTimes)
{
  mock().expect_n_calls(2, "func");
  mock().actual_call("func");
  auto& actual_call =
      static_cast<CheckedActualCall&>(mock().actual_call("func"));
  actual_call.check_expectations();
  CHECK(!mock().expected_calls_left());
}

TEST(MockCall, expectAMultiCallThatDoesntHappenTheExpectedTimes)
{
  mock().expect_n_calls(2, "func");
  CheckedActualCall& actual_call =
      static_cast<CheckedActualCall&>(mock().actual_call("func"));
  actual_call.check_expectations();
  CHECK(mock().expected_calls_left());
  mock().clear();
}

TEST(MockCall, checkExpectationsClearsTheExpectations)
{
  FailureReporterInstaller failure_reporter_installer;

  ExpectedCallsListForTest expectations;
  expectations.add_function("foobar");
  mu::tiny::mock::ExpectedCallsDidntHappenFailure expected_failure(
      mock_failure_test(), expectations
  );

  mock().expect_one_call("foobar");
  mock().check_expectations();

  CHECK(!mock().expected_calls_left());
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, expectOneCallInScopeButNotHappen)
{

  FailureReporterInstaller failure_reporter_installer;

  ExpectedCallsListForTest expectations;
  expectations.add_function("scope::foobar");
  mu::tiny::mock::ExpectedCallsDidntHappenFailure expected_failure(
      mock_failure_test(), expectations
  );

  mock("scope").expect_one_call("foobar");
  mock().check_expectations();

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, unexpectedCallHappened)
{
  FailureReporterInstaller failure_reporter_installer;

  ExpectedCallsListForTest empty_expectations;
  mu::tiny::mock::UnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "func", empty_expectations
  );

  mock().actual_call("func");

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, unexpectedScopeCallHappened)
{
  FailureReporterInstaller failure_reporter_installer;

  ExpectedCallsListForTest empty_expectations;
  mu::tiny::mock::UnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "scope::func", empty_expectations
  );

  mock("scope").actual_call("func");

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, expectOneCallInOneScopeButActualCallInAnotherScope)
{
  FailureReporterInstaller failure_reporter_installer;

  ExpectedCallsListForTest empty_expectations;
  mu::tiny::mock::UnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "class::foo", empty_expectations
  );

  mock("scope").expect_one_call("foo");
  mock("class").actual_call("foo");

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
  mock().clear();
}

TEST(MockCall, expectOneCallInScopeButActualCallInGlobal)
{
  FailureReporterInstaller failure_reporter_installer;

  ExpectedCallsListForTest empty_expectations;
  mu::tiny::mock::UnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "foo", empty_expectations
  );

  mock("scope").expect_one_call("foo");
  mock().actual_call("foo");

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
  mock().clear();
}

TEST(MockCall, expectMultipleSingleCallsThatHappen)
{
  mock().expect_one_call("foo");
  mock().expect_one_call("foo");
  mock().actual_call("foo");
  mock().actual_call("foo");
  mock().check_expectations();
}

TEST(MockCall, expectOneCallHoweverMultipleHappened)
{
  FailureReporterInstaller failure_reporter_installer;

  ExpectedCallsListForTest expectations;
  expectations.add_function("foo")->call_was_made(1);
  expectations.add_function("foo")->call_was_made(2);
  mu::tiny::mock::UnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "foo", expectations
  );

  mock().expect_one_call("foo");
  mock().expect_one_call("foo");
  mock().actual_call("foo");
  mock().actual_call("foo");
  mock().actual_call("foo");

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, expectNoCallThatHappened)
{
  FailureReporterInstaller failure_reporter_installer;

  ExpectedCallsListForTest expectations;
  expectations.add_function(0, "lazy");
  mu::tiny::mock::UnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "lazy", expectations
  );

  mock().expect_no_call("lazy");
  mock().actual_call("lazy");

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, expectNoCallDoesntInfluenceExpectOneCall)
{
  FailureReporterInstaller failure_reporter_installer;

  ExpectedCallsListForTest expectations;
  expectations.add_function(0, "lazy");
  expectations.add_function("influence")->call_was_made(1);
  mu::tiny::mock::UnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "lazy", expectations
  );

  mock().expect_no_call("lazy");
  mock().expect_one_call("influence");
  mock().actual_call("influence");
  mock().actual_call("lazy");

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, expectNoCallOnlyFailureOnceWhenMultipleHappened)
{
  FailureReporterInstaller failure_reporter_installer;

  ExpectedCallsListForTest expectations;
  expectations.add_function(0, "lazy");
  mu::tiny::mock::UnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "lazy", expectations
  );

  mock().expect_no_call("lazy");
  mock().actual_call("lazy");
  mock().actual_call("lazy");
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, ignoreOtherCallsExceptForTheUnExpectedOne)
{
  FailureReporterInstaller failure_reporter_installer;

  ExpectedCallsListForTest expectations;
  expectations.add_function(0, "lazy");
  mu::tiny::mock::UnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "lazy", expectations
  );

  mock().expect_no_call("lazy");
  mock().ignore_other_calls();
  mock().actual_call("bar").with_parameter("foo", 1);
  mock().actual_call("bar1").with_parameter("foo", 1);
  mock().actual_call("bar2").with_parameter("foo", 1);
  mock().actual_call("lazy");

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, expectNoCallInScopeThatHappened)
{
  FailureReporterInstaller failure_reporter_installer;

  ExpectedCallsListForTest expectations;
  expectations.add_function(0, "scope::lazy");
  mu::tiny::mock::UnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "scope::lazy", expectations
  );

  mock("scope").expect_no_call("lazy");
  mock("scope").actual_call("lazy");

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, expectNoCallInScopeButActualCallInAnotherScope)
{
  FailureReporterInstaller failure_reporter_installer;

  ExpectedCallsListForTest expectations;
  mu::tiny::mock::UnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "scope2::lazy", expectations
  );

  mock("scope1").expect_no_call("lazy");
  mock("scope2").actual_call("lazy");

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, expectNoCallInScopeButActualCallInGlobal)
{
  FailureReporterInstaller failure_reporter_installer;

  ExpectedCallsListForTest expectations;
  mu::tiny::mock::UnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "lazy", expectations
  );

  mock("scope1").expect_no_call("lazy");
  mock().actual_call("lazy");

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, ignoreOtherCallsExceptForTheExpectedOne)
{
  mock().expect_one_call("foo");
  mock().ignore_other_calls();
  mock().actual_call("bar").with_parameter("foo", 1);

  mock().clear();
}

TEST(MockCall, ignoreOtherCallsDoesntIgnoreMultipleCallsOfTheSameFunction)
{
  FailureReporterInstaller failure_reporter_installer;

  ExpectedCallsListForTest expectations;
  expectations.add_function("foo")->call_was_made(1);
  mu::tiny::mock::UnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "foo", expectations
  );

  mock().expect_one_call("foo");
  mock().ignore_other_calls();
  mock().actual_call("bar");
  mock().actual_call("foo");
  mock().actual_call("foo");

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, ignoreOtherStillFailsIfExpectedOneDidntHappen)
{
  FailureReporterInstaller failure_reporter_installer;

  ExpectedCallsListForTest expectations;
  expectations.add_function("foo");
  mu::tiny::mock::ExpectedCallsDidntHappenFailure expected_failure(
      mock_failure_test(), expectations
  );

  mock().expect_one_call("foo");
  mock().ignore_other_calls();
  mock().check_expectations();

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, threeExpectedAndActual)
{
  mock().expect_one_call("function1");
  mock().expect_one_call("function2");
  mock().expect_one_call("function3");
  mock().actual_call("function1");
  mock().actual_call("function2");
  mock().actual_call("function3");

  mock().check_expectations();
}

TEST(MockCall, disableEnable)
{
  mock().disable();
  mock().expect_one_call("function");
  mock().actual_call("differenFunction");
  CHECK(!mock().expected_calls_left());
  mock().enable();
  mock().expect_one_call("function");
  CHECK(mock().expected_calls_left());
  mock().actual_call("function");

  mock().check_expectations();
}

TEST(MockCall, OnObject)
{
  void* object_ptr = reinterpret_cast<void*>(0x001);
  mock().expect_one_call("boo").on_object(object_ptr);
  mock().actual_call("boo").on_object(object_ptr);
}

TEST(MockCall, OnObjectIgnored_MatchingAlreadyWhenObjectPassed)
{
  void* object_ptr = reinterpret_cast<void*>(0x001);
  mock().expect_one_call("boo");
  mock().actual_call("boo").on_object(object_ptr);
}

TEST(MockCall, OnObjectIgnored_NotMatchingYetWhenObjectPassed)
{
  void* object_ptr = reinterpret_cast<void*>(0x001);
  mock().expect_one_call("boo").with_parameter("p", true);
  mock().actual_call("boo").on_object(object_ptr).with_parameter("p", true);
}

TEST(MockCall, OnObjectIgnored_InitialMatchDiscarded)
{
  void* object_ptr1 = reinterpret_cast<void*>(0x001);
  void* object_ptr2 = reinterpret_cast<void*>(0x002);

  mock().expect_one_call("boo");
  mock().expect_one_call("boo").with_parameter("p", true);
  mock().actual_call("boo").on_object(object_ptr2).with_parameter("p", true);
  mock().actual_call("boo").on_object(object_ptr1);
}

TEST(MockCall, OnObjectFails)
{
  FailureReporterInstaller failure_reporter_installer;

  void* object_ptr = reinterpret_cast<void*>(0x001);
  void* object_ptr2 = reinterpret_cast<void*>(0x002);
  ExpectedCallsListForTest expectations;
  expectations.add_function("boo")->on_object(object_ptr);

  mock().expect_one_call("boo").on_object(object_ptr);
  mock().actual_call("boo").on_object(object_ptr2);

  mu::tiny::mock::UnexpectedObjectFailure expected_failure(
      mock_failure_test(), "boo", object_ptr2, expectations
  );
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, OnObjectExpectedButNotCalled)
{
  FailureReporterInstaller failure_reporter_installer;

  void* object_ptr = reinterpret_cast<void*>(0x001);
  ExpectedCallsListForTest expectations;
  expectations.add_function("boo")->on_object(object_ptr);
  expectations.add_function("boo")->on_object(object_ptr);

  mock().expect_one_call("boo").on_object(object_ptr);
  mock().expect_one_call("boo").on_object(object_ptr);
  mock().actual_call("boo");
  mock().actual_call("boo");

  mu::tiny::mock::ExpectedObjectDidntHappenFailure expected_failure(
      mock_failure_test(), "boo", expectations
  );
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
  mock().check_expectations();
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, expectNCalls_Fulfilled)
{
  mock().expect_n_calls(2, "boo");
  mock().actual_call("boo");
  mock().actual_call("boo");
  mock().check_expectations();
}

TEST(MockCall, expectNCalls_NotFulfilled)
{
  FailureReporterInstaller failure_reporter_installer;

  ExpectedCallsListForTest expectations;
  expectations.add_function(2, "boo")->call_was_made(1);
  mu::tiny::mock::ExpectedCallsDidntHappenFailure expected_failure(
      mock_failure_test(), expectations
  );

  mock().expect_n_calls(2, "boo");
  mock().actual_call("boo");
  mock().check_expectations();

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, shouldntFailTwice)
{
  FailureReporterInstaller failure_reporter_installer;

  mock().strict_order();
  mock().expect_one_call("foo");
  mock().expect_one_call("boo");
  mock().actual_call("boo");
  mock().actual_call("bar");
  mock().check_expectations();

  CHECK(!mu::tiny::string_contains(
      FailureReporterForTest::get_reporter()->mock_failure_string, "bar"
  ));
  CHECK(
      mu::tiny::string_contains(
          FailureReporterForTest::get_reporter()->mock_failure_string, "boo"
      )
  );
}

TEST(MockCall, shouldReturnDefaultWhenThereIsntAnythingToReturn)
{
  CHECK(mock().return_value().equals(mu::tiny::mock::NamedValue("")));
}

SKIPPED_TEST(MockCall, testForPerformanceProfiling)
{
  /* TO fix! */
  mock().expect_n_calls(2000, "SimpleFunction");
  for (int i = 0; i < 2000; i++) {
    mock().actual_call("SimpleFunction");
  }
}

namespace {
void mocks_are_counted_as_checks_test_function()
{
  mock().expect_one_call("foo");
  mock().expect_n_calls(3, "bar");
  mock().expect_no_call("lazy");
  mock().clear();
}
} // namespace

TEST(MockCall, mockExpectationShouldIncreaseNumberOfChecks)
{
  mu::tiny::test::TestingFixture fixture;
  fixture.set_test_function(mocks_are_counted_as_checks_test_function);
  fixture.run_all_tests();
  CHECK_EQUAL(size_t{ 3 }, fixture.get_check_count());
}

TEST(MockCall, expectationsLeftBeforCheckExpectations)
{
  CHECK(!mock().expected_calls_left());
  mock().expect_one_call("boo");
  CHECK(mock().expected_calls_left());
  mock().actual_call("boo");
  CHECK(!mock().expected_calls_left());
  mock().check_expectations();
  CHECK(!mock().expected_calls_left());
}
