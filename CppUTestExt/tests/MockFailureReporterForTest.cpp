#include "MockFailureReporterForTest.h"

#include "CppUTest/TestHarness.hpp"

using namespace cpputest;
using namespace cpputest::extensions;
using UtestShell = cpputest::TestShell;

void
MockFailureReporterForTest::failTest(MockFailure failure)
{
  mockFailureString = failure.getMessage();
}

void
MockFailureReporterForTest::reportFailure(const MockFailure& failure)
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

UtestShell*
mockFailureTest()
{
  return MockFailureReporterForTest::getReporter()->getTestToFail();
}

String
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
CHECK_EXPECTED_MOCK_FAILURE_LOCATION(const MockFailure& expectedFailure,
    const char* file,
    size_t line)
{
  String expectedFailureString = expectedFailure.getMessage();
  String actualFailureString = mockFailureString();
  CLEAR_MOCK_FAILURE();
  if (expectedFailureString != actualFailureString) {
    String error = "MockFailures are different.\n";
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
    String error = "Unexpected mock failure:\n";
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

MockCheckedExpectedCall*
MockExpectedCallsListForTest::addFunction(const String& name)
{
  MockCheckedExpectedCall* newCall = new MockCheckedExpectedCall;
  newCall->withName(name);
  addExpectedCall(newCall);
  return newCall;
}

MockCheckedExpectedCall*
MockExpectedCallsListForTest::addFunction(unsigned int numCalls,
    const String& name)
{
  MockCheckedExpectedCall* newCall = new MockCheckedExpectedCall(numCalls);
  newCall->withName(name);
  addExpectedCall(newCall);
  return newCall;
}

MockCheckedExpectedCall*
MockExpectedCallsListForTest::addFunctionOrdered(const String& name,
    unsigned int order)
{
  MockCheckedExpectedCall* newCall = addFunction(name);
  newCall->withCallOrder(order);
  return newCall;
}
