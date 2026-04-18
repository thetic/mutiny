#include "MockFailureReporter.hpp"

#include "mu/tiny/test.hpp"

using mu::tiny::mock::mock;

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
  FailureReporterInstaller failure_reporter_installer;
  mock().strict_order();

  mock().expect_one_call("foo1");
  mock().expect_one_call("foo1");
  mock().expect_one_call("foo2");
  mock().actual_call("foo1");
  mock().actual_call("foo2");
  mock().actual_call("foo1");

  mock().check_expectations();
  STRCMP_CONTAINS("foo", mock_failure_string().c_str());
}

TEST(MockStrictOrder, orderViolatedWorksHierarchically)
{
  FailureReporterInstaller failure_reporter_installer;
  mock().strict_order();
  mock("bla").strict_order();

  mock("bla").expect_one_call("foo1");
  mock("foo").expect_one_call("foo1");
  mock("foo").expect_one_call("foo2");

  mock("bla").actual_call("foo1");
  mock("foo").actual_call("foo2");
  mock("foo").actual_call("foo1");

  mock().check_expectations();
  STRCMP_CONTAINS("foo::foo", mock_failure_string().c_str());
}

TEST(MockStrictOrder, orderViolatedWorksWithExtraUnexpectedCall)
{
  FailureReporterInstaller failure_reporter_installer;
  mock().strict_order();
  mock("bla").strict_order();
  mock().ignore_other_calls();

  mock("bla").expect_one_call("foo1");
  mock("foo").expect_one_call("foo1");
  mock("foo").expect_one_call("foo2");

  mock("bla").actual_call("foo1");
  mock("foo").actual_call("foo2");
  mock("foo").actual_call("unexpected1");
  mock("foo").actual_call("foo1");
  mock("foo").actual_call("unexpected2");

  mock().check_expectations();
  STRCMP_CONTAINS("foo::foo", mock_failure_string().c_str());
}

TEST(MockStrictOrder, orderViolatedWithinAScope)
{
  FailureReporterInstaller failure_reporter_installer;
  mock().strict_order();

  mock("scope").expect_one_call("foo1");
  mock("scope").expect_one_call("foo2");
  mock("scope").actual_call("foo2");
  mock("scope").actual_call("foo1");

  mock().check_expectations();
  STRCMP_CONTAINS("scope::foo", mock_failure_string().c_str());
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
