#include "MockFailureReporterForTest.hpp"

#include "CppMu/CppMu.hpp"

using cppmu::mock;

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

  LONGS_EQUAL(0, mock("other").expected_calls_left());
  LONGS_EQUAL(1, mock("first").expected_calls_left());

  mock("first").clear();
}

TEST(MockHierarchy, EnableDisableWorkHierarchically)
{
  mock("first");

  mock().disable();
  mock("first").expect_one_call("boo");
  LONGS_EQUAL(0, mock("first").expected_calls_left());

  mock().enable();
  mock("first").expect_one_call("boo");
  LONGS_EQUAL(1, mock("first").expected_calls_left());

  mock("first").clear();
}

TEST(MockHierarchy, EnableDisableWorkHierarchicallyWhenSupportIsDynamicallyCreated)
{
  mock().disable();
  mock("first").expect_one_call("boo");
  LONGS_EQUAL(0, mock("first").expected_calls_left());

  mock().enable();
  mock("second").expect_one_call("boo");
  LONGS_EQUAL(1, mock("second").expected_calls_left());

  mock().clear();
}

TEST(MockHierarchy, ExpectedCallsLeftWorksHierarchically)
{
  mock("first").expect_one_call("foobar");
  LONGS_EQUAL(1, mock().expected_calls_left());

  mock().clear();
}

TEST(MockHierarchy, checkExpectationsWorksHierarchically)
{
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function("first::foobar");
  expectations.add_function("second::helloworld");
  cppmu::MockExpectedCallsDidntHappenFailure expected_failure(
      mock_failure_test(), expectations
  );

  mock("first").expect_one_call("foobar");
  mock("second").expect_one_call("helloworld");

  mock().check_expectations();
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
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
  MockFailureReporterInstaller failure_reporter_installer;

  MockExpectedCallsListForTest expectations;
  expectations.add_function("first::foobar")->with_parameter("boo", 1);
  cppmu::MockExpectedParameterDidntHappenFailure expected_failure(
      mock_failure_test(), "first::foobar", expectations, expectations
  );

  mock("first").expect_one_call("foobar").with_parameter("boo", 1);
  mock("first").actual_call("foobar");

  mock().check_expectations();
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}

TEST(MockHierarchy, reporterIsInheritedInHierarchicalMocks)
{
  MockFailureReporterInstaller failure_reporter_installer;
  MockExpectedCallsListForTest expectations;

  mock("differentScope").actual_call("foobar");

  cppmu::MockUnexpectedCallHappenedFailure expected_failure(
      mock_failure_test(), "differentScope::foobar", expectations
  );
  CHECK_EXPECTED_MOCK_FAILURE(expected_failure);
}
