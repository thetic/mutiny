#include "CppMu/TestShell.hpp"

#include "CppMu/ConsoleTestOutput.hpp"
#include "CppMu/Test.hpp"
#include "CppMu/TestFailure.hpp"
#include "CppMu/TestPlugin.hpp"
#include "CppMu/TestRegistry.hpp"
#include "CppMu/TestResult.hpp"
#include "CppMu/jump_buffer.h"
#include "CppMu/math.hpp"

#include <math.h>
#include <stdlib.h>

namespace cppmu {

namespace {

/* Sometimes stubs use the CppMu assertions.
 * Its not correct to do so, but this small helper class will prevent a
 * segmentation fault and instead will give an error message and also the
 * file/line of the check that was executed outside the tests.
 */
class OutsideTestRunnerUTest : public TestShell
{
public:
  static OutsideTestRunnerUTest& instance();
  virtual TestResult& get_test_result() { return default_test_result_; }
  ~OutsideTestRunnerUTest() override = default;

private:
  OutsideTestRunnerUTest()
    : TestShell(
          "\n\t NOTE: Assertion happened without being in a test run "
          "(perhaps in "
          "main?)",
          "\n\t       Something is very wrong. Check this assertion and fix",
          "unknown file",
          0
      )
    , default_test_result_(default_output_)
  {
  }
  ConsoleTestOutput default_output_;
  TestResult default_test_result_;
};

OutsideTestRunnerUTest& OutsideTestRunnerUTest::instance()
{
  static OutsideTestRunnerUTest instance;
  return instance;
}

/*
 * Below helpers are used for the PlatformSpecificSetJmp and LongJmp. They pass
 * a method for what needs to happen after the jump, so that the stack stays
 * right.
 *
 */

struct HelperTestRunInfo
{
  HelperTestRunInfo(TestShell* s, TestPlugin* p, TestResult* r)
    : shell(s)
    , plugin(p)
    , result(r)
  {
  }

  TestShell* shell;
  TestPlugin* plugin;
  TestResult* result;
};

void helper_do_run_one_test_in_current_process(void* data)
{
  auto* run_info = static_cast<HelperTestRunInfo*>(data);

  TestShell* shell = run_info->shell;
  TestPlugin* plugin = run_info->plugin;
  TestResult* result = run_info->result;

  shell->run_one_test_in_current_process(plugin, *result);
}

const NormalTestTerminator normal_test_terminator = NormalTestTerminator();
const CrashingTestTerminator crashing_test_terminator =
    CrashingTestTerminator();
const TestTerminatorWithoutExceptions
    normal_test_terminator_without_exceptions =
        TestTerminatorWithoutExceptions();
const CrashingTestTerminatorWithoutExceptions
    crashing_test_terminator_without_exceptions =
        CrashingTestTerminatorWithoutExceptions();

void (*please_crash_me_right_now)() = abort;
} // namespace

bool doubles_equal(double d1, double d2, double threshold)
{
  if (cppmu::is_nan(d1) || cppmu::is_nan(d2) || cppmu::is_nan(threshold))
    return false;

  if (cppmu::is_inf(d1) && cppmu::is_inf(d2)) {
    return true;
  }

  return fabs(d1 - d2) <= threshold;
}

const TestTerminator* TestShell::current_test_terminator_ =
    &normal_test_terminator;
const TestTerminator* TestShell::current_test_terminator_without_exceptions_ =
    &normal_test_terminator_without_exceptions;

bool TestShell::rethrow_exceptions_ = false;

/******************************** */

TestShell::TestShell()
  : group_("UndefinedTestGroup")
  , name_("UndefinedTest")
  , file_("UndefinedFile")
  , line_number_(0)
  , next_(nullptr)
  , has_failed_(false)
{
}

TestShell::TestShell(
    const char* group_name,
    const char* test_name,
    const char* file_name,
    size_t line_number
)
  : group_(group_name)
  , name_(test_name)
  , file_(file_name)
  , line_number_(line_number)
  , next_(nullptr)
  , has_failed_(false)
{
}

void TestShell::set_crash_method(void (*crashme)())
{
  please_crash_me_right_now = crashme;
}

void TestShell::reset_crash_method()
{
  please_crash_me_right_now = abort;
}

void TestShell::crash()
{
  please_crash_me_right_now();
}

void TestShell::run_one_test(TestPlugin* plugin, TestResult& result)
{
  has_failed_ = false;
  result.count_run();
  HelperTestRunInfo run_info(this, plugin, &result);
  cppmu_set_jmp(helper_do_run_one_test_in_current_process, &run_info);
}

Test* TestShell::create_test()
{
  return new Test();
}

void TestShell::destroy_test(Test* test)
{
  delete test;
}

void TestShell::run_one_test_in_current_process(
    TestPlugin* plugin,
    TestResult& result
)
{
  result.print_very_verbose("\n-- before runAllPreTestAction: ");
  plugin->run_all_pre_test_action(*this, result);
  result.print_very_verbose("\n-- after runAllPreTestAction: ");

  // save test context, so that test class can be tested
  TestShell* saved_test = TestShell::get_current();
  TestResult* saved_result = TestShell::get_test_result();

  TestShell::set_test_result(&result);
  TestShell::set_current_test(this);

  Test* test_to_run = nullptr;

#if CPPMU_HAVE_EXCEPTIONS
  try {
#endif
    result.print_very_verbose("\n---- before createTest: ");
    test_to_run = create_test();
    result.print_very_verbose("\n---- after createTest: ");

    result.print_very_verbose("\n------ before runTest: ");
    test_to_run->run();
    result.print_very_verbose("\n------ after runTest: ");

    TestShell::set_current_test(saved_test);
    TestShell::set_test_result(saved_result);
#if CPPMU_HAVE_EXCEPTIONS
  } catch (...) {
    destroy_test(test_to_run);
    throw;
  }
#endif

  result.print_very_verbose("\n---- before destroyTest: ");
  destroy_test(test_to_run);
  result.print_very_verbose("\n---- after destroyTest: ");

  result.print_very_verbose("\n-- before runAllPostTestAction: ");
  plugin->run_all_post_test_action(*this, result);
  result.print_very_verbose("\n-- after runAllPostTestAction: ");
}

TestShell* TestShell::get_next() const
{
  return next_;
}

TestShell* TestShell::add_test(TestShell* test)
{
  next_ = test;
  return this;
}

size_t TestShell::count_tests()
{
  return next_ ? next_->count_tests() + 1 : 1;
}

String TestShell::get_macro_name() const
{
  return "TEST";
}

const String TestShell::get_name() const
{
  return String(name_);
}

const String TestShell::get_group() const
{
  return String(group_);
}

String TestShell::get_formatted_name() const
{
  String formatted_name(get_macro_name());
  formatted_name += "(";
  formatted_name += group_;
  formatted_name += ", ";
  formatted_name += name_;
  formatted_name += ")";

  return formatted_name;
}

bool TestShell::has_failed() const
{
  return has_failed_;
}

void TestShell::count_check()
{
  get_test_result()->count_check();
}

bool TestShell::will_run() const
{
  return true;
}

bool TestShell::is_ordered() const
{
  return false;
}

void TestShell::set_run_ignored() {}

void TestShell::set_file_name(const char* file_name)
{
  file_ = file_name;
}

void TestShell::set_line_number(size_t line_number)
{
  line_number_ = line_number;
}

void TestShell::set_group_name(const char* group_name)
{
  group_ = group_name;
}

void TestShell::set_test_name(const char* test_name)
{
  name_ = test_name;
}

const String TestShell::get_file() const
{
  return String(file_);
}

size_t TestShell::get_line_number() const
{
  return line_number_;
}

bool TestShell::match(const char* target, const TestFilter* filters) const
{
  if (filters == nullptr)
    return true;

  for (; filters != nullptr; filters = filters->get_next())
    if (filters->match(target))
      return true;

  return false;
}

bool TestShell::should_run(
    const TestFilter* group_filters,
    const TestFilter* name_filters
) const
{
  return match(group_, group_filters) && match(name_, name_filters);
}

void TestShell::add_failure(const TestFailure& failure)
{
  has_failed_ = true;
  get_test_result()->add_failure(failure);
}

void TestShell::add_test_property(const char* name, const char* value)
{
  get_test_result()->add_test_property(name, value);
}

void TestShell::exit_test(const TestTerminator& terminator)
{
  terminator.exit_current_test();
}

void TestShell::assert_true(
    bool condition,
    const char* check_string,
    const char* condition_string,
    const char* text,
    const char* file_name,
    size_t line_number,
    const TestTerminator& test_terminator
)
{
  get_test_result()->count_check();
  if (!condition) {
    add_failure(CheckFailure(
        this, file_name, line_number, check_string, condition_string, text
    ));
    test_terminator.exit_current_test();
  }
}

void TestShell::fail(
    const char* text,
    const char* file_name,
    size_t line_number,
    const TestTerminator& test_terminator
)
{
  get_test_result()->count_check();
  add_failure(FailFailure(this, file_name, line_number, text));
  test_terminator.exit_current_test();
}

void TestShell::assert_cstr_equal(
    const char* expected,
    const char* actual,
    const char* text,
    const char* file_name,
    size_t line_number,
    const TestTerminator& test_terminator
)
{
  get_test_result()->count_check();
  if (actual == nullptr && expected == nullptr)
    return;
  if (actual == nullptr || expected == nullptr) {
    add_failure(
        StringEqualFailure(this, file_name, line_number, expected, actual, text)
    );
    test_terminator.exit_current_test();
  }
  if (str_cmp(expected, actual) != 0) {
    add_failure(
        StringEqualFailure(this, file_name, line_number, expected, actual, text)
    );
    test_terminator.exit_current_test();
  }
}

void TestShell::assert_cstr_n_equal(
    const char* expected,
    const char* actual,
    size_t length,
    const char* text,
    const char* file_name,
    size_t line_number,
    const TestTerminator& test_terminator
)
{
  get_test_result()->count_check();
  if (actual == nullptr && expected == nullptr)
    return;
  if (actual == nullptr || expected == nullptr) {
    add_failure(
        StringEqualFailure(this, file_name, line_number, expected, actual, text)
    );
    test_terminator.exit_current_test();
  }
  if (str_n_cmp(expected, actual, length) != 0) {
    add_failure(
        StringEqualFailure(this, file_name, line_number, expected, actual, text)
    );
    test_terminator.exit_current_test();
  }
}

void TestShell::assert_cstr_contains(
    const char* expected,
    const char* actual,
    const char* text,
    const char* file_name,
    size_t line_number
)
{
  get_test_result()->count_check();
  if (actual == nullptr && expected == nullptr)
    return;
  if (actual == nullptr || expected == nullptr) {
    add_failure(ContainsFailure(
        this,
        file_name,
        line_number,
        string_from_or_null(expected),
        string_from_or_null(actual),
        text ? text : ""
    ));
    exit_test(get_current_test_terminator());
    return;
  }
  if (!string_contains(actual, expected)) {
    add_failure(ContainsFailure(
        this, file_name, line_number, expected, actual, text ? text : ""
    ));
    exit_test(get_current_test_terminator());
  }
}

void TestShell::assert_longs_equal(
    long expected,
    long actual,
    const char* text,
    const char* file_name,
    size_t line_number,
    const TestTerminator& test_terminator
)
{
  get_test_result()->count_check();
  if (expected != actual) {
    add_failure(
        LongsEqualFailure(this, file_name, line_number, expected, actual, text)
    );
    test_terminator.exit_current_test();
  }
}

void TestShell::assert_unsigned_longs_equal(
    unsigned long expected,
    unsigned long actual,
    const char* text,
    const char* file_name,
    size_t line_number,
    const TestTerminator& test_terminator
)
{
  get_test_result()->count_check();
  if (expected != actual) {
    add_failure(UnsignedLongsEqualFailure(
        this, file_name, line_number, expected, actual, text
    ));
    test_terminator.exit_current_test();
  }
}

void TestShell::assert_long_longs_equal(
    long long expected,
    long long actual,
    const char* text,
    const char* file_name,
    size_t line_number,
    const TestTerminator& test_terminator
)
{
  get_test_result()->count_check();
  if (expected != actual) {
    add_failure(LongLongsEqualFailure(
        this, file_name, line_number, expected, actual, text
    ));
    test_terminator.exit_current_test();
  }
}

void TestShell::assert_unsigned_long_longs_equal(
    unsigned long long expected,
    unsigned long long actual,
    const char* text,
    const char* file_name,
    size_t line_number,
    const TestTerminator& test_terminator
)
{
  get_test_result()->count_check();
  if (expected != actual) {
    add_failure(UnsignedLongLongsEqualFailure(
        this, file_name, line_number, expected, actual, text
    ));
    test_terminator.exit_current_test();
  }
}

void TestShell::assert_signed_bytes_equal(
    signed char expected,
    signed char actual,
    const char* text,
    const char* file_name,
    size_t line_number,
    const TestTerminator& test_terminator
)
{
  get_test_result()->count_check();
  if (expected != actual) {
    add_failure(SignedBytesEqualFailure(
        this, file_name, line_number, expected, actual, text
    ));
    test_terminator.exit_current_test();
  }
}

void TestShell::assert_pointers_equal(
    const void* expected,
    const void* actual,
    const char* text,
    const char* file_name,
    size_t line_number,
    const TestTerminator& test_terminator
)
{
  get_test_result()->count_check();
  if (expected != actual) {
    add_failure(EqualsFailure(
        this,
        file_name,
        line_number,
        string_from(expected),
        string_from(actual),
        text
    ));
    test_terminator.exit_current_test();
  }
}

void TestShell::assert_function_pointers_equal(
    void (*expected)(),
    void (*actual)(),
    const char* text,
    const char* file_name,
    size_t line_number,
    const TestTerminator& test_terminator
)
{
  get_test_result()->count_check();
  if (expected != actual) {
    add_failure(EqualsFailure(
        this,
        file_name,
        line_number,
        string_from(expected),
        string_from(actual),
        text
    ));
    test_terminator.exit_current_test();
  }
}

void TestShell::assert_doubles_equal(
    double expected,
    double actual,
    double threshold,
    const char* text,
    const char* file_name,
    size_t line_number,
    const TestTerminator& test_terminator
)
{
  get_test_result()->count_check();
  if (!doubles_equal(expected, actual, threshold)) {
    add_failure(DoublesEqualFailure(
        this, file_name, line_number, expected, actual, threshold, text
    ));
    test_terminator.exit_current_test();
  }
}

void TestShell::assert_binary_equal(
    const void* expected,
    const void* actual,
    size_t length,
    const char* text,
    const char* file_name,
    size_t line_number,
    const TestTerminator& test_terminator
)
{
  get_test_result()->count_check();
  if (length == 0)
    return;
  if (actual == nullptr && expected == nullptr)
    return;
  if (actual == nullptr || expected == nullptr) {
    add_failure(BinaryEqualFailure(
        this,
        file_name,
        line_number,
        static_cast<const unsigned char*>(expected),
        static_cast<const unsigned char*>(actual),
        length,
        text
    ));
    test_terminator.exit_current_test();
  }
  if (mem_cmp(expected, actual, length) != 0) {
    add_failure(BinaryEqualFailure(
        this,
        file_name,
        line_number,
        static_cast<const unsigned char*>(expected),
        static_cast<const unsigned char*>(actual),
        length,
        text
    ));
    test_terminator.exit_current_test();
  }
}

void TestShell::assert_equals(
    bool failed,
    const char* expected,
    const char* actual,
    const char* text,
    const char* file,
    size_t line,
    const TestTerminator& test_terminator
)
{
  get_test_result()->count_check();
  if (failed) {
    add_failure(CheckEqualFailure(this, file, line, expected, actual, text));
    test_terminator.exit_current_test();
  }
}

void TestShell::assert_equals(
    bool failed,
    String expected,
    String actual,
    const char* text,
    const char* file,
    size_t line,
    const TestTerminator& test_terminator
)
{
  get_test_result()->count_check();
  if (failed) {
    add_failure(CheckEqualFailure(
        this, file, line, expected.c_str(), actual.c_str(), text
    ));
    expected.clear();
    actual.clear();
    test_terminator.exit_current_test();
  }
}

void TestShell::assert_compare(
    bool comparison,
    const char* check_string,
    const char* comparison_string,
    const char* text,
    const char* file_name,
    size_t line_number,
    const TestTerminator& test_terminator
)
{
  get_test_result()->count_check();
  if (!comparison) {
    add_failure(ComparisonFailure(
        this, file_name, line_number, check_string, comparison_string, text
    ));
    test_terminator.exit_current_test();
  }
}

void TestShell::print(
    const char* text,
    const char* file_name,
    size_t line_number
)
{
  String string_to_print = "\n";
  string_to_print += file_name;
  string_to_print += ":";
  string_to_print += string_from(line_number);
  string_to_print += " ";
  string_to_print += text;
  get_test_result()->print(string_to_print.c_str());
}

void TestShell::print_very_verbose(const char* text)
{
  get_test_result()->print_very_verbose(text);
}

TestResult* TestShell::test_result_ = nullptr;
TestShell* TestShell::current_test_ = nullptr;

void TestShell::set_test_result(TestResult* result)
{
  test_result_ = result;
}

void TestShell::set_current_test(TestShell* test)
{
  current_test_ = test;
}

TestResult* TestShell::get_test_result()
{
  if (test_result_ == nullptr)
    return &OutsideTestRunnerUTest::instance().get_test_result();
  return test_result_;
}

TestShell* TestShell::get_current()
{
  if (current_test_ == nullptr)
    return &OutsideTestRunnerUTest::instance();
  return current_test_;
}

const TestTerminator& TestShell::get_current_test_terminator()
{
  return *current_test_terminator_;
}

const TestTerminator& TestShell::
    get_current_test_terminator_without_exceptions()
{
  return *current_test_terminator_without_exceptions_;
}

void TestShell::set_crash_on_fail()
{
  current_test_terminator_ = &crashing_test_terminator;
  current_test_terminator_without_exceptions_ =
      &crashing_test_terminator_without_exceptions;
}

void TestShell::restore_default_test_terminator()
{
  current_test_terminator_ = &normal_test_terminator;
  current_test_terminator_without_exceptions_ =
      &normal_test_terminator_without_exceptions;
}

void TestShell::set_rethrow_exceptions(bool rethrow_exceptions)
{
  rethrow_exceptions_ = rethrow_exceptions;
}

bool TestShell::is_rethrowing_exceptions()
{
  return rethrow_exceptions_;
}

} // namespace cppmu
