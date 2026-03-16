#include "CppMu/TestOutput.hpp"

#include "CppMu/CompositeTestOutput.hpp"
#include "CppMu/CppMu.hpp"
#include "CppMu/StringBufferTestOutput.hpp"
#include "CppMu/TestResult.hpp"
#include "CppMu/time.hpp"

namespace {
unsigned long millis_time;

unsigned long
mock_get_time_in_millis()
{
  return millis_time;
}

class CompositeTestOutputTestStringBufferTestOutput
  : public cppmu::StringBufferTestOutput
{
public:
  void print_tests_started() override { output_ += "Test Start\n"; }

  void print_tests_ended(const cppmu::TestResult& result) override
  {
    output_ += cppmu::string_from_format(
        "Test End %d\n", static_cast<int>(result.get_test_count()));
  }

  void print_current_group_started(const cppmu::TestShell& test) override
  {
    output_ +=
        cppmu::string_from_format("Group %s Start\n", test.get_group().c_str());
  }

  void print_current_group_ended(const cppmu::TestResult& res) override
  {
    output_ += cppmu::string_from_format(
        "Group End %d\n", static_cast<int>(res.get_test_count()));
  }

  void print_current_test_started(const cppmu::TestShell&) override
  {
    output_ += "s";
  }

  void flush() override { output_ += "flush"; }

  virtual bool is_verbose()
  {
    return verbose_ == VerbosityLevel::verbose ||
           verbose_ == VerbosityLevel::very_verbose;
  }

  virtual bool is_color() { return color_; }

  virtual const char* get_progress_indicator() { return progress_indication_; }
};

} // namespace

TEST_GROUP(TestOutput)
{
  cppmu::TestOutput* printer;
  cppmu::StringBufferTestOutput* mock;
  cppmu::TestShell* tst;
  cppmu::TestFailure* f;
  cppmu::TestFailure* f2;
  cppmu::TestFailure* f3;
  cppmu::TestResult* result;

  void setup() override
  {
    mock = new cppmu::StringBufferTestOutput();
    printer = mock;
    tst = new cppmu::TestShell("group", "test", "file", 10);
    f = new cppmu::TestFailure(tst, "failfile", 20, "message");
    f2 = new cppmu::TestFailure(tst, "file", 20, "message");
    f3 = new cppmu::TestFailure(tst, "file", 2, "message");
    result = new cppmu::TestResult(*mock);
    result->set_total_execution_time(10);
    millis_time = 0;
    CPPMU_PTR_SET(cppmu::get_time_in_millis, mock_get_time_in_millis);
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
  STRCMP_EQUAL("..................................................\n..........",
      mock->get_output().c_str());
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
      mock->get_output().c_str());
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
  mock->verbose(cppmu::TestOutput::VerbosityLevel::verbose);
  printer->print_current_test_started(*tst);
  STRCMP_EQUAL("TEST(group, test)", mock->get_output().c_str());
}

TEST(TestOutput, PrintTestVerboseEnded)
{
  mock->verbose(cppmu::TestOutput::VerbosityLevel::verbose);
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
  STRCMP_EQUAL("\n\033[32;1mOK (1 tests, 1 ran, 0 checks, 0 ignored, 0 "
               "filtered out, 10 ms)\033[m\n\n",
      mock->get_output().c_str());
}

TEST(TestOutput, printColorWithFailures)
{
  mock->color();
  run_one_test();
  result->add_failure(*f);
  printer->flush();
  printer->print_tests_ended(*result);
  STRCMP_EQUAL("\n\033[31;1mErrors (1 failures, 1 tests, 1 ran, 0 checks, 0 "
               "ignored, 0 filtered out, 10 ms)"
               "\033[m\n\n",
      mock->get_output().c_str());
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
  STRCMP_EQUAL("\nfile:20: error: Failure in TEST(group, test)\n\tmessage\n\n",
      mock->get_output().c_str());
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
      mock->get_output().c_str());
}

TEST(TestOutput, printTestsEndedWithFailures)
{
  result->add_failure(*f);
  printer->flush();
  printer->print_tests_ended(*result);
  STRCMP_EQUAL("\nErrors (1 failures, 0 tests, 0 ran, 0 checks, 0 ignored, 0 "
               "filtered out, 10 ms)\n\n",
      mock->get_output().c_str());
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
      mock->get_output().c_str());
}

TEST_GROUP(CompositeTestOutput)
{
  CompositeTestOutputTestStringBufferTestOutput* output1;
  CompositeTestOutputTestStringBufferTestOutput* output2;
  cppmu::CompositeTestOutput composite_output;
  cppmu::TestResult* result;
  cppmu::TestShell* test;

  void setup() override
  {
    output1 = new CompositeTestOutputTestStringBufferTestOutput;
    output2 = new CompositeTestOutputTestStringBufferTestOutput;
    composite_output.set_output_one(output1);
    composite_output.set_output_two(output2);
    result = new cppmu::TestResult(composite_output);
    test = new cppmu::TestShell("Group", "Name", "file", 10);
  }

  void teardown() override
  {
    delete test;
    delete result;
  }
};

TEST(CompositeTestOutput, TestStartedAndEnded)
{
  composite_output.print_tests_started();
  composite_output.print_tests_ended(*result);
  STRCMP_EQUAL("Test Start\nTest End 0\n", output1->get_output().c_str());
  STRCMP_EQUAL("Test Start\nTest End 0\n", output2->get_output().c_str());
}

TEST(CompositeTestOutput, CurrentTestStartedAndEnded)
{
  composite_output.print_current_test_started(*test);
  composite_output.print_current_test_ended(*result);
  STRCMP_EQUAL("s.", output1->get_output().c_str());
  STRCMP_EQUAL("s.", output2->get_output().c_str());
}

TEST(CompositeTestOutput, CurrentGroupStartedAndEnded)
{
  composite_output.print_current_group_started(*test);
  composite_output.print_current_group_ended(*result);
  STRCMP_EQUAL(
      "Group Group Start\nGroup End 0\n", output1->get_output().c_str());
  STRCMP_EQUAL(
      "Group Group Start\nGroup End 0\n", output2->get_output().c_str());
}

TEST(CompositeTestOutput, PrintBuffer)
{
  composite_output.print_buffer("Boo");
  STRCMP_EQUAL("Boo", output1->get_output().c_str());
  STRCMP_EQUAL("Boo", output2->get_output().c_str());
}

TEST(CompositeTestOutput, printChar)
{
  composite_output.print("Boo");
  STRCMP_EQUAL("Boo", output1->get_output().c_str());
  STRCMP_EQUAL("Boo", output2->get_output().c_str());
}

TEST(CompositeTestOutput, printLong)
{
  long ten = 10;
  composite_output.print(ten);
  STRCMP_EQUAL("10", output1->get_output().c_str());
  STRCMP_EQUAL("10", output2->get_output().c_str());
}

TEST(CompositeTestOutput, PrintSize)
{
  size_t ten = 10;
  composite_output.print(ten);
  STRCMP_EQUAL("10", output1->get_output().c_str());
  STRCMP_EQUAL("10", output2->get_output().c_str());
}

TEST(CompositeTestOutput, printDouble)
{
  composite_output.print_double(1.01);
  STRCMP_EQUAL("1.01", output1->get_output().c_str());
  STRCMP_EQUAL("1.01", output2->get_output().c_str());
}

TEST(CompositeTestOutput, verbose)
{
  composite_output.verbose(cppmu::TestOutput::VerbosityLevel::verbose);
  CHECK(output1->is_verbose());
  CHECK(output2->is_verbose());
}

TEST(CompositeTestOutput, color)
{
  composite_output.color();
  CHECK(output1->is_color());
  CHECK(output2->is_color());
}

TEST(CompositeTestOutput, PrintTestFailure)
{
  cppmu::TestFailure failure(test, "file", 10, "failed");
  composite_output.print_failure(failure);
  STRCMP_EQUAL("\nfile:10: error: Failure in TEST(Group, Name)\n\tfailed\n\n",
      output1->get_output().c_str());
  STRCMP_EQUAL("\nfile:10: error: Failure in TEST(Group, Name)\n\tfailed\n\n",
      output2->get_output().c_str());
}

TEST(CompositeTestOutput, PrintTestRun)
{
  composite_output.print_test_run(1, 2);
  STRCMP_EQUAL("Test run 1 of 2\n", output1->get_output().c_str());
  STRCMP_EQUAL("Test run 1 of 2\n", output2->get_output().c_str());
}

TEST(CompositeTestOutput, setProgressIndicator)
{
  composite_output.set_progress_indicator("?");
  STRCMP_EQUAL("?", output1->get_progress_indicator());
  STRCMP_EQUAL("?", output2->get_progress_indicator());
}

TEST(CompositeTestOutput, flush)
{
  composite_output.flush();
  STRCMP_EQUAL("flush", output1->get_output().c_str());
  STRCMP_EQUAL("flush", output2->get_output().c_str());
}

TEST(CompositeTestOutput, printVeryVerbose)
{
  composite_output.verbose(cppmu::TestOutput::VerbosityLevel::very_verbose);
  composite_output.print_very_verbose("very-verbose");
  STRCMP_EQUAL("very-verbose", output1->get_output().c_str());
  STRCMP_EQUAL("very-verbose", output2->get_output().c_str());
}
