#include "CppMu/CppMu.hpp"
#include "CppMu/StringBufferTestOutput.hpp"
#include "CppMu/TestFailure.hpp"
#include "CppMu/TestOutput.hpp"
#include "CppMu/time.hpp"

namespace {
unsigned long mock_get_time_in_millis()
{
  return 10;
}
}

TEST_GROUP(TestResult)
{
  cppmu::TestOutput* printer;
  cppmu::StringBufferTestOutput* mock;

  cppmu::TestResult* res;

  void setup() override
  {
    mock = new cppmu::StringBufferTestOutput();
    printer = mock;
    res = new cppmu::TestResult(*printer);
    CPPMU_PTR_SET(cppmu::get_time_in_millis, mock_get_time_in_millis);
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
  CHECK(cppmu::string_contains(mock->get_output(), "10 ms"));
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
  res->add_failure(
      cppmu::TestFailure(
          cppmu::TestShell::get_current(), cppmu::string_from("dummy message")
      )
  );
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
