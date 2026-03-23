#include "mutiny/test.hpp"
#include "mutiny/test/CompositeOutput.hpp"
#include "mutiny/test/Output.hpp"
#include "mutiny/test/Result.hpp"
#include "mutiny/test/StringBufferOutput.hpp"

namespace {

class CompositeTestOutputTestStringBufferTestOutput
  : public mu::tiny::test::StringBufferTestOutput
{
public:
  void print_tests_started() override { output_ += "Test Start\n"; }

  void print_tests_ended(const mu::tiny::test::TestResult& result) override
  {
    output_ += mu::tiny::test::string_from_format(
        "Test End %d\n", static_cast<int>(result.get_test_count())
    );
  }

  void print_current_group_started(
      const mu::tiny::test::TestShell& test
  ) override
  {
    output_ += mu::tiny::test::string_from_format(
        "Group %s Start\n", test.get_group()
    );
  }

  void print_current_group_ended(const mu::tiny::test::TestResult& res) override
  {
    output_ += mu::tiny::test::string_from_format(
        "Group End %d\n", static_cast<int>(res.get_test_count())
    );
  }

  void print_current_test_started(const mu::tiny::test::TestShell&) override
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

TEST_GROUP(CompositeTestOutput)
{
  CompositeTestOutputTestStringBufferTestOutput* output1;
  CompositeTestOutputTestStringBufferTestOutput* output2;
  mu::tiny::test::CompositeTestOutput composite_output;
  mu::tiny::test::TestResult* result;
  mu::tiny::test::TestShell* test;

  void setup() override
  {
    output1 = new CompositeTestOutputTestStringBufferTestOutput;
    output2 = new CompositeTestOutputTestStringBufferTestOutput;
    composite_output.set_output_one(output1);
    composite_output.set_output_two(output2);
    result = new mu::tiny::test::TestResult(composite_output);
    test = new mu::tiny::test::TestShell("Group", "Name", "file", 10);
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
      "Group Group Start\nGroup End 0\n", output1->get_output().c_str()
  );
  STRCMP_EQUAL(
      "Group Group Start\nGroup End 0\n", output2->get_output().c_str()
  );
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
  composite_output.verbose(mu::tiny::test::TestOutput::VerbosityLevel::verbose);
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
  mu::tiny::test::TestFailure failure(test, "file", 10, "failed");
  composite_output.print_failure(failure);
  STRCMP_EQUAL(
      "\nfile:10: error: Failure in TEST(Group, Name)\n\tfailed\n\n",
      output1->get_output().c_str()
  );
  STRCMP_EQUAL(
      "\nfile:10: error: Failure in TEST(Group, Name)\n\tfailed\n\n",
      output2->get_output().c_str()
  );
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
  composite_output.verbose(
      mu::tiny::test::TestOutput::VerbosityLevel::very_verbose
  );
  composite_output.print_very_verbose("very-verbose");
  STRCMP_EQUAL("very-verbose", output1->get_output().c_str());
  STRCMP_EQUAL("very-verbose", output2->get_output().c_str());
}
