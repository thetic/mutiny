#include "MockFailureReporterForTest.hpp"

#include "CppMu/CppMu.hpp"

using cppmu::mock;

TEST_GROUP(MockStrictOrder)
{
  void teardown() override { mock().clear(); }
};

TEST(MockStrictOrder, OrderObserved)
{
  mock().strict_order();

  mock().expect_one_call("foo1");
  mock().expect_one_call("foo2");
  mock().actual_call("foo1");
  mock().actual_call("foo2");

  mock().check_expectations();
}

TEST(MockStrictOrder, someOrderObserved)
{
  mock().expect_one_call("foo3").with_call_order(3);
  mock().expect_one_call("foo1");
  mock().expect_one_call("foo2");
  mock().actual_call("foo2");
  mock().actual_call("foo1");
  mock().actual_call("foo3");

  mock().check_expectations();
}

TEST(MockStrictOrder, orderViolated)
{
  MockFailureReporterInstaller failure_reporter_installer;
  mock().strict_order();

  MockExpectedCallsListForTest expectations;
  expectations.add_function_ordered("foo1", 1)->call_was_made(1);
  expectations.add_function_ordered("foo1", 2)->call_was_made(3);
  expectations.add_function_ordered("foo2", 3)->call_was_made(2);
  cppmu::MockCallOrderFailure expected_failure(
      mock_failure_test(), expectations
  );

  mock().expect_one_call("foo1");
  mock().expect_one_call("foo1");
  mock().expect_one_call("foo2");
  mock().actual_call("foo1");
  mock().actual_call("foo2");
  mock().actual_call("foo1");

  mock().check_expectations();
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockStrictOrder, orderViolatedWorksHierarchically)
{
  MockFailureReporterInstaller failure_reporter_installer;
  mock().strict_order();
  mock("bla").strict_order();

  MockExpectedCallsListForTest expectations;
  expectations.add_function_ordered("foo::foo1", 1)->call_was_made(2);
  expectations.add_function_ordered("foo::foo2", 2)->call_was_made(1);
  cppmu::MockCallOrderFailure expected_failure(
      mock_failure_test(), expectations
  );

  mock("bla").expect_one_call("foo1");
  mock("foo").expect_one_call("foo1");
  mock("foo").expect_one_call("foo2");

  mock("bla").actual_call("foo1");
  mock("foo").actual_call("foo2");
  mock("foo").actual_call("foo1");

  mock().check_expectations();
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockStrictOrder, orderViolatedWorksWithExtraUnexpectedCall)
{
  MockFailureReporterInstaller failure_reporter_installer;
  mock().strict_order();
  mock("bla").strict_order();
  mock().ignore_other_calls();

  MockExpectedCallsListForTest expectations;
  expectations.add_function_ordered("foo::foo1", 1)->call_was_made(2);
  expectations.add_function_ordered("foo::foo2", 2)->call_was_made(1);
  cppmu::MockCallOrderFailure expected_failure(
      mock_failure_test(), expectations
  );

  mock("bla").expect_one_call("foo1");
  mock("foo").expect_one_call("foo1");
  mock("foo").expect_one_call("foo2");

  mock("bla").actual_call("foo1");
  mock("foo").actual_call("foo2");
  mock("foo").actual_call("unexpected1");
  mock("foo").actual_call("foo1");
  mock("foo").actual_call("unexpected2");

  mock().check_expectations();
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockStrictOrder, orderViolatedWithinAScope)
{
  MockFailureReporterInstaller failure_reporter_installer;
  mock().strict_order();

  MockExpectedCallsListForTest expectations;
  expectations.add_function_ordered("scope::foo1", 1)->call_was_made(2);
  expectations.add_function_ordered("scope::foo2", 2)->call_was_made(1);
  cppmu::MockCallOrderFailure expected_failure(
      mock_failure_test(), expectations
  );

  mock("scope").expect_one_call("foo1");
  mock("scope").expect_one_call("foo2");
  mock("scope").actual_call("foo2");
  mock("scope").actual_call("foo1");

  mock().check_expectations();
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockStrictOrder, orderNotViolatedAcrossScopes)
{
  mock("mock1").strict_order();
  mock("mock2").strict_order();

  mock("mock1").expect_one_call("foo1");
  mock("mock2").expect_one_call("foo2");
  mock("mock1").actual_call("foo1");
  mock("mock2").actual_call("foo2");

  mock("mock1").check_expectations();
  mock("mock2").check_expectations();
}

TEST(MockStrictOrder, orderViolatedAcrossScopes)
{
  mock("mock1").strict_order();
  mock("mock2").strict_order();

  mock("mock1").expect_one_call("foo1");
  mock("mock2").expect_one_call("foo2");
  mock("mock2").actual_call("foo2");
  mock("mock1").actual_call("foo1");

  mock("mock1").check_expectations();
  mock("mock2").check_expectations();
}

TEST(MockStrictOrder, orderUsingNCalls)
{
  mock().strict_order();

  mock().expect_one_call("foo1");
  mock().expect_n_calls(2, "foo2");
  mock().expect_one_call("foo1");
  mock().expect_n_calls(3, "foo2");

  mock().actual_call("foo1");
  mock().actual_call("foo2");
  mock().actual_call("foo2");
  mock().actual_call("foo1");
  mock().actual_call("foo2");
  mock().actual_call("foo2");
  mock().actual_call("foo2");

  mock().check_expectations();
}
