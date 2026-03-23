#include "MockFailureReporterForTest.hpp"

#include "mutiny/mock/CheckedActualCall.hpp"
#include "mutiny/test.hpp"
#include "mutiny/test/TestingFixture.hpp"

using mu::tiny::mock::mock;
using mu::tiny::mock::MockCheckedActualCall;

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
  auto& actual_call = static_cast<mu::tiny::mock::MockCheckedActualCall&>(
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
      static_cast<MockCheckedActualCall&>(mock().actual_call("func"));
  actual_call.check_expectations();
  CHECK(!mock().expected_calls_left());
}

TEST(MockCall, expectAMultiCallThatDoesntHappenTheExpectedTimes)
{
  mock().expect_n_calls(2, "func");
  MockCheckedActualCall& actual_call =
      static_cast<MockCheckedActualCall&>(mock().actual_call("func"));
  actual_call.check_expectations();
  CHECK(mock().expected_calls_left());
  mock().clear();
}

TEST(MockCall, checkExpectationsClearsTheExpectations)
{
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function("foobar");
  mu::tiny::mock::MockExpectedCallsDidntHappenFailure expected_failure(
      mock_failure_test(), expectations
  );

  mock().expect_one_call("foobar");
  mock().check_expectations();

  CHECK(!mock().expected_calls_left());
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, expectOneCallInScopeButNotHappen)
{

  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function("scope::foobar");
  mu::tiny::mock::MockExpectedCallsDidntHappenFailure expected_failure(
      mock_failure_test(), expectations
  );

  mock("scope").expect_one_call("foobar");
  mock().check_expectations();

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, unexpectedCallHappened)
{
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest empty_expectations;
  mu::tiny::mock::MockUnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "func", empty_expectations
  );

  mock().actual_call("func");

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, unexpectedScopeCallHappened)
{
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest empty_expectations;
  mu::tiny::mock::MockUnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "scope::func", empty_expectations
  );

  mock("scope").actual_call("func");

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, expectOneCallInOneScopeButActualCallInAnotherScope)
{
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest empty_expectations;
  mu::tiny::mock::MockUnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "class::foo", empty_expectations
  );

  mock("scope").expect_one_call("foo");
  mock("class").actual_call("foo");

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
  mock().clear();
}

TEST(MockCall, expectOneCallInScopeButActualCallInGlobal)
{
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest empty_expectations;
  mu::tiny::mock::MockUnexpectedCallHappenedFailure expected_failure(
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
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function("foo")->call_was_made(1);
  expectations.add_function("foo")->call_was_made(2);
  mu::tiny::mock::MockUnexpectedCallHappenedFailure expected_failure(
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
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function(0, "lazy");
  mu::tiny::mock::MockUnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "lazy", expectations
  );

  mock().expect_no_call("lazy");
  mock().actual_call("lazy");

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, expectNoCallDoesntInfluenceExpectOneCall)
{
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function(0, "lazy");
  expectations.add_function("influence")->call_was_made(1);
  mu::tiny::mock::MockUnexpectedCallHappenedFailure expected_failure(
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
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function(0, "lazy");
  mu::tiny::mock::MockUnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "lazy", expectations
  );

  mock().expect_no_call("lazy");
  mock().actual_call("lazy");
  mock().actual_call("lazy");
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, ignoreOtherCallsExceptForTheUnExpectedOne)
{
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function(0, "lazy");
  mu::tiny::mock::MockUnexpectedCallHappenedFailure expected_failure(
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
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function(0, "scope::lazy");
  mu::tiny::mock::MockUnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "scope::lazy", expectations
  );

  mock("scope").expect_no_call("lazy");
  mock("scope").actual_call("lazy");

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, expectNoCallInScopeButActualCallInAnotherScope)
{
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  mu::tiny::mock::MockUnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "scope2::lazy", expectations
  );

  mock("scope1").expect_no_call("lazy");
  mock("scope2").actual_call("lazy");

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, expectNoCallInScopeButActualCallInGlobal)
{
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  mu::tiny::mock::MockUnexpectedCallHappenedFailure expected_failure(
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
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function("foo")->call_was_made(1);
  mu::tiny::mock::MockUnexpectedCallHappenedFailure expected_failure(
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
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function("foo");
  mu::tiny::mock::MockExpectedCallsDidntHappenFailure expected_failure(
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
  mock().expect_one_call("boo").with_bool_parameter("p", true);
  mock()
      .actual_call("boo")
      .on_object(object_ptr)
      .with_bool_parameter("p", true);
}

TEST(MockCall, OnObjectIgnored_InitialMatchDiscarded)
{
  void* object_ptr1 = reinterpret_cast<void*>(0x001);
  void* object_ptr2 = reinterpret_cast<void*>(0x002);

  mock().expect_one_call("boo");
  mock().expect_one_call("boo").with_bool_parameter("p", true);
  mock()
      .actual_call("boo")
      .on_object(object_ptr2)
      .with_bool_parameter("p", true);
  mock().actual_call("boo").on_object(object_ptr1);
}

TEST(MockCall, OnObjectFails)
{
  MockFailureReporterInstaller failure_reporter_installer;

  void* object_ptr = reinterpret_cast<void*>(0x001);
  void* object_ptr2 = reinterpret_cast<void*>(0x002);
  MockExpectedCallsListForTest expectations;
  expectations.add_function("boo")->on_object(object_ptr);

  mock().expect_one_call("boo").on_object(object_ptr);
  mock().actual_call("boo").on_object(object_ptr2);

  mu::tiny::mock::MockUnexpectedObjectFailure expected_failure(
      mock_failure_test(), "boo", object_ptr2, expectations
  );
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, OnObjectExpectedButNotCalled)
{
  MockFailureReporterInstaller failure_reporter_installer;

  void* object_ptr = reinterpret_cast<void*>(0x001);
  MockExpectedCallsListForTest expectations;
  expectations.add_function("boo")->on_object(object_ptr);
  expectations.add_function("boo")->on_object(object_ptr);

  mock().expect_one_call("boo").on_object(object_ptr);
  mock().expect_one_call("boo").on_object(object_ptr);
  mock().actual_call("boo");
  mock().actual_call("boo");

  mu::tiny::mock::MockExpectedObjectDidntHappenFailure expected_failure(
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
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function(2, "boo")->call_was_made(1);
  mu::tiny::mock::MockExpectedCallsDidntHappenFailure expected_failure(
      mock_failure_test(), expectations
  );

  mock().expect_n_calls(2, "boo");
  mock().actual_call("boo");
  mock().check_expectations();

  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockCall, shouldntFailTwice)
{
  MockFailureReporterInstaller failure_reporter_installer;

  mock().strict_order();
  mock().expect_one_call("foo");
  mock().expect_one_call("boo");
  mock().actual_call("boo");
  mock().actual_call("bar");
  mock().check_expectations();

  CHECK(!mu::tiny::test::string_contains(
      MockFailureReporterForTest::get_reporter()->mock_failure_string, "bar"
  ));
  CHECK(
      mu::tiny::test::string_contains(
          MockFailureReporterForTest::get_reporter()->mock_failure_string, "boo"
      )
  );
}

TEST(MockCall, shouldReturnDefaultWhenThereIsntAnythingToReturn)
{
  CHECK(mock().return_value().equals(mu::tiny::mock::MockNamedValue("")));
}

IGNORE_TEST(MockCall, testForPerformanceProfiling)
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
}

TEST(MockCall, mockExpectationShouldIncreaseNumberOfChecks)
{
  mu::tiny::test::TestTestingFixture fixture;
  fixture.set_test_function(mocks_are_counted_as_checks_test_function);
  fixture.run_all_tests();
  LONGS_EQUAL(3, fixture.get_check_count());
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
