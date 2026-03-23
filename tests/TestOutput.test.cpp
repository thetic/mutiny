#include "mutiny/test.hpp"
#include "mutiny/test/Output.hpp"
#include "mutiny/test/Result.hpp"
#include "mutiny/test/StringBufferOutput.hpp"
#include "mutiny/test/time.hpp"

namespace {
unsigned long millis_time;

unsigned long mock_get_time_in_millis()
{
  return millis_time;
}

} // namespace

TEST_GROUP(TestOutput)
{
  mu::tiny::test::TestOutput* printer;
  mu::tiny::test::StringBufferTestOutput* mock;
  mu::tiny::test::TestShell* tst;
  mu::tiny::test::TestFailure* f;
  mu::tiny::test::TestFailure* f2;
  mu::tiny::test::TestFailure* f3;
  mu::tiny::test::TestResult* result;

  void setup() override
  {
    mock = new mu::tiny::test::StringBufferTestOutput();
    printer = mock;
    tst = new mu::tiny::test::TestShell("group", "test", "file", 10);
    f = new mu::tiny::test::TestFailure(tst, "failfile", 20, "message");
    f2 = new mu::tiny::test::TestFailure(tst, "file", 20, "message");
    f3 = new mu::tiny::test::TestFailure(tst, "file", 2, "message");
    result = new mu::tiny::test::TestResult(*mock);
    result->set_total_execution_time(10);
    millis_time = 0;
    MUTINY_PTR_SET(mu::tiny::test::get_time_in_millis, mock_get_time_in_millis);
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

TEST(TestOutput, PrintConstCharStar)
{
  printer->print("hello");
  printer->print("hello\n");
  STRCMP_EQUAL("hellohello\n", mock->get_output().c_str());
}

TEST(TestOutput, PrintLong)
{
  long number = 1234;
  printer->print(number);
  STRCMP_EQUAL("1234", mock->get_output().c_str());
}

TEST(TestOutput, PrintSize)
{
  size_t ten = 10;
  printer->print(ten);
  STRCMP_EQUAL("10", mock->get_output().c_str());
}

TEST(TestOutput, PrintDouble)
{
  printer->print_double(12.34);
  STRCMP_EQUAL("12.34", mock->get_output().c_str());
}

TEST(TestOutput, StreamOperators)
{
  *printer << "n=" << 1234;
  STRCMP_EQUAL("n=1234", mock->get_output().c_str());
}

TEST(TestOutput, PrintTestEnded)
{
  printer->print_current_test_ended(*result);
  STRCMP_EQUAL(".", mock->get_output().c_str());
}

TEST(TestOutput, PrintTestALot)
{
  for (int i = 0; i < 60; ++i) {
    printer->print_current_test_ended(*result);
  }
  STRCMP_EQUAL(
      "..................................................\n..........",
      mock->get_output().c_str()
  );
}

TEST(TestOutput, PrintTestALotAndSimulateRepeatRun)
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

TEST(TestOutput, SetProgressIndicator)
{
  printer->set_progress_indicator(".");
  printer->print_current_test_ended(*result);
  printer->set_progress_indicator("!");
  printer->print_current_test_ended(*result);
  printer->set_progress_indicator(".");
  printer->print_current_test_ended(*result);

  STRCMP_EQUAL(".!.", mock->get_output().c_str());
}

TEST(TestOutput, PrintTestVerboseStarted)
{
  mock->verbose(mu::tiny::test::TestOutput::VerbosityLevel::verbose);
  printer->print_current_test_started(*tst);
  STRCMP_EQUAL("TEST(group, test)", mock->get_output().c_str());
}

TEST(TestOutput, PrintTestVerboseEnded)
{
  mock->verbose(mu::tiny::test::TestOutput::VerbosityLevel::verbose);
  result->current_test_started(tst);
  millis_time = 5;
  result->current_test_ended(tst);
  STRCMP_EQUAL("TEST(group, test) - 5 ms\n", mock->get_output().c_str());
}

TEST(TestOutput, printColorWithSuccess)
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

TEST(TestOutput, printColorWithFailures)
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

TEST(TestOutput, PrintTestRun)
{
  printer->print_test_run(2, 3);
  STRCMP_EQUAL("Test run 2 of 3\n", mock->get_output().c_str());
}

TEST(TestOutput, PrintTestRunOnlyOne)
{
  printer->print_test_run(1, 1);
  STRCMP_EQUAL("", mock->get_output().c_str());
}

TEST(TestOutput, PrintWithFailureInSameFile)
{
  printer->print_failure(*f2);
  STRCMP_EQUAL(
      "\nfile:20: error: Failure in TEST(group, test)\n\tmessage\n\n",
      mock->get_output().c_str()
  );
}

TEST(TestOutput, PrintFailureWithFailInDifferentFile)
{
  printer->print_failure(*f);
  const char* expected = "\nfile:10: error: Failure in TEST(group, test)"
                         "\nfailfile:20: error:\n\tmessage\n\n";
  STRCMP_EQUAL(expected, mock->get_output().c_str());
}

TEST(TestOutput, PrintFailureWithFailInHelper)
{
  printer->print_failure(*f3);
  const char* expected = "\nfile:10: error: Failure in TEST(group, test)"
                         "\nfile:2: error:\n\tmessage\n\n";
  STRCMP_EQUAL(expected, mock->get_output().c_str());
}

TEST(TestOutput, PrintTestStarts)
{
  printer->print_tests_started();
  STRCMP_EQUAL("", mock->get_output().c_str());
}

TEST(TestOutput, printTestsEnded)
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

TEST(TestOutput, printTestsEndedWithFailures)
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

TEST(TestOutput, printTestsEndedWithNoTestsRunOrIgnored)
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
