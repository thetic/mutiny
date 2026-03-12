#include "MockFailureReporterForTest.h"

#include "CppUTest/TestHarness.hpp"

using cpputest::extensions::mock;

void
MockFailureReporterForTest::failTest(cpputest::extensions::MockFailure failure)
{
  mockFailureString = failure.getMessage();
}

void
MockFailureReporterForTest::reportFailure(
    const cpputest::extensions::MockFailure& failure)
{
  mockFailureString = failure.getMessage();
}

void
MockFailureReporterForTest::exitTest()
{
  // suppress exit in test-spy mode
}

MockFailureReporterForTest* MockFailureReporterForTest::instance_ = nullptr;

MockFailureReporterForTest*
MockFailureReporterForTest::getReporter()
{
  if (instance_ == nullptr)
    instance_ = new MockFailureReporterForTest;

  return instance_;
}

void
MockFailureReporterForTest::clearReporter()
{
  delete instance_;
  instance_ = nullptr;
}

MockFailureReporterInstaller::MockFailureReporterInstaller()
{
  mock().setMockFailureStandardReporter(
      MockFailureReporterForTest::getReporter());
}

MockFailureReporterInstaller::~MockFailureReporterInstaller()
{
  mock().setMockFailureStandardReporter(nullptr);
  MockFailureReporterForTest::clearReporter();
}

cpputest::TestShell*
mockFailureTest()
{
  return MockFailureReporterForTest::getReporter()->getTestToFail();
}

cpputest::String
mockFailureString()
{
  return MockFailureReporterForTest::getReporter()->mockFailureString;
}

void
CLEAR_MOCK_FAILURE()
{
  MockFailureReporterForTest::getReporter()->mockFailureString = "";
}

void
CHECK_EXPECTED_MOCK_FAILURE_LOCATION(
    const cpputest::extensions::MockFailure& expectedFailure,
    const char* file,
    size_t line)
{
  cpputest::String expectedFailureString = expectedFailure.getMessage();
  cpputest::String actualFailureString = mockFailureString();
  CLEAR_MOCK_FAILURE();
  if (expectedFailureString != actualFailureString) {
    cpputest::String error = "MockFailures are different.\n";
    error += "Expected MockFailure:\n\t";
    error += expectedFailureString;
    error += "\nActual MockFailure:\n\t";
    error += actualFailureString;
    FAIL_LOCATION(error.c_str(), file, line);
  }
}

void
CHECK_NO_MOCK_FAILURE_LOCATION(const char* file, size_t line)
{
  if (mockFailureString() != "") {
    cpputest::String error = "Unexpected mock failure:\n";
    error += mockFailureString();
    CLEAR_MOCK_FAILURE();
    FAIL_LOCATION(error.c_str(), file, line);
  }
  CLEAR_MOCK_FAILURE();
}

MockExpectedCallsListForTest::~MockExpectedCallsListForTest()
{
  deleteAllExpectationsAndClearList();
}

cpputest::extensions::MockCheckedExpectedCall*
MockExpectedCallsListForTest::addFunction(const cpputest::String& name)
{
  cpputest::extensions::MockCheckedExpectedCall* newCall =
      new cpputest::extensions::MockCheckedExpectedCall;
  newCall->withName(name);
  addExpectedCall(newCall);
  return newCall;
}

cpputest::extensions::MockCheckedExpectedCall*
MockExpectedCallsListForTest::addFunction(unsigned int numCalls,
    const cpputest::String& name)
{
  cpputest::extensions::MockCheckedExpectedCall* newCall =
      new cpputest::extensions::MockCheckedExpectedCall(numCalls);
  newCall->withName(name);
  addExpectedCall(newCall);
  return newCall;
}

cpputest::extensions::MockCheckedExpectedCall*
MockExpectedCallsListForTest::addFunctionOrdered(const cpputest::String& name,
    unsigned int order)
{
  cpputest::extensions::MockCheckedExpectedCall* newCall = addFunction(name);
  newCall->withCallOrder(order);
  return newCall;
}
