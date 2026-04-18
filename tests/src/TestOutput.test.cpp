#include "mutiny/test/Output.hpp"
#include "mutiny/test/Result.hpp"
#include "mutiny/test/StringBufferOutput.hpp"

#include "mutiny/test.hpp"
#include "mutiny/time.hpp"

namespace {
uint_least64_t millis_time;

uint_least64_t mock_get_time_in_millis()
{
  return millis_time;
}

} // namespace

TEST_GROUP(Output)
{
  mu::tiny::test::Output* printer;
  mu::tiny::test::StringBufferOutput* mock;
  mu::tiny::test::Shell* tst;
  mu::tiny::test::Failure* f;
  mu::tiny::test::Failure* f2;
  mu::tiny::test::Failure* f3;
  mu::tiny::test::Result* result;

  void setup() override
  {
    mock = new mu::tiny::test::StringBufferOutput();
    printer = mock;
    tst = new mu::tiny::test::Shell("group", "test", "file", 10);
    f = new mu::tiny::test::Failure(tst, "failfile", 20, "message");
    f2 = new mu::tiny::test::Failure(tst, "file", 20, "message");
    f3 = new mu::tiny::test::Failure(tst, "file", 2, "message");
    result = new mu::tiny::test::Result(*mock);
    result->set_total_execution_time(10);
    millis_time = 0;
    MUTINY_PTR_SET(mu::tiny::get_time_in_millis, mock_get_time_in_millis);
  }
  void teardown() override
  {
    delete printer;
    delete tst;
    delete f;
    delete f2;
    delete f3;
    delete result;
  }

  void run_one_test()
  {
    result->count_test();
    result->count_run();
  }
};

TEST(Output, PrintConstCharStar)
{
  printer->print("hello");
  printer->print("hello\n");
  STRCMP_EQUAL("hellohello\n", mock->get_output().c_str());
}

TEST(Output, PrintLong)
{
  long number = 1234;
  printer->print(number);
  STRCMP_EQUAL("1234", mock->get_output().c_str());
}

TEST(Output, PrintSize)
{
  size_t ten = 10;
  printer->print(ten);
  STRCMP_EQUAL("10", mock->get_output().c_str());
}

TEST(Output, PrintDouble)
{
  printer->print_double(12.34);
  STRCMP_EQUAL("12.34", mock->get_output().c_str());
}

TEST(Output, StreamOperators)
{
  *printer << "n=" << 1234;
  STRCMP_EQUAL("n=1234", mock->get_output().c_str());
}

TEST(Output, PrintTestEnded)
{
  printer->print_current_test_ended(*result);
  STRCMP_EQUAL(".", mock->get_output().c_str());
}

TEST(Output, PrintTestALot)
{
  for (int i = 0; i < 60; ++i) {
    printer->print_current_test_ended(*result);
  }
  STRCMP_EQUAL(
      "..................................................\n..........",
      mock->get_output().c_str()
  );
}

TEST(Output, PrintTestALotAndSimulateRepeatRun)
{
  for (int i = 0; i < 60; ++i) {
    run_one_test();
    printer->print_current_test_ended(*result);
  }

  printer->print_tests_ended(*result);

  for (int i = 0; i < 60; ++i) {
    run_one_test();
    printer->print_current_test_ended(*result);
  }
  STRCMP_EQUAL(
      "..................................................\n.........."
      "\nOK (60 tests, 60 ran, 0 checks, 0 ignored, 0 filtered out, 10 ms)\n\n"
      "..................................................\n..........",
      mock->get_output().c_str()
  );
}

TEST(Output, SetProgressIndicator)
{
  printer->set_progress_indicator(".");
  printer->print_current_test_ended(*result);
  printer->set_progress_indicator("!");
  printer->print_current_test_ended(*result);
  printer->set_progress_indicator(".");
  printer->print_current_test_ended(*result);

  STRCMP_EQUAL(".!.", mock->get_output().c_str());
}

TEST(Output, PrintTestVerboseStarted)
{
  mock->verbose(mu::tiny::test::Output::MutinyVerbosityLevel::verbose);
  printer->print_current_test_started(*tst);
  STRCMP_EQUAL("TEST(group, test)", mock->get_output().c_str());
}

TEST(Output, PrintTestVerboseEnded)
{
  mock->verbose(mu::tiny::test::Output::MutinyVerbosityLevel::verbose);
  result->current_test_started(tst);
  millis_time = 5;
  result->current_test_ended(tst);
  STRCMP_EQUAL("TEST(group, test) - 5 ms\n", mock->get_output().c_str());
}

TEST(Output, printColorWithSuccess)
{
  mock->color();
  run_one_test();
  printer->print_tests_ended(*result);
  STRCMP_EQUAL(
      "\n\033[32;1mOK (1 tests, 1 ran, 0 checks, 0 ignored, 0 "
      "filtered out, 10 ms)\033[m\n\n",
      mock->get_output().c_str()
  );
}

TEST(Output, printColorWithFailures)
{
  mock->color();
  run_one_test();
  result->add_failure(*f);
  printer->flush();
  printer->print_tests_ended(*result);
  STRCMP_EQUAL(
      "\n\033[31;1mErrors (1 failures, 1 tests, 1 ran, 0 checks, 0 "
      "ignored, 0 filtered out, 10 ms)"
      "\033[m\n\n",
      mock->get_output().c_str()
  );
}

TEST(Output, PrintTestRun)
{
  printer->print_test_run(2, 3);
  STRCMP_EQUAL("Test run 2 of 3\n", mock->get_output().c_str());
}

TEST(Output, PrintTestRunOnlyOne)
{
  printer->print_test_run(1, 1);
  STRCMP_EQUAL("", mock->get_output().c_str());
}

TEST(Output, PrintWithFailureInSameFile)
{
  printer->print_failure(*f2);
  STRCMP_EQUAL(
      "\nfile:20: error: Failure in TEST(group, test)\n\tmessage\n\n",
      mock->get_output().c_str()
  );
}

TEST(Output, PrintFailureWithFailInDifferentFile)
{
  printer->print_failure(*f);
  const char* expected = "\nfile:10: error: Failure in TEST(group, test)"
                         "\nfailfile:20: error:\n\tmessage\n\n";
  STRCMP_EQUAL(expected, mock->get_output().c_str());
}

TEST(Output, PrintFailureWithFailInHelper)
{
  printer->print_failure(*f3);
  const char* expected = "\nfile:10: error: Failure in TEST(group, test)"
                         "\nfile:2: error:\n\tmessage\n\n";
  STRCMP_EQUAL(expected, mock->get_output().c_str());
}

TEST(Output, PrintTestStarts)
{
  printer->print_tests_started();
  STRCMP_EQUAL("", mock->get_output().c_str());
}

TEST(Output, printTestsEnded)
{
  result->count_test();
  result->count_check();
  result->count_ignored();
  result->count_ignored();
  result->count_run();
  result->count_run();
  result->count_run();
  printer->print_tests_ended(*result);
  STRCMP_EQUAL(
      "\nOK (1 tests, 3 ran, 1 checks, 2 ignored, 0 filtered out, 10 ms)\n\n",
      mock->get_output().c_str()
  );
}

TEST(Output, printTestsEndedWithFailures)
{
  result->add_failure(*f);
  printer->flush();
  printer->print_tests_ended(*result);
  STRCMP_EQUAL(
      "\nErrors (1 failures, 0 tests, 0 ran, 0 checks, 0 ignored, 0 "
      "filtered out, 10 ms)\n\n",
      mock->get_output().c_str()
  );
}

TEST(Output, printTestsEndedWithNoTestsRunOrIgnored)
{
  result->count_test();
  printer->flush();
  printer->print_tests_ended(*result);
  STRCMP_EQUAL(
      "\nErrors (ran nothing, 1 tests, 0 ran, 0 checks, 0 ignored, 0 filtered "
      "out, 10 ms)\n"
      "Note: test run failed because no tests were run or ignored. Assuming "
      "something went wrong. "
      "This often happens because of linking errors or typos in test "
      "filter.\n\n",
      mock->get_output().c_str()
  );
}
