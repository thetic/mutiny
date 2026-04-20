#include "mu/tiny/test/Failure.hpp"
#include "mu/tiny/test/Output.hpp"
#include "mu/tiny/test/StringBufferOutput.hpp"

#include "mu/tiny/test.hpp"
#include "mu/tiny/time.hpp"

namespace {
uint_least64_t mock_get_time_in_millis()
{
  return 10;
}
} // namespace

TEST_GROUP(Result)
{
  mu::tiny::test::Output* printer;
  mu::tiny::test::StringBufferOutput* mock;

  mu::tiny::test::Result* res;

  void setup() override
  {
    mock = new mu::tiny::test::StringBufferOutput();
    printer = mock;
    res = new mu::tiny::test::Result(*printer);
    MUTINY_PTR_SET(mu::tiny::get_time_in_millis, mock_get_time_in_millis);
  }
  void teardown() override
  {
    delete printer;
    delete res;
  }
};

TEST(Result, TestEndedWillPrintResultsAndExecutionTime)
{
  res->tests_ended();
  CHECK(mu::tiny::string_contains(mock->get_output(), "10 ms"));
}

TEST(Result, ResultIsOkIfTestIsRunWithNoFailures)
{
  res->count_test();
  res->count_run();
  CHECK(!res->is_failure());
}

TEST(Result, ResultIsOkIfTestIsIgnored)
{
  res->count_test();
  res->count_skipped();
  CHECK(!res->is_failure());
}

TEST(Result, ResultIsNotOkIfFailures)
{
  res->count_test();
  res->count_run();
  res->add_failure(
      mu::tiny::test::Failure(
          mu::tiny::test::Shell::get_current(),
          mu::tiny::string_from("dummy message")
      )
  );
  CHECK(res->is_failure());
}

TEST(Result, ResultIsNotOkIfNoTestsAtAll)
{
  CHECK(res->is_failure());
}

TEST(Result, ResultIsNotOkIfNoTestsRunOrIgnored)
{
  res->count_test();
  CHECK(res->is_failure());
}
