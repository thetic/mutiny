#include "CppUTest/StringBufferTestOutput.hpp"
#include "CppUTest/TestFailure.hpp"
#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestOutput.hpp"
#include "CppUTest/time.hpp"

namespace {
unsigned long
MockGetTimeInMillis()
{
  return 10;
}
}

TEST_GROUP(TestResult)
{
  cpputest::TestOutput* printer;
  cpputest::StringBufferTestOutput* mock;

  cpputest::TestResult* res;

  void setup() override
  {
    mock = new cpputest::StringBufferTestOutput();
    printer = mock;
    res = new cpputest::TestResult(*printer);
    UT_PTR_SET(cpputest::GetTimeInMillis, MockGetTimeInMillis);
  }
  void teardown() override
  {
    delete printer;
    delete res;
  }
};

TEST(TestResult, TestEndedWillPrintResultsAndExecutionTime)
{
  res->testsEnded();
  CHECK(stringContains(mock->getOutput(), "10 ms"));
}

TEST(TestResult, ResultIsOkIfTestIsRunWithNoFailures)
{
  res->countTest();
  res->countRun();
  CHECK_FALSE(res->isFailure());
}

TEST(TestResult, ResultIsOkIfTestIsIgnored)
{
  res->countTest();
  res->countIgnored();
  CHECK_FALSE(res->isFailure());
}

TEST(TestResult, ResultIsNotOkIfFailures)
{
  res->countTest();
  res->countRun();
  res->addFailure(cpputest::TestFailure(cpputest::TestShell::getCurrent(),
      cpputest::StringFrom("dummy message")));
  CHECK_TRUE(res->isFailure());
}

TEST(TestResult, ResultIsNotOkIfNoTestsAtAll)
{
  CHECK_TRUE(res->isFailure());
}

TEST(TestResult, ResultIsNotOkIfNoTestsRunOrIgnored)
{
  res->countTest();
  CHECK_TRUE(res->isFailure());
}
