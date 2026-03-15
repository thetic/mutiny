#include "CppUTest/Extensions/MockSupport.hpp"

#include "MockFailureReporterForTest.hpp"

#include "CppUTest/Extensions/MockExpectedCall.hpp"
#include "CppUTest/Extensions/MockFailure.hpp"
#include "CppUTest/Extensions/OrderedTest.hpp"

#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestTestingFixture.hpp"

using cpputest::extensions::mock;

namespace {
bool cpputestHasCrashed;

void
crashMethod()
{
  cpputestHasCrashed = true;
}

void
unexpectedCallTestFunction_(void)
{
  mock().actualCall("unexpected");
}

void
CHECK_EXPECTED_MOCK_FAILURE_LOCATION_failedTestMethod_()
{
  MockExpectedCallsListForTest::MockExpectedCallsList list;
  cpputest::extensions::MockUnexpectedCallHappenedFailure expectedFailure(
      cpputest::TestShell::getCurrent(), "unexpected", list);
  mock().actualCall("boo");
  CHECK_EXPECTED_MOCK_FAILURE_LOCATION(expectedFailure, "file", 1);
}

void
CHECK_NO_MOCK_FAILURE_LOCATION_failedTestMethod_()
{
  mock().actualCall("boo");
  CHECK_NO_MOCK_FAILURE_LOCATION("file", 1);
}

}

TEST_GROUP(MockSupport)
{
  MockExpectedCallsListForTest expectations;
  MockFailureReporterInstaller failureReporterInstaller;

  void teardown() override
  {
    mock().checkExpectations();
    CHECK_NO_MOCK_FAILURE();
    MockFailureReporterForTest::clearReporter();
    mock().clear();
  }
};

TEST(MockSupport, setDataForUnsignedIntegerValues)
{
  unsigned int expected_data = 7;
  mock().setData("data", expected_data);
  LONGS_EQUAL(expected_data, mock().getData("data").getUnsignedIntValue());
}

TEST(MockSupport, setDataForIntegerValues)
{
  int expected_data = 10;
  mock().setData("data", expected_data);
  LONGS_EQUAL(expected_data, mock().getData("data").getIntValue());
}

TEST(MockSupport, setDataForBooleanValues)
{
  bool expected_data = true;
  mock().setData("data", expected_data);
  CHECK_EQUAL(expected_data, mock().getData("data").getBoolValue());
}

TEST(MockSupport, hasDataBeenSet)
{
  CHECK(!mock().hasData("data"));
  mock().setData("data", 10);
  CHECK(mock().hasData("data"));
}

TEST(MockSupport, dataCanBeChanged)
{
  mock().setData("data", 10);
  mock().setData("data", 15);
  LONGS_EQUAL(15, mock().getData("data").getIntValue());
}

TEST(MockSupport, uninitializedData)
{
  LONGS_EQUAL(0, mock().getData("nonexisting").getIntValue());
  STRCMP_EQUAL("int", mock().getData("nonexisting").getType().c_str());
}

TEST(MockSupport, setMultipleData)
{
  mock().setData("data", 1);
  mock().setData("data2", 10);
  LONGS_EQUAL(1, mock().getData("data").getIntValue());
  LONGS_EQUAL(10, mock().getData("data2").getIntValue());
}

TEST(MockSupport, setDataString)
{
  mock().setData("data", "string");
  STRCMP_EQUAL("string", mock().getData("data").getStringValue());
}

TEST(MockSupport, setDataDouble)
{
  mock().setData("data", 1.0);
  DOUBLES_EQUAL(1.0, mock().getData("data").getDoubleValue(), 0.05);
}

TEST(MockSupport, setDataLongInt)
{
  long int i = 100;
  mock().setData("data", i);
  LONGS_EQUAL(i, mock().getData("data").getLongIntValue());
}

TEST(MockSupport, setDataUnsignedLongInt)
{
  unsigned long int i = 100;
  mock().setData("data", i);
  UNSIGNED_LONGS_EQUAL(i, mock().getData("data").getUnsignedLongIntValue());
}

TEST(MockSupport, setDataPointer)
{
  void* ptr = reinterpret_cast<void*>(0x001);
  mock().setData("data", ptr);
  POINTERS_EQUAL(ptr, mock().getData("data").getPointerValue());
}

TEST(MockSupport, setConstDataPointer)
{
  const void* ptr = reinterpret_cast<const void*>(0x001);
  mock().setData("data", ptr);
  POINTERS_EQUAL(ptr, mock().getData("data").getConstPointerValue());
}

TEST(MockSupport, setDataFunctionPointer)
{
  void (*ptr)() = reinterpret_cast<void (*)()>(0x001);
  mock().setData("data", ptr);
  FUNCTIONPOINTERS_EQUAL(ptr, mock().getData("data").getFunctionPointerValue());
}

TEST(MockSupport, setDataObject)
{
  void* ptr = reinterpret_cast<void*>(0x001);
  mock().setDataObject("data", "type", ptr);
  POINTERS_EQUAL(ptr, mock().getData("data").getObjectPointer());
  STRCMP_EQUAL("type", mock().getData("data").getType().c_str());
}

TEST(MockSupport, setDataConstObject)
{
  void* ptr = reinterpret_cast<void*>(0x011);
  mock().setDataConstObject("data", "type", ptr);
  POINTERS_EQUAL(ptr, mock().getData("data").getConstObjectPointer());
  STRCMP_EQUAL("type", mock().getData("data").getType().c_str());
}

TEST(MockSupport, tracing)
{
  mock().tracing(true);

  mock()
      .actualCall("boo")
      .withParameter("double", 1.0)
      .withParameter("int", 1)
      .withParameter("string", "string");
  mock("scope")
      .actualCall("foo")
      .withParameter("double", 1.0)
      .withParameter("int", 1)
      .withParameter("string", "string");
  mock().checkExpectations();

  STRCMP_CONTAINS("boo", mock().getTraceOutput());
  STRCMP_CONTAINS("foo", mock().getTraceOutput());
}

TEST(MockSupport, tracingWorksHierarchically)
{
  mock("scope").tracing(true);
  mock().tracing(true);

  mock().actualCall("boo");
  mock("scope").actualCall("foo");
  mock().checkExpectations();

  STRCMP_CONTAINS("boo", mock().getTraceOutput());
  STRCMP_CONTAINS("foo", mock().getTraceOutput());
}

TEST_GROUP(MockSupportWithFixture)
{
  cpputest::TestTestingFixture fixture;

  void teardown() override
  {
    mock().clear();
    MockFailureReporterForTest::clearReporter();
  }
};

TEST(MockSupportWithFixture, CHECK_EXPECTED_MOCK_FAILURE_LOCATION_failed)
{
  mock().setMockFailureStandardReporter(
      MockFailureReporterForTest::getReporter());
  fixture.setTestFunction(
      CHECK_EXPECTED_MOCK_FAILURE_LOCATION_failedTestMethod_);
  fixture.runAllTests();
  fixture.assertPrintContains("MockFailures are different.");
  fixture.assertPrintContains("Expected MockFailure:");
  fixture.assertPrintContains(
      "Mock Failure: Unexpected call to function: unexpected");
  fixture.assertPrintContains("Actual MockFailure:");
  fixture.assertPrintContains("Mock Failure: Unexpected call to function: boo");
}

TEST(MockSupportWithFixture, CHECK_NO_MOCK_FAILURE_LOCATION_failed)
{
  mock().setMockFailureStandardReporter(
      MockFailureReporterForTest::getReporter());
  fixture.setTestFunction(CHECK_NO_MOCK_FAILURE_LOCATION_failedTestMethod_);
  fixture.runAllTests();
  fixture.assertPrintContains("Unexpected mock failure:");
  fixture.assertPrintContains("Mock Failure: Unexpected call to function: boo");
}

TEST(MockSupportWithFixture, shouldCrashOnFailure)
{
  cpputestHasCrashed = false;
  mock().crashOnFailure(true);
  cpputest::TestShell::setCrashMethod(crashMethod);
  fixture.setTestFunction(unexpectedCallTestFunction_);

  fixture.runAllTests();

  CHECK(cpputestHasCrashed);

  mock().crashOnFailure(false);
  cpputest::TestShell::resetCrashMethod();
}

TEST(MockSupportWithFixture, ShouldNotCrashOnFailureAfterCrashMethodWasReset)
{
  cpputestHasCrashed = false;
  cpputest::TestShell::setCrashMethod(crashMethod);
  fixture.setTestFunction(unexpectedCallTestFunction_);
  cpputest::TestShell::resetCrashMethod();

  fixture.runAllTests();

  fixture.assertPrintContains("Unexpected call to function: unexpected");
  CHECK_FALSE(cpputestHasCrashed);
}

TEST(MockSupportWithFixture, shouldCrashOnFailureWithCppUTestSetting)
{
  cpputestHasCrashed = false;
  cpputest::TestShell::setCrashOnFail();
  cpputest::TestShell::setCrashMethod(crashMethod);
  fixture.setTestFunction(unexpectedCallTestFunction_);

  fixture.runAllTests();

  CHECK(cpputestHasCrashed);

  cpputest::TestShell::restoreDefaultTestTerminator();
  cpputest::TestShell::resetCrashMethod();
}

TEST(MockSupportWithFixture, failedMockShouldFailAgainWhenRepeated)
{
  fixture.setTestFunction(unexpectedCallTestFunction_);
  int repeatCount = 2;
  while (repeatCount--) {
    fixture.runAllTests();
    fixture.assertPrintContains("Unexpected call to function: unexpected");
    fixture.assertPrintContains("Errors (1 failures, 1 tests, 1 ran, 0 checks, "
                                "0 ignored, 0 filtered out");
    fixture.flushOutputAndResetResult();
  }
}
