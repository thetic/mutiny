#include "mu/tiny/test/Failure.hpp"
#include "mu/tiny/test/IgnoredShell.hpp"
#include "mu/tiny/test/Output.hpp"
#include "mu/tiny/test/Result.hpp"
#include "mu/tiny/test/Shell.hpp"
#include "mu/tiny/test/TapOutputPlugin.hpp"

#include "mu/tiny/String.hpp"
#include "mu/tiny/test.hpp"

namespace {

class TapOutputBuffer
{
  mu::tiny::String content_;

public:
  TapOutputBuffer() = default;

  void write(const char* s) { content_ += s; }
  void clear() { content_ = ""; }

  mu::tiny::String content() const { return content_; }

  const char* line(size_t n) const
  {
    const char* p = content_.c_str();
    size_t count = 1;
    while ((*p != 0) && count < n) {
      if (*p == '\n') {
        ++count;
      }
      ++p;
    }
    line_buf_ = "";
    while ((*p != 0) && *p != '\n') {
      line_buf_ += *p++;
    }
    if (*p == '\n') {
      line_buf_ += '\n';
    }
    return line_buf_.c_str();
  }

  size_t amount_of_lines() const
  {
    size_t count = 0;
    for (const char* p = content_.c_str(); *p != 0; ++p) {
      if (*p == '\n') {
        ++count;
      }
    }
    return count;
  }

  const char* line_from_the_back(size_t n) const
  {
    return line(amount_of_lines() - (n - 1));
  }

private:
  mutable mu::tiny::String line_buf_;
};

class TapTestOutputTestRunner
{
  mu::tiny::test::Result result_;

  const char* current_group_name_{ nullptr };
  mu::tiny::test::Shell* current_test_{ nullptr };
  bool first_test_in_group_{ true };
  mu::tiny::test::Failure* test_failure_{ nullptr };
  const char* pending_skip_message_{ nullptr };

public:
  explicit TapTestOutputTestRunner(const mu::tiny::test::Result& result)
    : result_(result)
  {
  }

  TapTestOutputTestRunner& start()
  {
    result_.tests_started();
    return *this;
  }

  TapTestOutputTestRunner& end()
  {
    end_of_previous_test_group();
    delete current_test_;
    result_.tests_ended();
    return *this;
  }

  void end_of_previous_test_group()
  {
    run_previous_test();
    if (current_test_ != nullptr) {
      result_.current_group_ended(current_test_);
      first_test_in_group_ = true;
    }
    current_group_name_ = nullptr;
  }

  TapTestOutputTestRunner& with_group(const char* group_name)
  {
    end_of_previous_test_group();
    delete current_test_;
    current_test_ = nullptr;
    current_group_name_ = group_name;
    return *this;
  }

  TapTestOutputTestRunner& with_test(const char* test_name)
  {
    run_previous_test();
    delete current_test_;
    current_test_ =
        new mu::tiny::test::Shell(current_group_name_, test_name, "file", 1);
    return *this;
  }

  TapTestOutputTestRunner& with_ignored_test(const char* test_name)
  {
    run_previous_test();
    delete current_test_;
    current_test_ = new mu::tiny::test::IgnoredShell(
        current_group_name_, test_name, "file", 1
    );
    return *this;
  }

  void run_previous_test()
  {
    if (current_test_ == nullptr) {
      return;
    }

    if (first_test_in_group_) {
      result_.current_group_started(current_test_);
      first_test_in_group_ = false;
    }
    result_.current_test_started(current_test_);

    if (pending_skip_message_ != nullptr) {
      result_.skip_test(pending_skip_message_);
      pending_skip_message_ = nullptr;
    }

    if (test_failure_ != nullptr) {
      result_.add_failure(*test_failure_);
      delete test_failure_;
      test_failure_ = nullptr;
    }

    result_.current_test_ended(current_test_);
  }

  TapTestOutputTestRunner& that_fails(
      const char* message,
      const char* file,
      size_t line
  )
  {
    test_failure_ =
        new mu::tiny::test::Failure(current_test_, file, line, message);
    return *this;
  }

#if MUTINY_HAVE_EXCEPTIONS
  TapTestOutputTestRunner& that_errors()
  {
    test_failure_ =
        // NOLINTNEXTLINE(bugprone-throw-keyword-missing)
        new mu::tiny::test::UnexpectedExceptionFailure(current_test_);
    return *this;
  }
#endif

  TapTestOutputTestRunner& that_is_skipped(const char* message)
  {
    pending_skip_message_ = message;
    return *this;
  }
};

TapOutputBuffer* tap_buffer = nullptr;

void mock_tap_fputs(const char* str, mu::tiny::test::Output::File file)
{
  if (file == mu::tiny::test::Output::stdout_ && tap_buffer != nullptr) {
    tap_buffer->write(str);
  }
}

} // namespace

TEST_GROUP(TapOutput)
{
  mu::tiny::test::TapOutputPlugin* plugin;
  mu::tiny::test::Output* output;
  mu::tiny::test::Result* result;
  TapTestOutputTestRunner* test_case_runner;
  mu::tiny::test::Output::FPutsFunc original_fputs;

  void setup() override
  {
    tap_buffer = new TapOutputBuffer();
    original_fputs = mu::tiny::test::Output::fputs_;
    MUTINY_PTR_SET(mu::tiny::test::Output::fputs_, mock_tap_fputs);

    plugin = new mu::tiny::test::TapOutputPlugin();
    const char* argv[] = { "", "-ptap" };
    plugin->parse_arguments(2, argv, 1);
    output = plugin->create_output();
    result = new mu::tiny::test::Result(*output);
    test_case_runner = new TapTestOutputTestRunner(*result);
  }

  void teardown() override
  {
    MUTINY_PTR_SET(mu::tiny::test::Output::fputs_, original_fputs);
    delete test_case_runner;
    delete result;
    delete output;
    delete plugin;
    delete tap_buffer;
    tap_buffer = nullptr;
  }
};

TEST(TapOutput, headerLineIsVersion13)
{
  test_case_runner->start().with_group("group").with_test("test").end();
  STRCMP_EQUAL("TAP version 13\n", tap_buffer->line(1));
}

TEST(TapOutput, planLineReflectsTestCount)
{
  test_case_runner->start()
      .with_group("group")
      .with_test("first")
      .with_test("second")
      .with_test("third")
      .end();
  STRCMP_EQUAL("1..3\n", tap_buffer->line(2));
}

TEST(TapOutput, singlePassingTestEmitsOkLine)
{
  test_case_runner->start().with_group("MyGroup").with_test("MyTest").end();
  STRCMP_EQUAL("ok 1 - MyGroup.MyTest\n", tap_buffer->line(3));
}

TEST(TapOutput, multiplePassingTestsNumberedSequentially)
{
  test_case_runner->start()
      .with_group("group")
      .with_test("first")
      .with_test("second")
      .end();
  STRCMP_EQUAL("ok 1 - group.first\n", tap_buffer->line(3));
  STRCMP_EQUAL("ok 2 - group.second\n", tap_buffer->line(4));
}

TEST(TapOutput, failingTestEmitsNotOkLine)
{
  test_case_runner->start()
      .with_group("group")
      .with_test("failTest")
      .that_fails("assertion failed", "src/foo.cpp", 42)
      .end();
  STRCMP_EQUAL("not ok 1 - group.failTest\n", tap_buffer->line(3));
}

TEST(TapOutput, failingTestYamlBlockContainsMessage)
{
  test_case_runner->start()
      .with_group("group")
      .with_test("failTest")
      .that_fails("assertion failed", "src/foo.cpp", 42)
      .end();
  STRCMP_EQUAL("  ---\n", tap_buffer->line(4));
  STRCMP_EQUAL("  message: \"assertion failed\"\n", tap_buffer->line(5));
  STRCMP_EQUAL("  severity: fail\n", tap_buffer->line(6));
  STRCMP_EQUAL("  at:\n", tap_buffer->line(7));
  STRCMP_EQUAL("    file: src/foo.cpp\n", tap_buffer->line(8));
  STRCMP_EQUAL("    line: 42\n", tap_buffer->line(9));
  STRCMP_EQUAL("  ...\n", tap_buffer->line(10));
}

TEST(TapOutput, ignoredTestEmitsOkWithSkipDirective)
{
  test_case_runner->start()
      .with_group("group")
      .with_ignored_test("skipped")
      .end();
  STRCMP_EQUAL(
      "ok 1 - group.skipped # SKIP SKIPPED_TEST\n", tap_buffer->line(3)
  );
}

TEST(TapOutput, skippedTestEmitsOkWithSkipDirectiveAndMessage)
{
  test_case_runner->start()
      .with_group("group")
      .with_test("dynamic")
      .that_is_skipped("not ready yet")
      .end();
  STRCMP_EQUAL(
      "ok 1 - group.dynamic # SKIP not ready yet\n", tap_buffer->line(3)
  );
}

TEST(TapOutput, testsAcrossGroupsNumberedGlobally)
{
  test_case_runner->start()
      .with_group("groupA")
      .with_test("testA")
      .with_group("groupB")
      .with_test("testB")
      .end();
  STRCMP_EQUAL("1..2\n", tap_buffer->line(2));
  STRCMP_EQUAL("ok 1 - groupA.testA\n", tap_buffer->line(3));
  STRCMP_EQUAL("ok 2 - groupB.testB\n", tap_buffer->line(4));
}

TEST(TapOutput, messageSpecialCharsAreYamlEscaped)
{
  test_case_runner->start()
      .with_group("group")
      .with_test("test")
      .that_fails("has \"quotes\" and \\backslash", "file.cpp", 1)
      .end();
  STRCMP_EQUAL(
      "  message: \"has \\\"quotes\\\" and \\\\backslash\"\n",
      tap_buffer->line(5)
  );
}

TEST(TapOutput, messageNewlineIsYamlEscaped)
{
  test_case_runner->start()
      .with_group("group")
      .with_test("test")
      .that_fails("line1\nline2", "file.cpp", 1)
      .end();
  STRCMP_EQUAL("  message: \"line1\\nline2\"\n", tap_buffer->line(5));
}

TEST(TapOutput, messageCarriageReturnIsYamlEscaped)
{
  test_case_runner->start()
      .with_group("group")
      .with_test("test")
      .that_fails("line1\rline2", "file.cpp", 1)
      .end();
  STRCMP_EQUAL("  message: \"line1\\rline2\"\n", tap_buffer->line(5));
}

TEST(TapOutput, messageTabIsYamlEscaped)
{
  test_case_runner->start()
      .with_group("group")
      .with_test("test")
      .that_fails("col1\tcol2", "file.cpp", 1)
      .end();
  STRCMP_EQUAL("  message: \"col1\\tcol2\"\n", tap_buffer->line(5));
}

TEST(TapOutput, secondRunResetsResults)
{
  test_case_runner->start().with_group("g").with_test("first").end();
  tap_buffer->clear();
  mu::tiny::test::Result second(*output);
  TapTestOutputTestRunner runner2(second);
  runner2.start().with_group("g").with_test("second").end();
  STRCMP_EQUAL("1..1\n", tap_buffer->line(2));
  STRCMP_EQUAL("ok 1 - g.second\n", tap_buffer->line(3));
}

TEST(TapOutput, parseArguments_ptapIsRecognised)
{
  mu::tiny::test::TapOutputPlugin p;
  const char* argv[] = { "", "-ptap" };
  CHECK(p.parse_arguments(2, argv, 1));
  auto* created = p.create_output();
  CHECK(created != nullptr);
  delete created;
}

TEST(TapOutput, parseArguments_otherArgReturnsFalse)
{
  mu::tiny::test::TapOutputPlugin p;
  const char* argv[] = { "", "-pjunit" };
  CHECK(!p.parse_arguments(2, argv, 1));
  CHECK(p.create_output() == nullptr);
}

TEST(TapOutput, getHelpMentionsPtap)
{
  mu::tiny::test::TapOutputPlugin p;
  STRCMP_CONTAINS("-ptap", p.get_help().c_str());
}

TEST(TapOutput, needsNoConsoleCompanion)
{
  CHECK(!output->needs_console_companion());
}

TEST(TapOutput, printBufferIsNoOp)
{
  test_case_runner->start().with_group("group").with_test("test").end();
  tap_buffer->clear();
  output->print_buffer("ignored text");
  STRCMP_EQUAL("", tap_buffer->content().c_str());
}

#if MUTINY_HAVE_EXCEPTIONS
TEST(TapOutput, unexpectedExceptionEmitsNotOkWithCommentSeverity)
{
  test_case_runner->start()
      .with_group("group")
      .with_test("errorTest")
      .that_errors()
      .end();
  STRCMP_EQUAL("not ok 1 - group.errorTest\n", tap_buffer->line(3));
  STRCMP_EQUAL("  severity: comment\n", tap_buffer->line(6));
}
#endif
