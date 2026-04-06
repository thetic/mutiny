#include "MockFailureReporter.hpp"

#include "mutiny/test.hpp"

using mu::tiny::mock::mock;

TEST_GROUP(MockHierarchy)
{
  void teardown() override { mock().clear(); }
};

TEST(MockHierarchy, getMockSupportScope)
{
  auto* mock1 = mock().get_mock_support_scope("name");
  auto* mock2 = mock().get_mock_support_scope("differentName");

  CHECK(!mock().has_data("name"));
  CHECK(mock1 != mock2);
  POINTERS_EQUAL(mock1, mock().get_mock_support_scope("name"));
  CHECK(mock1 != &mock());
}

TEST(MockHierarchy, usingTwoMockSupportsByName)
{
  mock("first").expect_one_call("boo");

  CHECK_FALSE(mock("other").expected_calls_left());
  CHECK_TRUE(mock("first").expected_calls_left());

  mock("first").clear();
}

TEST(MockHierarchy, EnableDisableWorkHierarchically)
{
  mock("first");

  mock().disable();
  mock("first").expect_one_call("boo");
  CHECK_FALSE(mock("first").expected_calls_left());

  mock().enable();
  mock("first").expect_one_call("boo");
  CHECK_TRUE(mock("first").expected_calls_left());

  mock("first").clear();
}

TEST(MockHierarchy, EnableDisableWorkHierarchicallyWhenSupportIsDynamicallyCreated)
{
  mock().disable();
  mock("first").expect_one_call("boo");
  CHECK_FALSE(mock("first").expected_calls_left());

  mock().enable();
  mock("second").expect_one_call("boo");
  CHECK_TRUE(mock("second").expected_calls_left());

  mock().clear();
}

TEST(MockHierarchy, ExpectedCallsLeftWorksHierarchically)
{
  mock("first").expect_one_call("foobar");
  CHECK_TRUE(mock().expected_calls_left());

  mock().clear();
}

TEST(MockHierarchy, checkExpectationsWorksHierarchically)
{
  FailureReporterInstaller failure_reporter_installer;

  mock("first").expect_one_call("foobar");
  mock("second").expect_one_call("helloworld");

  mock().check_expectations();
  STRCMP_CONTAINS("first::foobar", mock_failure_string().c_str());
}

TEST(MockHierarchy, ignoreOtherCallsWorksHierarchically)
{
  mock("first");
  mock().ignore_other_calls();
  mock("first").actual_call("boo");

  mock().check_expectations();
}

TEST(MockHierarchy, ignoreOtherCallsWorksHierarchicallyWhenDynamicallyCreated)
{
  mock().ignore_other_calls();
  mock("first").actual_call("boo");

  mock().check_expectations();
}

TEST(MockHierarchy, checkExpectationsWorksHierarchicallyForLastCallNotFinished)
{
  FailureReporterInstaller failure_reporter_installer;

  mock("first").expect_one_call("foobar").with_parameter("boo", 1);
  mock("first").actual_call("foobar");

  mock().check_expectations();
  STRCMP_CONTAINS("first::foobar", mock_failure_string().c_str());
}

TEST(MockHierarchy, reporterIsInheritedInHierarchicalMocks)
{
  FailureReporterInstaller failure_reporter_installer;

  mock("differentScope").actual_call("foobar");

  STRCMP_CONTAINS("differentScope::foobar", mock_failure_string().c_str());
}
