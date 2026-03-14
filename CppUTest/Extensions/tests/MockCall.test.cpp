#include "MockFailureReporterForTest.hpp"

#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestTestingFixture.hpp"

using cpputest::extensions::mock;
using cpputest::extensions::MockCheckedActualCall;

TEST_GROUP(MockCall)
{
  void teardown() override
  {
    mock().checkExpectations();
    mock().clear();
  }
};

TEST(MockCall, clear)
{
  mock().expectOneCall("func");
  mock().clear();
  CHECK(!mock().expectedCallsLeft());
}

TEST(MockCall, checkExpectationsDoesntFail)
{
  mock().checkExpectations();
}

TEST(MockCall, expectASingleCallThatHappens)
{
  mock().expectOneCall("func");
  auto& actualCall = static_cast<cpputest::extensions::MockCheckedActualCall&>(
      mock().actualCall("func"));
  actualCall.checkExpectations();
  CHECK(!mock().expectedCallsLeft());
}

TEST(MockCall, expectASingleCallThatDoesntHappen)
{
  mock().expectOneCall("func");
  CHECK(mock().expectedCallsLeft());
  mock().clear();
}

TEST(MockCall, expectAMultiCallThatHappensTheExpectedTimes)
{
  mock().expectNCalls(2, "func");
  mock().actualCall("func");
  auto& actualCall =
      static_cast<MockCheckedActualCall&>(mock().actualCall("func"));
  actualCall.checkExpectations();
  CHECK(!mock().expectedCallsLeft());
}

TEST(MockCall, expectAMultiCallThatDoesntHappenTheExpectedTimes)
{
  mock().expectNCalls(2, "func");
  MockCheckedActualCall& actualCall =
      static_cast<MockCheckedActualCall&>(mock().actualCall("func"));
  actualCall.checkExpectations();
  CHECK(mock().expectedCallsLeft());
  mock().clear();
}

TEST(MockCall, checkExpectationsClearsTheExpectations)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foobar");
  cpputest::extensions::MockExpectedCallsDidntHappenFailure expectedFailure(
      mockFailureTest(), expectations);

  mock().expectOneCall("foobar");
  mock().checkExpectations();

  CHECK(!mock().expectedCallsLeft());
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCall, expectOneCallInScopeButNotHappen)
{

  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("scope::foobar");
  cpputest::extensions::MockExpectedCallsDidntHappenFailure expectedFailure(
      mockFailureTest(), expectations);

  mock("scope").expectOneCall("foobar");
  mock().checkExpectations();

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCall, unexpectedCallHappened)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest emptyExpectations;
  cpputest::extensions::MockUnexpectedCallHappenedFailure expectedFailure(
      mockFailureTest(), "func", emptyExpectations);

  mock().actualCall("func");

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCall, unexpectedScopeCallHappened)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest emptyExpectations;
  cpputest::extensions::MockUnexpectedCallHappenedFailure expectedFailure(
      mockFailureTest(), "scope::func", emptyExpectations);

  mock("scope").actualCall("func");

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCall, expectOneCallInOneScopeButActualCallInAnotherScope)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest emptyExpectations;
  cpputest::extensions::MockUnexpectedCallHappenedFailure expectedFailure(
      mockFailureTest(), "class::foo", emptyExpectations);

  mock("scope").expectOneCall("foo");
  mock("class").actualCall("foo");

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
  mock().clear();
}

TEST(MockCall, expectOneCallInScopeButActualCallInGlobal)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest emptyExpectations;
  cpputest::extensions::MockUnexpectedCallHappenedFailure expectedFailure(
      mockFailureTest(), "foo", emptyExpectations);

  mock("scope").expectOneCall("foo");
  mock().actualCall("foo");

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
  mock().clear();
}

TEST(MockCall, expectMultipleSingleCallsThatHappen)
{
  mock().expectOneCall("foo");
  mock().expectOneCall("foo");
  mock().actualCall("foo");
  mock().actualCall("foo");
  mock().checkExpectations();
}

TEST(MockCall, expectOneCallHoweverMultipleHappened)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foo")->callWasMade(1);
  expectations.addFunction("foo")->callWasMade(2);
  cpputest::extensions::MockUnexpectedCallHappenedFailure expectedFailure(
      mockFailureTest(), "foo", expectations);

  mock().expectOneCall("foo");
  mock().expectOneCall("foo");
  mock().actualCall("foo");
  mock().actualCall("foo");
  mock().actualCall("foo");

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCall, expectNoCallThatHappened)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction(0, "lazy");
  cpputest::extensions::MockUnexpectedCallHappenedFailure expectedFailure(
      mockFailureTest(), "lazy", expectations);

  mock().expectNoCall("lazy");
  mock().actualCall("lazy");

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCall, expectNoCallDoesntInfluenceExpectOneCall)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction(0, "lazy");
  expectations.addFunction("influence")->callWasMade(1);
  cpputest::extensions::MockUnexpectedCallHappenedFailure expectedFailure(
      mockFailureTest(), "lazy", expectations);

  mock().expectNoCall("lazy");
  mock().expectOneCall("influence");
  mock().actualCall("influence");
  mock().actualCall("lazy");

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCall, expectNoCallOnlyFailureOnceWhenMultipleHappened)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction(0, "lazy");
  cpputest::extensions::MockUnexpectedCallHappenedFailure expectedFailure(
      mockFailureTest(), "lazy", expectations);

  mock().expectNoCall("lazy");
  mock().actualCall("lazy");
  mock().actualCall("lazy");
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCall, ignoreOtherCallsExceptForTheUnExpectedOne)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction(0, "lazy");
  cpputest::extensions::MockUnexpectedCallHappenedFailure expectedFailure(
      mockFailureTest(), "lazy", expectations);

  mock().expectNoCall("lazy");
  mock().ignoreOtherCalls();
  mock().actualCall("bar").withParameter("foo", 1);
  mock().actualCall("bar1").withParameter("foo", 1);
  mock().actualCall("bar2").withParameter("foo", 1);
  mock().actualCall("lazy");

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCall, expectNoCallInScopeThatHappened)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction(0, "scope::lazy");
  cpputest::extensions::MockUnexpectedCallHappenedFailure expectedFailure(
      mockFailureTest(), "scope::lazy", expectations);

  mock("scope").expectNoCall("lazy");
  mock("scope").actualCall("lazy");

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCall, expectNoCallInScopeButActualCallInAnotherScope)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  cpputest::extensions::MockUnexpectedCallHappenedFailure expectedFailure(
      mockFailureTest(), "scope2::lazy", expectations);

  mock("scope1").expectNoCall("lazy");
  mock("scope2").actualCall("lazy");

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCall, expectNoCallInScopeButActualCallInGlobal)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  cpputest::extensions::MockUnexpectedCallHappenedFailure expectedFailure(
      mockFailureTest(), "lazy", expectations);

  mock("scope1").expectNoCall("lazy");
  mock().actualCall("lazy");

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCall, ignoreOtherCallsExceptForTheExpectedOne)
{
  mock().expectOneCall("foo");
  mock().ignoreOtherCalls();
  mock().actualCall("bar").withParameter("foo", 1);

  mock().clear();
}

TEST(MockCall, ignoreOtherCallsDoesntIgnoreMultipleCallsOfTheSameFunction)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foo")->callWasMade(1);
  cpputest::extensions::MockUnexpectedCallHappenedFailure expectedFailure(
      mockFailureTest(), "foo", expectations);

  mock().expectOneCall("foo");
  mock().ignoreOtherCalls();
  mock().actualCall("bar");
  mock().actualCall("foo");
  mock().actualCall("foo");

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCall, ignoreOtherStillFailsIfExpectedOneDidntHappen)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction("foo");
  cpputest::extensions::MockExpectedCallsDidntHappenFailure expectedFailure(
      mockFailureTest(), expectations);

  mock().expectOneCall("foo");
  mock().ignoreOtherCalls();
  mock().checkExpectations();

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCall, threeExpectedAndActual)
{
  mock().expectOneCall("function1");
  mock().expectOneCall("function2");
  mock().expectOneCall("function3");
  mock().actualCall("function1");
  mock().actualCall("function2");
  mock().actualCall("function3");

  mock().checkExpectations();
}

TEST(MockCall, disableEnable)
{
  mock().disable();
  mock().expectOneCall("function");
  mock().actualCall("differenFunction");
  CHECK(!mock().expectedCallsLeft());
  mock().enable();
  mock().expectOneCall("function");
  CHECK(mock().expectedCallsLeft());
  mock().actualCall("function");

  mock().checkExpectations();
}

TEST(MockCall, OnObject)
{
  void* objectPtr = reinterpret_cast<void*>(0x001);
  mock().expectOneCall("boo").onObject(objectPtr);
  mock().actualCall("boo").onObject(objectPtr);
}

TEST(MockCall, OnObjectIgnored_MatchingAlreadyWhenObjectPassed)
{
  void* objectPtr = reinterpret_cast<void*>(0x001);
  mock().expectOneCall("boo");
  mock().actualCall("boo").onObject(objectPtr);
}

TEST(MockCall, OnObjectIgnored_NotMatchingYetWhenObjectPassed)
{
  void* objectPtr = reinterpret_cast<void*>(0x001);
  mock().expectOneCall("boo").withBoolParameter("p", true);
  mock().actualCall("boo").onObject(objectPtr).withBoolParameter("p", true);
}

TEST(MockCall, OnObjectIgnored_InitialMatchDiscarded)
{
  void* objectPtr1 = reinterpret_cast<void*>(0x001);
  void* objectPtr2 = reinterpret_cast<void*>(0x002);

  mock().expectOneCall("boo");
  mock().expectOneCall("boo").withBoolParameter("p", true);
  mock().actualCall("boo").onObject(objectPtr2).withBoolParameter("p", true);
  mock().actualCall("boo").onObject(objectPtr1);
}

TEST(MockCall, OnObjectFails)
{
  MockFailureReporterInstaller failureReporterInstaller;

  void* objectPtr = reinterpret_cast<void*>(0x001);
  void* objectPtr2 = reinterpret_cast<void*>(0x002);
  MockExpectedCallsListForTest expectations;
  expectations.addFunction("boo")->onObject(objectPtr);

  mock().expectOneCall("boo").onObject(objectPtr);
  mock().actualCall("boo").onObject(objectPtr2);

  cpputest::extensions::MockUnexpectedObjectFailure expectedFailure(
      mockFailureTest(), "boo", objectPtr2, expectations);
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCall, OnObjectExpectedButNotCalled)
{
  MockFailureReporterInstaller failureReporterInstaller;

  void* objectPtr = reinterpret_cast<void*>(0x001);
  MockExpectedCallsListForTest expectations;
  expectations.addFunction("boo")->onObject(objectPtr);
  expectations.addFunction("boo")->onObject(objectPtr);

  mock().expectOneCall("boo").onObject(objectPtr);
  mock().expectOneCall("boo").onObject(objectPtr);
  mock().actualCall("boo");
  mock().actualCall("boo");

  cpputest::extensions::MockExpectedObjectDidntHappenFailure expectedFailure(
      mockFailureTest(), "boo", expectations);
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
  mock().checkExpectations();
  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCall, expectNCalls_Fulfilled)
{
  mock().expectNCalls(2, "boo");
  mock().actualCall("boo");
  mock().actualCall("boo");
  mock().checkExpectations();
}

TEST(MockCall, expectNCalls_NotFulfilled)
{
  MockFailureReporterInstaller failureReporterInstaller;

  MockExpectedCallsListForTest expectations;
  expectations.addFunction(2, "boo")->callWasMade(1);
  cpputest::extensions::MockExpectedCallsDidntHappenFailure expectedFailure(
      mockFailureTest(), expectations);

  mock().expectNCalls(2, "boo");
  mock().actualCall("boo");
  mock().checkExpectations();

  CHECK_EXPECTED_MOCK_FAILURE(expectedFailure);
}

TEST(MockCall, shouldntFailTwice)
{
  MockFailureReporterInstaller failureReporterInstaller;

  mock().strictOrder();
  mock().expectOneCall("foo");
  mock().expectOneCall("boo");
  mock().actualCall("boo");
  mock().actualCall("bar");
  mock().checkExpectations();

  CHECK(!stringContains(
      MockFailureReporterForTest::getReporter()->mockFailureString, "bar"));
  CHECK(stringContains(
      MockFailureReporterForTest::getReporter()->mockFailureString, "boo"));
}

TEST(MockCall, shouldReturnDefaultWhenThereIsntAnythingToReturn)
{
  CHECK(mock().returnValue().equals(cpputest::extensions::MockNamedValue("")));
}

IGNORE_TEST(MockCall, testForPerformanceProfiling)
{
  /* TO fix! */
  mock().expectNCalls(2000, "SimpleFunction");
  for (int i = 0; i < 2000; i++) {
    mock().actualCall("SimpleFunction");
  }
}

static void
mocksAreCountedAsChecksTestFunction_()
{
  mock().expectOneCall("foo");
  mock().expectNCalls(3, "bar");
  mock().expectNoCall("lazy");
  mock().clear();
}

TEST(MockCall, mockExpectationShouldIncreaseNumberOfChecks)
{
  cpputest::TestTestingFixture fixture;
  fixture.setTestFunction(mocksAreCountedAsChecksTestFunction_);
  fixture.runAllTests();
  LONGS_EQUAL(3, fixture.getCheckCount());
}

TEST(MockCall, expectationsLeftBeforCheckExpectations)
{
  CHECK(!mock().expectedCallsLeft());
  mock().expectOneCall("boo");
  CHECK(mock().expectedCallsLeft());
  mock().actualCall("boo");
  CHECK(!mock().expectedCallsLeft());
  mock().checkExpectations();
  CHECK(!mock().expectedCallsLeft());
}
