#include "MockFailureReporterForTest.h"

#include "CppUTest/TestHarness.hpp"

using namespace cpputest;
using namespace cpputest::extensions;

TEST_GROUP(MockHierarchy)
{
  void teardown() override { mock().clear(); }
};

TEST(MockHierarchy, getMockSupportScope)
{
  MockSupport* mock1 = mock().getMockSupportScope("name");
  MockSupport* mock2 = mock().getMockSupportScope("differentName");

  CHECK(!mock().hasData("name"));
  CHECK(mock1 != mock2);
  POINTERS_EQUAL(mock1, mock().getMockSupportScope("name"));
  CHECK(mock1 != &mock());
}

TEST(MockHierarchy, usingTwoMockSupportsByName)
{
  mock("first").expectOneCall("boo");

  LONGS_EQUAL(0, mock("other").expectedCallsLeft());
  LONGS_EQUAL(1, mock("first").expectedCallsLeft());

  mock("first").clear();
}

TEST(MockHierarchy, EnableDisableWorkHierarchically)
{
  mock("first");

  mock().disable();
  mock("first").expectOneCall("boo");
  LONGS_EQUAL(0, mock("first").expectedCallsLeft());

  mock().enable();
  mock("first").expectOneCall("boo");
  LONGS_EQUAL(1, mock("first").expectedCallsLeft());

  mock("first").clear();
}

TEST(MockHierarchy,
    EnableDisableWorkHierarchicallyWhenSupportIsDynamicallyCreated)
{
  mock().disable();
  mock("first").expectOneCall("boo");
  LONGS_EQUAL(0, mock("first").expectedCallsLeft());

  mock().enable();
  mock("second").expectOneCall("boo");
  LONGS_EQUAL(1, mock("second").expectedCallsLeft());

  mock().clear();
}

TEST(MockHierarchy, ExpectedCallsLeftWorksHierarchically)
{
  mock("first").expectOneCall("foobar");
  LONGS_EQUAL(1, mock().expectedCallsLeft());

  mock().clear();
}

TEST(MockHierarchy, checkExpectationsWorksHierarchically)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("first::foobar");
  expectations.addFunction("second::helloworld");
  MockExpectedCallsDidntHappenFailure expectedFailure(
      mockFailureTest(), expectations);

  mock("first").expectOneCall("foobar");
  mock("second").expectOneCall("helloworld");

  mock().checkExpectations();
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockHierarchy, ignoreOtherCallsWorksHierarchically)
{
  mock("first");
  mock().ignoreOtherCalls();
  mock("first").actualCall("boo");

  mock().checkExpectations();
}

TEST(MockHierarchy, ignoreOtherCallsWorksHierarchicallyWhenDynamicallyCreated)
{
  mock().ignoreOtherCalls();
  mock("first").actualCall("boo");

  mock().checkExpectations();
}

TEST(MockHierarchy, checkExpectationsWorksHierarchicallyForLastCallNotFinished)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("first::foobar")->withParameter("boo", 1);
  MockExpectedParameterDidntHappenFailure expectedFailure(
      mockFailureTest(), "first::foobar", expectations, expectations);

  mock("first").expectOneCall("foobar").withParameter("boo", 1);
  mock("first").actualCall("foobar");

  mock().checkExpectations();
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockHierarchy, reporterIsInheritedInHierarchicalMocks)
{
  MockFailureReporterInstaller failureReporterInstaller;
  MockExpectedCallsListForTest expectations;

  mock("differentScope").actualCall("foobar");

  MockUnexpectedCallHappenedFailure expectedFailure(
      mockFailureTest(), "differentScope::foobar", expectations);
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}
