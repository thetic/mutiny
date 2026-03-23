#include "mutiny/test.hpp"
#include "mutiny/test/Failure.hpp"
#include "mutiny/test/Output.hpp"
#include "mutiny/test/StringBufferOutput.hpp"
#include "mutiny/test/time.hpp"

namespace {
unsigned long mock_get_time_in_millis()
{
  return 10;
}
}

TEST_GROUP(TestResult)
{
  mu::tiny::test::TestOutput* printer;
  mu::tiny::test::StringBufferTestOutput* mock;

  mu::tiny::test::TestResult* res;

  void setup() override
  {
    mock = new mu::tiny::test::StringBufferTestOutput();
    printer = mock;
    res = new mu::tiny::test::TestResult(*printer);
    MUTINY_PTR_SET(mu::tiny::test::get_time_in_millis, mock_get_time_in_millis);
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
  CHECK(mu::tiny::test::string_contains(mock->get_output(), "10 ms"));
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
      mu::tiny::test::TestFailure(
          mu::tiny::test::TestShell::get_current(),
          mu::tiny::test::string_from("dummy message")
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
