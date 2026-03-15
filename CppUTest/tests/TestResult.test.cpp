#include "CppUTest/StringBufferTestOutput.hpp"
#include "CppUTest/TestFailure.hpp"
#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestOutput.hpp"
#include "CppUTest/time.hpp"

namespace {
unsigned long
mock_get_time_in_millis()
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
    UT_PTR_SET(cpputest::get_time_in_millis, mock_get_time_in_millis);
  }
  void teardown() override
  {
    delete printer;
    delete res;
  }
};

TEST(TestResult, TestEndedWillPrintResultsAndExecutionTime)
{
  res->tests_ended();
  CHECK(string_contains(mock->get_output(), "10 ms"));
}

TEST(TestResult, ResultIsOkIfTestIsRunWithNoFailures)
{
  res->count_test();
  res->count_run();
  CHECK_FALSE(res->is_failure());
}

TEST(TestResult, ResultIsOkIfTestIsIgnored)
{
  res->count_test();
  res->count_ignored();
  CHECK_FALSE(res->is_failure());
}

TEST(TestResult, ResultIsNotOkIfFailures)
{
  res->count_test();
  res->count_run();
  res->add_failure(cpputest::TestFailure(cpputest::TestShell::get_current(),
      cpputest::string_from("dummy message")));
  CHECK_TRUE(res->is_failure());
}

TEST(TestResult, ResultIsNotOkIfNoTestsAtAll)
{
  CHECK_TRUE(res->is_failure());
}

TEST(TestResult, ResultIsNotOkIfNoTestsRunOrIgnored)
{
  res->count_test();
  CHECK_TRUE(res->is_failure());
}
