#include "mutiny/test/CompositeOutput.hpp"
#include "mutiny/test/Output.hpp"
#include "mutiny/test/Result.hpp"
#include "mutiny/test/StringBufferOutput.hpp"

#include "mutiny/test.hpp"

namespace {

class CompositeTestOutputTestStringBufferTestOutput
  : public mu::tiny::test::StringBufferOutput
{
public:
  void print_tests_started() override { output_ += "Test Start\n"; }

  void print_tests_ended(const mu::tiny::test::Result& result) override
  {
    output_ += mu::tiny::string_from_format(
        "Test End %d\n", static_cast<int>(result.get_test_count())
    );
  }

  void print_current_group_started(const mu::tiny::test::Shell& test) override
  {
    output_ +=
        mu::tiny::string_from_format("Group %s Start\n", test.get_group());
  }

  void print_current_group_ended(const mu::tiny::test::Result& res) override
  {
    output_ += mu::tiny::string_from_format(
        "Group End %d\n", static_cast<int>(res.get_test_count())
    );
  }

  void print_current_test_started(const mu::tiny::test::Shell&) override
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

TEST_GROUP(CompositeOutput)
{
  CompositeTestOutputTestStringBufferTestOutput* output1;
  CompositeTestOutputTestStringBufferTestOutput* output2;
  mu::tiny::test::CompositeOutput composite_output;
  mu::tiny::test::Result* result;
  mu::tiny::test::Shell* test;

  void setup() override
  {
    output1 = new CompositeTestOutputTestStringBufferTestOutput;
    output2 = new CompositeTestOutputTestStringBufferTestOutput;
    composite_output.set_output_one(output1);
    composite_output.set_output_two(output2);
    result = new mu::tiny::test::Result(composite_output);
    test = new mu::tiny::test::Shell("Group", "Name", "file", 10);
  }

  void teardown() override
  {
    delete test;
    delete result;
  }
};

TEST(CompositeOutput, TestStartedAndEnded)
{
  composite_output.print_tests_started();
  composite_output.print_tests_ended(*result);
  STRCMP_EQUAL("Test Start\nTest End 0\n", output1->get_output().c_str());
  STRCMP_EQUAL("Test Start\nTest End 0\n", output2->get_output().c_str());
}

TEST(CompositeOutput, CurrentTestStartedAndEnded)
{
  composite_output.print_current_test_started(*test);
  composite_output.print_current_test_ended(*result);
  STRCMP_EQUAL("s.", output1->get_output().c_str());
  STRCMP_EQUAL("s.", output2->get_output().c_str());
}

TEST(CompositeOutput, CurrentGroupStartedAndEnded)
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

TEST(CompositeOutput, PrintBuffer)
{
  composite_output.print_buffer("Boo");
  STRCMP_EQUAL("Boo", output1->get_output().c_str());
  STRCMP_EQUAL("Boo", output2->get_output().c_str());
}

TEST(CompositeOutput, printChar)
{
  composite_output.print("Boo");
  STRCMP_EQUAL("Boo", output1->get_output().c_str());
  STRCMP_EQUAL("Boo", output2->get_output().c_str());
}

TEST(CompositeOutput, printLong)
{
  long ten = 10;
  composite_output.print(ten);
  STRCMP_EQUAL("10", output1->get_output().c_str());
  STRCMP_EQUAL("10", output2->get_output().c_str());
}

TEST(CompositeOutput, PrintSize)
{
  size_t ten = 10;
  composite_output.print(ten);
  STRCMP_EQUAL("10", output1->get_output().c_str());
  STRCMP_EQUAL("10", output2->get_output().c_str());
}

TEST(CompositeOutput, printDouble)
{
  composite_output.print_double(1.01);
  STRCMP_EQUAL("1.01", output1->get_output().c_str());
  STRCMP_EQUAL("1.01", output2->get_output().c_str());
}

TEST(CompositeOutput, verbose)
{
  composite_output.verbose(mu::tiny::test::Output::VerbosityLevel::verbose);
  CHECK(output1->is_verbose());
  CHECK(output2->is_verbose());
}

TEST(CompositeOutput, color)
{
  composite_output.color();
  CHECK(output1->is_color());
  CHECK(output2->is_color());
}

TEST(CompositeOutput, PrintTestFailure)
{
  mu::tiny::test::Failure failure(test, "file", 10, "failed");
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

TEST(CompositeOutput, PrintTestRun)
{
  composite_output.print_test_run(1, 2);
  STRCMP_EQUAL("Test run 1 of 2\n", output1->get_output().c_str());
  STRCMP_EQUAL("Test run 1 of 2\n", output2->get_output().c_str());
}

TEST(CompositeOutput, setProgressIndicator)
{
  composite_output.set_progress_indicator("?");
  STRCMP_EQUAL("?", output1->get_progress_indicator());
  STRCMP_EQUAL("?", output2->get_progress_indicator());
}

TEST(CompositeOutput, flush)
{
  composite_output.flush();
  STRCMP_EQUAL("flush", output1->get_output().c_str());
  STRCMP_EQUAL("flush", output2->get_output().c_str());
}

TEST(CompositeOutput, printVeryVerbose)
{
  composite_output.verbose(
      mu::tiny::test::Output::VerbosityLevel::very_verbose
  );
  composite_output.print_very_verbose("very-verbose");
  STRCMP_EQUAL("very-verbose", output1->get_output().c_str());
  STRCMP_EQUAL("very-verbose", output2->get_output().c_str());
}
