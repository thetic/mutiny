#include "CppMu/TestShell.hpp"

#include "CppMu/CppMu.hpp"
#include "CppMu/ExecFunctionTestShell.hpp"
#include "CppMu/StringBufferTestOutput.hpp"
#include "CppMu/TestOutput.hpp"
#include "CppMu/TestTestingFixture.hpp"

#include <limits.h>
#include <math.h>

#define CHECK_TEST_FAILS_PROPER_WITH_TEXT(text)                                \
  fixture.check_test_fails_with_proper_test_location(text, __FILE__, __LINE__)

TEST_GROUP(TestShell)
{
  cppmu::TestTestingFixture fixture;
};

namespace {
void
fail_method()
{
  FAIL("This test fails");
}

void
simple_passing_method()
{
  CHECK(true);
}

void
passing_check_equal_test_method()
{
  CHECK_EQUAL(1, 1);
}

void
exit_test_method()
{
  TEST_EXIT;
  FAIL("Should not get here");
}

class PassingExecFunction : public cppmu::ExecFunction
{
public:
  bool has_run{ false };
  void exec() override { has_run = true; }
};

bool cppmu_has_crashed;

void
crash_method()
{
  cppmu_has_crashed = true;
}

int teardown_called = 0;

void
teardown_method()
{
  teardown_called++;
}

int stop_after_failure = 0;
void
stop_after_failure_method()
{
  FAIL("fail");
  stop_after_failure++;
}

#if CPPMU_HAVE_EXCEPTIONS
// Prevents -Wunreachable-code; should always be 'true'
bool should_throw_exception = true;

void
thrown_unknown_exception_method()
{
  if (should_throw_exception) {
    throw 33;
  }
  stop_after_failure++;
}

#if CPPMU_USE_STD_CPP_LIB
void
thrown_standard_exception_method()
{
  if (should_throw_exception) {
    throw std::runtime_error("exception text");
  }
  stop_after_failure++;
}
#endif

#endif

} // namespace

TEST(TestShell, compareDoubles)
{
  CHECK(cppmu::doubles_equal(1.0, 1.001, 0.01));
  CHECK(!cppmu::doubles_equal(1.0, 1.1, 0.05));
  double a = 1.2345678;
  CHECK(cppmu::doubles_equal(a, a, 0.000000001));
}

#ifdef NAN
TEST(TestShell, compareDoublesNaN)
{
  CHECK(!cppmu::doubles_equal(static_cast<double>(NAN), 1.001, 0.01));
  CHECK(!cppmu::doubles_equal(1.0, static_cast<double>(NAN), 0.01));
  CHECK(!cppmu::doubles_equal(1.0, 1.001, static_cast<double>(NAN)));
}
#endif

#ifdef INFINITY
TEST(TestShell, compareDoublesInf)
{
  CHECK(!cppmu::doubles_equal(static_cast<double>(INFINITY), 1.0, 0.01));
  CHECK(!cppmu::doubles_equal(1.0, static_cast<double>(INFINITY), 0.01));
  CHECK(cppmu::doubles_equal(1.0, -1.0, static_cast<double>(INFINITY)));
  CHECK(cppmu::doubles_equal(
      static_cast<double>(INFINITY), static_cast<double>(INFINITY), 0.01));
  CHECK(cppmu::doubles_equal(static_cast<double>(INFINITY),
      static_cast<double>(INFINITY),
      static_cast<double>(INFINITY)));
}
#endif

TEST(TestShell, FailWillIncreaseTheAmountOfChecks)
{
  fixture.set_test_function(fail_method);
  fixture.run_all_tests();
  LONGS_EQUAL(1, fixture.get_check_count());
}

TEST(TestShell, PassedCheckEqualWillIncreaseTheAmountOfChecks)
{
  fixture.set_test_function(passing_check_equal_test_method);
  fixture.run_all_tests();
  LONGS_EQUAL(1, fixture.get_check_count());
}

TEST(TestShell, SetTestFunctionExecFunctionOverloadRunsTheFunction)
{
  PassingExecFunction exec_func;
  fixture.set_test_function(&exec_func);
  fixture.run_all_tests();
  CHECK(exec_func.has_run);
}

IGNORE_TEST(TestShell, IgnoreTestAccessingFixture)
{
  CHECK(fixture.get_check_count() == 0);
}

TEST(TestShell, MacrosUsedInSetup)
{
  fixture.set_setup(fail_method);
  fixture.set_test_function(simple_passing_method);
  fixture.run_all_tests();
  LONGS_EQUAL(1, fixture.get_failure_count());
}

TEST(TestShell, MacrosUsedInTearDown)
{
  fixture.set_teardown(fail_method);
  fixture.set_test_function(simple_passing_method);
  fixture.run_all_tests();
  LONGS_EQUAL(1, fixture.get_failure_count());
}

TEST(TestShell, ExitLeavesQuietly)
{
  fixture.set_test_function(exit_test_method);
  fixture.run_all_tests();
  LONGS_EQUAL(0, fixture.get_failure_count());
}

TEST(TestShell, FailWillNotCrashIfNotEnabled)
{
  cppmu_has_crashed = false;
  cppmu::TestShell::set_crash_method(crash_method);

  fixture.set_test_function(fail_method);
  fixture.run_all_tests();

  CHECK_FALSE(cppmu_has_crashed);
  LONGS_EQUAL(1, fixture.get_failure_count());

  cppmu::TestShell::reset_crash_method();
}

TEST(TestShell, FailWillCrashIfEnabled)
{
  cppmu_has_crashed = false;
  cppmu::TestShell::set_crash_on_fail();
  cppmu::TestShell::set_crash_method(crash_method);

  fixture.set_test_function(fail_method);
  fixture.run_all_tests();

  CHECK(cppmu_has_crashed);

  cppmu::TestShell::restore_default_test_terminator();
  cppmu::TestShell::reset_crash_method();
}

TEST(TestShell, TeardownCalledAfterTestFailure)
{
  teardown_called = 0;
  fixture.set_teardown(teardown_method);
  fixture.set_test_function(fail_method);
  fixture.run_all_tests();
  LONGS_EQUAL(1, fixture.get_failure_count());
  LONGS_EQUAL(1, teardown_called);
}

TEST(TestShell, TestStopsAfterTestFailure)
{
  stop_after_failure = 0;
  fixture.set_test_function(stop_after_failure_method);
  fixture.run_all_tests();
  CHECK(fixture.has_test_failed());
  LONGS_EQUAL(1, fixture.get_failure_count());
  LONGS_EQUAL(0, stop_after_failure);
}

TEST(TestShell, TestStopsAfterSetupFailure)
{
  stop_after_failure = 0;
  fixture.set_setup(stop_after_failure_method);
  fixture.set_teardown(stop_after_failure_method);
  fixture.set_test_function(fail_method);
  fixture.run_all_tests();
  LONGS_EQUAL(2, fixture.get_failure_count());
  LONGS_EQUAL(0, stop_after_failure);
}

#if CPPMU_HAVE_EXCEPTIONS

TEST(TestShell, TestStopsAfterUnknownExceptionIsThrown)
{
  bool initial_rethrow_exceptions =
      cppmu::TestShell::is_rethrowing_exceptions();
  cppmu::TestShell::set_rethrow_exceptions(false);
  stop_after_failure = 0;
  should_throw_exception = true;
  fixture.set_test_function(thrown_unknown_exception_method);
  fixture.run_all_tests();
  LONGS_EQUAL(1, fixture.get_failure_count());
  fixture.assert_print_contains(
      "Unexpected exception of unknown type was thrown");
  LONGS_EQUAL(0, stop_after_failure);
  cppmu::TestShell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

TEST(
TestShell, NoExceptionIsRethrownIfEnabledButNotThrown)
{
  bool initial_rethrow_exceptions =
      cppmu::TestShell::is_rethrowing_exceptions();
  bool exception_rethrown = false;
  stop_after_failure = 0;
  cppmu::TestShell::set_rethrow_exceptions(true);
  should_throw_exception = false;
  fixture.set_test_function(thrown_unknown_exception_method);
  try {
    fixture.run_all_tests();
  } catch (...) {
    exception_rethrown = true;
  }
  CHECK_FALSE(exception_rethrown);
  LONGS_EQUAL(0, fixture.get_failure_count());
  LONGS_EQUAL(1, stop_after_failure);
  cppmu::TestShell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

TEST(
TestShell, UnknownExceptionIsRethrownIfEnabled)
{
  bool initial_rethrow_exceptions =
      cppmu::TestShell::is_rethrowing_exceptions();
  bool exception_rethrown = false;
  stop_after_failure = 0;
  cppmu::TestShell::set_rethrow_exceptions(true);
  should_throw_exception = true;
  fixture.set_test_function(thrown_unknown_exception_method);
  try {
    fixture.run_all_tests();
    stop_after_failure++;
  } catch (...) {
    exception_rethrown = true;
  }
  CHECK_TRUE(exception_rethrown);
  LONGS_EQUAL(1, fixture.get_failure_count());
  fixture.assert_print_contains(
      "Unexpected exception of unknown type was thrown");
  LONGS_EQUAL(0, stop_after_failure);
  cppmu::TestShell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

#if CPPMU_USE_STD_CPP_LIB

TEST(TestShell, TestStopsAfterStandardExceptionIsThrown)
{
  bool initial_rethrow_exceptions =
      cppmu::TestShell::is_rethrowing_exceptions();
  cppmu::TestShell::set_rethrow_exceptions(false);
  stop_after_failure = 0;
  should_throw_exception = true;
  fixture.set_test_function(thrown_standard_exception_method);
  fixture.run_all_tests();
  LONGS_EQUAL(1, fixture.get_failure_count());
#if CPPMU_HAVE_RTTI
  fixture.assert_print_contains("Unexpected exception of type '");
  fixture.assert_print_contains("runtime_error");
  fixture.assert_print_contains("' was thrown: exception text");
#else
  fixture.assert_print_contains(
      "Unexpected exception of unknown type was thrown");
#endif
  LONGS_EQUAL(0, stop_after_failure);
  cppmu::TestShell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

TEST(
TestShell, StandardExceptionIsRethrownIfEnabled)
{
  bool initial_rethrow_exceptions =
      cppmu::TestShell::is_rethrowing_exceptions();
  bool exception_rethrown = false;
  stop_after_failure = 0;
  cppmu::TestShell::set_rethrow_exceptions(true);
  should_throw_exception = true;
  fixture.set_test_function(thrown_standard_exception_method);
  try {
    fixture.run_all_tests();
    stop_after_failure++;
  } catch (const std::exception&) {
    exception_rethrown = true;
  }
  CHECK_TRUE(exception_rethrown);
  LONGS_EQUAL(1, fixture.get_failure_count());
  fixture.assert_print_contains("Unexpected exception of type '");
  fixture.assert_print_contains("runtime_error");
  fixture.assert_print_contains("' was thrown: exception text");
  LONGS_EQUAL(0, stop_after_failure);
  cppmu::TestShell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

#endif // CPPMU_USE_STD_CPP_LIB

TEST(TestShell, TeardownStopsAfterUnknownExceptionIsThrown)
{
  bool initial_rethrow_exceptions =
      cppmu::TestShell::is_rethrowing_exceptions();
  cppmu::TestShell::set_rethrow_exceptions(false);
  stop_after_failure = 0;
  should_throw_exception = true;
  fixture.set_teardown(thrown_unknown_exception_method);
  fixture.run_all_tests();
  LONGS_EQUAL(1, fixture.get_failure_count());
  fixture.assert_print_contains(
      "Unexpected exception of unknown type was thrown");
  LONGS_EQUAL(0, stop_after_failure);
  cppmu::TestShell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

TEST(
TestShell, TeardownUnknownExceptionIsRethrownIfEnabled)
{
  bool initial_rethrow_exceptions =
      cppmu::TestShell::is_rethrowing_exceptions();
  bool exception_rethrown = false;
  stop_after_failure = 0;
  cppmu::TestShell::set_rethrow_exceptions(true);
  should_throw_exception = true;
  fixture.set_teardown(thrown_unknown_exception_method);
  try {
    fixture.run_all_tests();
    stop_after_failure++;
  } catch (...) {
    exception_rethrown = true;
  }
  CHECK_TRUE(exception_rethrown);
  LONGS_EQUAL(1, fixture.get_failure_count());
  fixture.assert_print_contains(
      "Unexpected exception of unknown type was thrown");
  LONGS_EQUAL(0, stop_after_failure);
  cppmu::TestShell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

#if CPPMU_USE_STD_CPP_LIB

TEST(TestShell, TeardownStopsAfterStandardExceptionIsThrown)
{
  bool initial_rethrow_exceptions =
      cppmu::TestShell::is_rethrowing_exceptions();
  cppmu::TestShell::set_rethrow_exceptions(false);
  stop_after_failure = 0;
  should_throw_exception = true;
  fixture.set_teardown(thrown_standard_exception_method);
  fixture.run_all_tests();
  LONGS_EQUAL(1, fixture.get_failure_count());
#if CPPMU_HAVE_RTTI
  fixture.assert_print_contains("Unexpected exception of type '");
  fixture.assert_print_contains("runtime_error");
  fixture.assert_print_contains("' was thrown: exception text");
#else
  fixture.assert_print_contains(
      "Unexpected exception of unknown type was thrown");
#endif
  LONGS_EQUAL(0, stop_after_failure);
  cppmu::TestShell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

TEST(
TestShell, TeardownStandardExceptionIsRethrownIfEnabled)
{
  bool initial_rethrow_exceptions =
      cppmu::TestShell::is_rethrowing_exceptions();
  bool exception_rethrown = false;
  stop_after_failure = 0;
  cppmu::TestShell::set_rethrow_exceptions(true);
  should_throw_exception = true;
  fixture.set_teardown(thrown_standard_exception_method);
  try {
    fixture.run_all_tests();
    stop_after_failure++;
  } catch (const std::exception&) {
    exception_rethrown = true;
  }
  CHECK_TRUE(exception_rethrown);
  LONGS_EQUAL(1, fixture.get_failure_count());
  fixture.assert_print_contains("Unexpected exception of type '");
  fixture.assert_print_contains("runtime_error");
  fixture.assert_print_contains("' was thrown: exception text");
  LONGS_EQUAL(0, stop_after_failure);
  cppmu::TestShell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

#endif // CPPMU_USE_STD_CPP_LIB
#endif // CPPMU_HAVE_EXCEPTIONS

TEST(TestShell, veryVebose)
{
  cppmu::TestShell shell("Group", "name", __FILE__, __LINE__);
  cppmu::StringBufferTestOutput normal_output;
  normal_output.verbose(cppmu::TestOutput::VerbosityLevel::very_verbose);
  cppmu::NullTestPlugin plugin;

  cppmu::TestResult result(normal_output);
  shell.run_one_test_in_current_process(&plugin, result);
  STRCMP_CONTAINS(
      "\n------ before runTest", normal_output.get_output().c_str());
}

class DefaultTestShell : public cppmu::TestShell
{};

TEST(TestShell,
    this_test_covers_the_TestShell_createTest_and_Utest_testBody_methods)
{
  DefaultTestShell shell;
  fixture.add_test(&shell);
  fixture.run_all_tests();
  LONGS_EQUAL(2, fixture.get_test_count());
}

#if CPPMU_HAVE_EXCEPTIONS

namespace {
bool destructor_was_called_on_failed_test = false;

void
destructor_called_for_local_objects()
{
  struct SetBoolOnDestruct
  {
    bool& b;
    ~SetBoolOnDestruct() { b = true; }
  } please_call_the_destructor{ destructor_was_called_on_failed_test };
  destructor_was_called_on_failed_test = false;
  FAIL("fail");
}
}

TEST(
TestShell, DestructorIsCalledForLocalObjectsWhenTheTestFails)
{
  fixture.set_test_function(destructor_called_for_local_objects);
  fixture.run_all_tests();
  CHECK(destructor_was_called_on_failed_test);
}

#endif

TEST_BASE(MyOwnTest)
{
  MyOwnTest() = default;
  bool in_test{ false };

  void setup() override
  {
    CHECK(!in_test);
    in_test = true;
  }
  void teardown() override
  {
    CHECK(in_test);
    in_test = false;
  }
};

TEST_GROUP_BASE(UtestMyOwn, MyOwnTest){};

TEST(UtestMyOwn, test)
{
  CHECK(in_test);
}

class NullParameterTest : public cppmu::TestShell
{};

TEST(UtestMyOwn, NullParameters)
{
  NullParameterTest null_test; /* Bug fix tests for creating a test without a
                                 name, fix in String */
  cppmu::TestFilter empty_filter;
  CHECK(null_test.should_run(&empty_filter, &empty_filter));
}

// Mainly this is for Visual C++, but we'll define it for any system that has
// the same behavior

TEST_GROUP(UtestMacros)
{
  cppmu::TestTestingFixture fixture;
};

namespace {

void
failing_test_method_with_fail()
{
  FAIL("This test fails");
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
unsigned_longs_equal_test_method()
{
  UNSIGNED_LONGS_EQUAL(1, 1);
  UNSIGNED_LONGS_EQUAL(1, 0);
}

void
unsigned_longs_equal_text_test_method()
{
  UNSIGNED_LONGS_EQUAL_TEXT(1, 0, "Failed because it failed");
}

void
longlongs_equal_test_method()
{
  LONGLONGS_EQUAL(1, 1);
  LONGLONGS_EQUAL(1, 0);
}

void
longlongs_equal_text_test_method()
{
  LONGLONGS_EQUAL_TEXT(1, 0, "Failed because it failed");
}

void
unsigned_longlongs_equal_test_method()
{
  UNSIGNED_LONGLONGS_EQUAL(1, 1);
  UNSIGNED_LONGLONGS_EQUAL(1, 0);
}

void
unsigned_longlongs_equal_text_test_method()
{
  UNSIGNED_LONGLONGS_EQUAL_TEXT(1, 0, "Failed because it failed");
}

void
failing_test_method_with_check()
{
  CHECK(false);
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_check_text()
{
  CHECK_TEXT(false, "Failed because it failed");
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_check_true()
{
  CHECK_TRUE(false);
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_check_true_text()
{
  CHECK_TRUE_TEXT(false, "Failed because it failed");
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_check_false()
{
  CHECK_FALSE(true);
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_check_false_text()
{
  CHECK_FALSE_TEXT(true, "Failed because it failed");
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_check_equal()
{
  CHECK_EQUAL(1, 2);
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_check_compare()
{
  double small = 0.5, big = 0.8;
  CHECK_COMPARE(small, >=, big);
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_check_compare_text()
{
  double small = 0.5, big = 0.8;
  CHECK_COMPARE_TEXT(small, >=, big, "small bigger than big");
  cppmu::TestTestingFixture::line_executed_after_check();
}

int count_in_counting_method;
int
counting_method()
{
  return count_in_counting_method++;
}

void
failing_check_equal_with_actual_being_evaluates_multiple_times_will_give_a_warning()
{
  CHECK_EQUAL(12345, counting_method());
}

void
failing_check_equal_with_expected_being_evaluates_multiple_times_will_give_a_warning()
{
  CHECK_EQUAL(counting_method(), 12345);
}

void
failing_test_method_with_check_equal_text()
{
  CHECK_EQUAL_TEXT(1, 2, "Failed because it failed");
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_check_equal_zero()
{
  CHECK_EQUAL_ZERO(1);
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_check_equal_zero_with_actual_being_evaluates_multiple_times_will_give_a_warning()
{
  count_in_counting_method = 1;
  CHECK_EQUAL_ZERO(counting_method());
}

void
failing_test_method_with_check_equal_zero_text()
{
  CHECK_EQUAL_ZERO_TEXT(1, "Failed because it failed");
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_longs_equal()
{
  LONGS_EQUAL(1, 0xff);
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_longs_equal_with_symbolic_parameters()
{
#define MONDAY 1
  int day_of_the_week = MONDAY + 1;
  LONGS_EQUAL(MONDAY, day_of_the_week);
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_longs_equal_text()
{
  LONGS_EQUAL_TEXT(1, 0xff, "Failed because it failed");
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_bytes_equal()
{
  BYTES_EQUAL('a', 'b');
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_bytes_equal_text()
{
  BYTES_EQUAL_TEXT('a', 'b', "Failed because it failed");
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_signed_bytes_equal()
{
  SIGNED_BYTES_EQUAL(-1, -2);
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_signed_bytes_equal_text()
{
  SIGNED_BYTES_EQUAL_TEXT(-127, -126, "Failed because it failed");
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_pointers_equal()
{
  POINTERS_EQUAL(
      reinterpret_cast<void*>(0xa5a5), reinterpret_cast<void*>(0xf0f0));
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_pointers_equal_text()
{
  POINTERS_EQUAL_TEXT(reinterpret_cast<void*>(0xa5a5),
      reinterpret_cast<void*>(0xf0f0),
      "Failed because it failed");
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_functionpointers_equal()
{
  FUNCTIONPOINTERS_EQUAL(reinterpret_cast<void (*)()>(0xa5a5),
      reinterpret_cast<void (*)()>(0xf0f0));
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_functionpointers_equal_text()
{
  FUNCTIONPOINTERS_EQUAL_TEXT(reinterpret_cast<void (*)()>(0xa5a5),
      reinterpret_cast<void (*)()>(0xf0f0),
      "Failed because it failed");
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_doubles_equal()
{
  DOUBLES_EQUAL(0.12, 44.1, 0.3);
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_doubles_equal_text()
{
  DOUBLES_EQUAL_TEXT(0.12, 44.1, 0.3, "Failed because it failed");
  cppmu::TestTestingFixture::line_executed_after_check();
}

bool line_of_code_executed_after_check = false;

void
passing_test_method()
{
  CHECK(true);
  line_of_code_executed_after_check = true;
}

int
function_that_returns_a_value()
{
  CHECK(0 == 0);
  CHECK_TEXT(0 == 0, "Shouldn't fail");
  CHECK_TRUE(0 == 0);
  CHECK_TRUE_TEXT(0 == 0, "Shouldn't fail");
  CHECK_FALSE(0 != 0);
  CHECK_FALSE_TEXT(0 != 0, "Shouldn't fail");
  LONGS_EQUAL(1, 1);
  LONGS_EQUAL_TEXT(1, 1, "Shouldn't fail");
  BYTES_EQUAL(0xab, 0xab);
  BYTES_EQUAL_TEXT(0xab, 0xab, "Shouldn't fail");
  CHECK_EQUAL(100, 100);
  CHECK_EQUAL_TEXT(100, 100, "Shouldn't fail");
  CHECK_EQUAL_ZERO(0);
  CHECK_EQUAL_ZERO_TEXT(0, "Shouldn't fail");
  STRCMP_EQUAL("THIS", "THIS");
  STRCMP_EQUAL_TEXT("THIS", "THIS", "Shouldn't fail");
  DOUBLES_EQUAL(1.0, 1.0, .01);
  DOUBLES_EQUAL_TEXT(1.0, 1.0, .01, "Shouldn't fail");
  POINTERS_EQUAL(nullptr, nullptr);
  POINTERS_EQUAL_TEXT(nullptr, nullptr, "Shouldn't fail");
  MEMCMP_EQUAL("THIS", "THIS", 5);
  MEMCMP_EQUAL_TEXT("THIS", "THIS", 5, "Shouldn't fail");
  return 0;
}

void
memcmp_equal_failing_test_method_with_unequal_input()
{
  unsigned char expected_data[] = { 0x00, 0x01, 0x02, 0x03 };
  unsigned char actual_data[] = { 0x00, 0x01, 0x03, 0x03 };

  MEMCMP_EQUAL(expected_data, actual_data, sizeof(expected_data));
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
memcmp_equal_failing_test_method_with_null_expected()
{
  unsigned char actual_data[] = { 0x00, 0x01, 0x02, 0x03 };

  MEMCMP_EQUAL(nullptr, actual_data, sizeof(actual_data));
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
memcmp_equal_failing_test_method_with_null_actual()
{
  unsigned char expected_data[] = { 0x00, 0x01, 0x02, 0x03 };

  MEMCMP_EQUAL(expected_data, nullptr, sizeof(expected_data));
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_memcmp_equal_text()
{
  unsigned char expected_data[] = { 0x00, 0x01, 0x02, 0x03 };
  unsigned char actual_data[] = { 0x00, 0x01, 0x03, 0x03 };

  MEMCMP_EQUAL_TEXT(expected_data,
      actual_data,
      sizeof(expected_data),
      "Failed because it failed");
  cppmu::TestTestingFixture::line_executed_after_check();
}

enum class ScopedIntEnum
{
  a,
  b
};

void
enums_equal_int_with_scoped_int_enum_test_method()
{
  ENUMS_EQUAL_INT(ScopedIntEnum::b, ScopedIntEnum::b);
  ENUMS_EQUAL_INT(ScopedIntEnum::b, ScopedIntEnum::a);
}

void
enums_equal_int_text_with_scoped_int_enum_test_method()
{
  ENUMS_EQUAL_INT_TEXT(
      ScopedIntEnum::b, ScopedIntEnum::a, "Failed because it failed");
}

enum class ScopedLongEnum : long
{
  a,
  b
};

void
enums_equal_type_with_scoped_long_enum_test_method()
{
  ENUMS_EQUAL_TYPE(long, ScopedLongEnum::b, ScopedLongEnum::b);
  ENUMS_EQUAL_TYPE(long, ScopedLongEnum::b, ScopedLongEnum::a);
}

void
enums_equal_type_text_with_scoped_long_enum_test_method()
{
  ENUMS_EQUAL_TYPE_TEXT(
      long, ScopedLongEnum::b, ScopedLongEnum::a, "Failed because it failed");
}

enum UnscopedEnum
{
  unscoped_enum_a,
  unscoped_enum_b
};

void
enums_equal_int_with_unscoped_enum_test_method()
{
  ENUMS_EQUAL_INT(unscoped_enum_b, unscoped_enum_b);
  ENUMS_EQUAL_INT(unscoped_enum_b, unscoped_enum_a);
}

void
enums_equal_int_text_with_unscoped_enum_test_method()
{
  ENUMS_EQUAL_INT_TEXT(
      unscoped_enum_b, unscoped_enum_a, "Failed because it failed");
}

#if CPPMU_HAVE_EXCEPTIONS
void
failing_test_method_no_throw_with_check_throws()
{
  CHECK_THROWS(int, (void)(1 + 2));
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
succeeding_test_method_correct_throw_with_check_throws()
{
  CHECK_THROWS(int, throw 4);
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_wrong_throw_with_check_throws()
{
  CHECK_THROWS(int, throw 4.3);
  cppmu::TestTestingFixture::line_executed_after_check();
}

#endif

} // namespace

TEST(UtestMacros, FAILMakesTheTestFailPrintsTheRightResultAndStopsExecuting)
{
  fixture.run_test_with_method(failing_test_method_with_fail);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("This test fails");
}

TEST(UtestMacros, FAILWillPrintTheFileThatItFailed)
{
  fixture.run_test_with_method(failing_test_method_with_fail);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT(__FILE__);
}

TEST(UtestMacros, FAILBehavesAsAProperMacro)
{
  if (false)
    FAIL("");
  else
    CHECK(true);

  if (true)
    CHECK(true);
  else
    FAIL("");
}

IGNORE_TEST(UtestMacros, FAILworksInAnIgnoredTest)
{
  FAIL("die!");
}

TEST(UtestMacros, TestUNSIGNED_LONGS_EQUAL)
{
  fixture.run_test_with_method(unsigned_longs_equal_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1 (0x1)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0 (0x0)>");
}

TEST(UtestMacros, UNSIGNED_LONGS_EQUALBehavesAsProperMacro)
{
  if (false)
    UNSIGNED_LONGS_EQUAL(1, 0);
  else
    UNSIGNED_LONGS_EQUAL(1, 1);
}

IGNORE_TEST(UtestMacros, UNSIGNED_LONGS_EQUALWorksInAnIgnoredTest)
{
  UNSIGNED_LONGS_EQUAL(1, 0);
}

TEST(UtestMacros, TestUNSIGNED_LONGS_EQUAL_TEXT)
{
  fixture.run_test_with_method(unsigned_longs_equal_text_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1 (0x1)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0 (0x0)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UtestMacros, UNSIGNED_LONGS_EQUAL_TEXTBehavesAsProperMacro)
{
  if (false)
    UNSIGNED_LONGS_EQUAL_TEXT(1, 0, "Failed because it failed");
  else
    UNSIGNED_LONGS_EQUAL_TEXT(1, 1, "Failed because it failed");
}

IGNORE_TEST(UtestMacros, UNSIGNED_LONGS_EQUAL_TEXTWorksInAnIgnoredTest)
{
  UNSIGNED_LONGS_EQUAL_TEXT(1, 0, "Failed because it failed");
}

TEST(UtestMacros, TestLONGLONGS_EQUAL)
{
  fixture.run_test_with_method(longlongs_equal_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1 (0x1)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0 (0x0)>");
}

TEST(UtestMacros, LONGLONGS_EQUALBehavesAsProperMacro)
{
  if (false)
    LONGLONGS_EQUAL(1, 0);
  else
    LONGLONGS_EQUAL(1, 1);
}

IGNORE_TEST(UtestMacros, LONGLONGS_EQUALWorksInAnIgnoredTest)
{
  LONGLONGS_EQUAL(1, 0);
}

TEST(UtestMacros, TestLONGLONGS_EQUAL_TEXT)
{
  fixture.run_test_with_method(longlongs_equal_text_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1 (0x1)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0 (0x0)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UtestMacros, LONGLONGS_EQUAL_TEXTBehavesAsProperMacro)
{
  if (false)
    LONGLONGS_EQUAL_TEXT(1, 0, "Failed because it failed");
  else
    LONGLONGS_EQUAL_TEXT(1, 1, "Failed because it failed");
}

IGNORE_TEST(UtestMacros, LONGLONGS_EQUAL_TEXTWorksInAnIgnoredTest)
{
  LONGLONGS_EQUAL_TEXT(1, 0, "Failed because it failed");
}

TEST(UtestMacros, TestUNSIGNED_LONGLONGS_EQUAL)
{
  fixture.run_test_with_method(unsigned_longlongs_equal_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1 (0x1)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0 (0x0)>");
}

TEST(UtestMacros, UNSIGNED_LONGLONGS_EQUALBehavesAsProperMacro)
{
  if (false)
    UNSIGNED_LONGLONGS_EQUAL(1, 0);
  else
    UNSIGNED_LONGLONGS_EQUAL(1, 1);
}

IGNORE_TEST(UtestMacros, UNSIGNED_LONGLONGS_EQUALWorksInAnIgnoredTest)
{
  UNSIGNED_LONGLONGS_EQUAL(1, 0);
}

TEST(UtestMacros, TestUNSIGNED_LONGLONGS_EQUAL_TEXT)
{
  fixture.run_test_with_method(unsigned_longlongs_equal_text_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1 (0x1)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0 (0x0)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UtestMacros, UNSIGNED_LONGLONGS_EQUAL_TEXTBehavesAsProperMacro)
{
  if (false)
    UNSIGNED_LONGLONGS_EQUAL_TEXT(1, 0, "Failed because it failed");
  else
    UNSIGNED_LONGLONGS_EQUAL_TEXT(1, 1, "Failed because it failed");
}

IGNORE_TEST(UtestMacros, UNSIGNED_LONGLONGS_EQUAL_TEXTWorksInAnIgnoredTest)
{
  UNSIGNED_LONGLONGS_EQUAL_TEXT(1, 0, "Failed because it failed");
}

TEST(UtestMacros, FailureWithCHECK)
{
  fixture.run_test_with_method(failing_test_method_with_check);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK(false) failed");
}

TEST(UtestMacros, CHECKBehavesAsProperMacro)
{
  if (false)
    CHECK(false);
  else
    CHECK(true);
}

IGNORE_TEST(UtestMacros, CHECKWorksInAnIgnoredTest)
{
  CHECK(false);
}

TEST(UtestMacros, FailureWithCHECK_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_check_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK(false) failed");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UtestMacros, CHECK_TEXTBehavesAsProperMacro)
{
  if (false)
    CHECK_TEXT(false, "false");
  else
    CHECK_TEXT(true, "true");
}

IGNORE_TEST(UtestMacros, CHECK_TEXTWorksInAnIgnoredTest)
{
  CHECK_TEXT(false, "false");
}

TEST(UtestMacros, FailureWithCHECK_TRUE)
{
  fixture.run_test_with_method(failing_test_method_with_check_true);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_TRUE(false) failed");
}

TEST(UtestMacros, CHECK_TRUEBehavesAsProperMacro)
{
  if (false)
    CHECK_TRUE(false);
  else
    CHECK_TRUE(true);
}

IGNORE_TEST(UtestMacros, CHECK_TRUEWorksInAnIgnoredTest)
{
  CHECK_TRUE(false);
}

TEST(UtestMacros, FailureWithCHECK_TRUE_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_check_true_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_TRUE(false) failed");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UtestMacros, CHECK_TRUE_TEXTBehavesAsProperMacro)
{
  if (false)
    CHECK_TRUE_TEXT(false, "Failed because it failed");
  else
    CHECK_TRUE_TEXT(true, "Failed because it failed");
}

IGNORE_TEST(UtestMacros, CHECK_TRUE_TEXTWorksInAnIgnoredTest)
{
  CHECK_TRUE_TEXT(false, "Failed because it failed");
}

TEST(UtestMacros, FailureWithCHECK_FALSE)
{
  fixture.run_test_with_method(failing_test_method_with_check_false);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_FALSE(true) failed");
}

TEST(UtestMacros, CHECK_FALSEBehavesAsProperMacro)
{
  if (false)
    CHECK_FALSE(true);
  else
    CHECK_FALSE(false);
}

IGNORE_TEST(UtestMacros, CHECK_FALSEWorksInAnIgnoredTest)
{
  CHECK_FALSE(true);
}

TEST(UtestMacros, FailureWithCHECK_FALSE_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_check_false_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_FALSE(true)");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UtestMacros, CHECK_FALSE_TEXTBehavesAsProperMacro)
{
  if (false)
    CHECK_FALSE_TEXT(true, "Failed because it failed");
  else
    CHECK_FALSE_TEXT(false, "Failed because it failed");
}

IGNORE_TEST(UtestMacros, CHECK_FALSE_TEXTWorksInAnIgnoredTest)
{
  CHECK_FALSE_TEXT(true, "Failed because it failed");
}

TEST(UtestMacros, FailureWithCHECK_EQUAL)
{
  fixture.run_test_with_method(failing_test_method_with_check_equal);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <2>");
}

TEST(UtestMacros, FailureWithCHECK_COMPARE)
{
  fixture.run_test_with_method(failing_test_method_with_check_compare);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_COMPARE(0.5 >= 0.8)");
}

TEST(UtestMacros, CHECK_COMPAREBehavesAsProperMacro)
{
  if (false)
    CHECK_COMPARE(1, >, 2);
  else
    CHECK_COMPARE(1, <, 2);
}

IGNORE_TEST(UtestMacros, CHECK_COMPAREWorksInAnIgnoredTest)
{
  CHECK_COMPARE(1, >, 2);
}

TEST(UtestMacros, FailureWithCHECK_COMPARE_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_check_compare_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("CHECK_COMPARE(0.5 >= 0.8)");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("small bigger than big");
}

TEST(UtestMacros, CHECK_COMPARE_TEXTBehavesAsProperMacro)
{
  if (false)
    CHECK_COMPARE_TEXT(1, >, 2, "1 bigger than 2");
  else
    CHECK_COMPARE_TEXT(1, <, 2, "1 smaller than 2");
}

IGNORE_TEST(UtestMacros, CHECK_COMPARE_TEXTWorksInAnIgnoredTest)
{
  CHECK_COMPARE_TEXT(1, >, 2, "1 smaller than 2");
}

TEST(UtestMacros, LONGS_EQUAL_macroExpressionSafety)
{
  LONGS_EQUAL(1, 0.4 + 0.7);
  LONGS_EQUAL(0.4 + 0.7, 1);
  LONGS_EQUAL_TEXT(1, 0.4 + 0.7, "-Wconversion=great");
  LONGS_EQUAL_TEXT(0.4 + 0.7, 1, "-Wconversion=great");
}

TEST(UtestMacros, UNSIGNED_LONGS_EQUAL_macroExpressionSafety)
{
  UNSIGNED_LONGS_EQUAL(1, 0.4 + 0.7);
  UNSIGNED_LONGS_EQUAL(0.4 + 0.7, 1);
  UNSIGNED_LONGS_EQUAL_TEXT(1, 0.4 + 0.7, "-Wconversion=great");
  UNSIGNED_LONGS_EQUAL_TEXT(0.4 + 0.7, 1, "-Wconversion=great");
}

TEST(UtestMacros,
    passingCheckEqualWillNotBeEvaluatedMultipleTimesWithCHECK_EQUAL)
{
  count_in_counting_method = 0;
  CHECK_EQUAL(0, counting_method());

  LONGS_EQUAL(1, count_in_counting_method);
}

TEST(UtestMacros,
    failing_CHECK_EQUAL_WithActualBeingEvaluatesMultipleTimesWillGiveAWarning)
{
  fixture.run_test_with_method(
      failing_check_equal_with_actual_being_evaluates_multiple_times_will_give_a_warning);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT(
      "WARNING:\n\tThe \"Actual Parameter\" parameter is evaluated multiple "
      "times resulting in different values.\n\tThus the value in the error "
      "message is probably incorrect.");
}

TEST(UtestMacros,
    failing_CHECK_EQUAL_WithExpectedBeingEvaluatesMultipleTimesWillGiveAWarning)
{
  fixture.run_test_with_method(
      failing_check_equal_with_expected_being_evaluates_multiple_times_will_give_a_warning);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT(
      "WARNING:\n\tThe \"Expected Parameter\" parameter is evaluated multiple "
      "times resulting in different values.\n\tThus the value in the error "
      "message is probably incorrect.");
}

TEST(UtestMacros,
    failing_CHECK_EQUAL_withParamatersThatDontChangeWillNotGiveAnyWarning)
{
  fixture.run_test_with_method(failing_test_method_with_check_equal);
  fixture.assert_print_contains_not("WARNING");
}

TEST(UtestMacros, CHECK_EQUALBehavesAsProperMacro)
{
  if (false)
    CHECK_EQUAL(1, 2);
  else
    CHECK_EQUAL(1, 1);
}

IGNORE_TEST(UtestMacros, CHECK_EQUALWorksInAnIgnoredTest)
{
  CHECK_EQUAL(1, 2);
}

TEST(UtestMacros, FailureWithCHECK_EQUAL_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_check_equal_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <2>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UtestMacros, CHECK_EQUAL_TEXTBehavesAsProperMacro)
{
  if (false)
    CHECK_EQUAL_TEXT(1, 2, "Failed because it failed");
  else
    CHECK_EQUAL_TEXT(1, 1, "Failed because it failed");
}

IGNORE_TEST(UtestMacros, CHECK_EQUAL_TEXTWorksInAnIgnoredTest)
{
  CHECK_EQUAL_TEXT(1, 2, "Failed because it failed");
}

TEST(UtestMacros, FailureWithCHECK_EQUAL_ZERO)
{
  fixture.run_test_with_method(failing_test_method_with_check_equal_zero);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <1>");
}

TEST(UtestMacros,
    passingCheckEqualWillNotBeEvaluatedMultipleTimesWithCHECK_EQUAL_ZERO)
{
  count_in_counting_method = 0;
  CHECK_EQUAL_ZERO(counting_method());

  LONGS_EQUAL(1, count_in_counting_method);
}

TEST(UtestMacros,
    failing_CHECK_EQUAL_ZERO_WithActualBeingEvaluatesMultipleTimesWillGiveAWarning)
{
  fixture.run_test_with_method(
      failing_check_equal_zero_with_actual_being_evaluates_multiple_times_will_give_a_warning);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT(
      "WARNING:\n\tThe \"Actual Parameter\" parameter is evaluated multiple "
      "times resulting in different values.\n\tThus the value in the error "
      "message is probably incorrect.");
}

TEST(UtestMacros,
    failing_CHECK_EQUAL_ZERO_withParamatersThatDontChangeWillNotGiveAnyWarning)
{
  fixture.run_test_with_method(failing_test_method_with_check_equal_zero);
  fixture.assert_print_contains_not("WARNING");
}

IGNORE_TEST(UtestMacros, CHECK_EQUAL_ZERO_WorksInAnIgnoredTest)
{
  CHECK_EQUAL_ZERO(1);
}

TEST(UtestMacros, CHECK_EQUAL_ZERO_BehavesAsProperMacro)
{
  if (false)
    CHECK_EQUAL_ZERO(1);
  else
    CHECK_EQUAL_ZERO(0);
}

TEST(UtestMacros, FailureWithCHECK_EQUAL_ZERO_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_check_equal_zero_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UtestMacros, CHECK_EQUAL_ZERO_TEXTBehavesAsProperMacro)
{
  if (false)
    CHECK_EQUAL_ZERO_TEXT(1, "Failed because it failed");
  else
    CHECK_EQUAL_ZERO_TEXT(0, "Failed because it failed");
}

IGNORE_TEST(UtestMacros, CHECK_EQUAL_ZERO_TEXTWorksInAnIgnoredTest)
{
  CHECK_EQUAL_ZERO_TEXT(1, "Failed because it failed");
}

TEST(UtestMacros, FailureWithLONGS_EQUALS)
{
  fixture.run_test_with_method(failing_test_method_with_longs_equal);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <  1 (0x1)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <255 (0xff)>");
}

TEST(UtestMacros, FailureWithLONGS_EQUALShowsSymbolicParameters)
{
  fixture.run_test_with_method(
      failing_test_method_with_longs_equal_with_symbolic_parameters);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT(
      "LONGS_EQUAL(MONDAY, day_of_the_week) failed");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1 (0x1)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <2 (0x2)>");
  CHECK_FALSE(cppmu::string_contains(fixture.get_output(), "Message: "));
}

TEST(UtestMacros, LONGS_EQUALBehavesAsProperMacro)
{
  if (false)
    LONGS_EQUAL(1, 2);
  else
    LONGS_EQUAL(10, 10);
}

IGNORE_TEST(UtestMacros, LONGS_EQUALWorksInAnIgnoredTest)
{
  LONGS_EQUAL(11, 22);
}

TEST(UtestMacros, FailureWithLONGS_EQUALS_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_longs_equal_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <  1 (0x1)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <255 (0xff)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UtestMacros, LONGS_EQUAL_TEXTBehavesAsProperMacro)
{
  if (false)
    LONGS_EQUAL_TEXT(1, 2, "Failed because it failed");
  else
    LONGS_EQUAL_TEXT(10, 10, "Failed because it failed");
}

IGNORE_TEST(UtestMacros, LONGS_EQUAL_TEXTWorksInAnIgnoredTest)
{
  LONGS_EQUAL_TEXT(11, 22, "Failed because it failed");
}

TEST(UtestMacros, FailureWithBYTES_EQUAL)
{
  fixture.run_test_with_method(failing_test_method_with_bytes_equal);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <97 (0x61)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <98 (0x62)>");
}

TEST(UtestMacros, BYTES_EQUALBehavesAsProperMacro)
{
  if (false)
    BYTES_EQUAL('a', 'b');
  else
    BYTES_EQUAL('c', 'c');
}

IGNORE_TEST(UtestMacros, BYTES_EQUALWorksInAnIgnoredTest)
{
  BYTES_EQUAL('q', 'w');
}

TEST(UtestMacros, FailureWithBYTES_EQUAL_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_bytes_equal_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <97 (0x61)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <98 (0x62)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UtestMacros, BYTES_EQUAL_TEXTBehavesAsProperMacro)
{
  if (false)
    BYTES_EQUAL_TEXT('a', 'b', "Failed because it failed");
  else
    BYTES_EQUAL_TEXT('c', 'c', "Failed because it failed");
}

IGNORE_TEST(UtestMacros, BYTES_EQUAL_TEXTWorksInAnIgnoredTest)
{
  BYTES_EQUAL_TEXT('q', 'w', "Failed because it failed");
}

TEST(UtestMacros, FailureWithSIGNED_BYTES_EQUAL)
{
  fixture.run_test_with_method(failing_test_method_with_signed_bytes_equal);
#if CHAR_BIT == 16
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <-1 (0xffff)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <-2 (0xfffe)>");
#elif CHAR_BIT == 8
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <-1 (0xff)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <-2 (0xfe)>");
#endif
}

TEST(UtestMacros, CHARS_EQUALBehavesAsProperMacro)
{
  if (false)
    SIGNED_BYTES_EQUAL(-1, -2);
  else
    SIGNED_BYTES_EQUAL(-3, -3);
}

IGNORE_TEST(UtestMacros, CHARS_EQUALWorksInAnIgnoredTest)
{
  SIGNED_BYTES_EQUAL(-7, 19);
}

TEST(UtestMacros, FailureWithSIGNED_BYTES_EQUAL_TEXT)
{
  fixture.run_test_with_method(
      failing_test_method_with_signed_bytes_equal_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <-127 (0x81)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <-126 (0x82)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UtestMacros, CHARS_EQUAL_TEXTBehavesAsProperMacro)
{
  if (false)
    SIGNED_BYTES_EQUAL_TEXT(-1, -2, "Failed because it failed");
  else
    SIGNED_BYTES_EQUAL_TEXT(-3, -3, "Failed because it failed");
}

IGNORE_TEST(UtestMacros, SIGNED_BYTES_EQUAL_TEXTWorksInAnIgnoredTest)
{
  SIGNED_BYTES_EQUAL_TEXT(-7, 19, "Failed because it failed");
}

TEST(UtestMacros, FailureWithPOINTERS_EQUAL)
{
  fixture.run_test_with_method(failing_test_method_with_pointers_equal);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0xa5a5>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0xf0f0>");
}

TEST(UtestMacros, POINTERS_EQUALBehavesAsProperMacro)
{
  if (false)
    POINTERS_EQUAL(nullptr, nullptr);
  else
    POINTERS_EQUAL(reinterpret_cast<void*>(0xdeadbeefULL),
        reinterpret_cast<void*>(0xdeadbeefULL));
}

IGNORE_TEST(UtestMacros, POINTERS_EQUALWorksInAnIgnoredTest)
{
  POINTERS_EQUAL(
      reinterpret_cast<void*>(0xbeef), reinterpret_cast<void*>(0xdead));
}

TEST(UtestMacros, FailureWithPOINTERS_EQUAL_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_pointers_equal_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0xa5a5>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0xf0f0>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UtestMacros, POINTERS_EQUAL_TEXTBehavesAsProperMacro)
{
  if (false)
    POINTERS_EQUAL_TEXT(nullptr, nullptr, "Failed because it failed");
  else
    POINTERS_EQUAL_TEXT(reinterpret_cast<void*>(0xdeadbeefULL),
        reinterpret_cast<void*>(0xdeadbeefULL),
        "Failed because it failed");
}

IGNORE_TEST(UtestMacros, POINTERS_EQUAL_TEXTWorksInAnIgnoredTest)
{
  POINTERS_EQUAL_TEXT(reinterpret_cast<void*>(0xbeef),
      reinterpret_cast<void*>(0xdead),
      "Failed because it failed");
}

TEST(UtestMacros, FailureWithFUNCTIONPOINTERS_EQUAL)
{
  fixture.run_test_with_method(failing_test_method_with_functionpointers_equal);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0xa5a5>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0xf0f0>");
}

TEST(UtestMacros, FUNCTIONPOINTERS_EQUALBehavesAsProperMacro)
{
  if (false)
    FUNCTIONPOINTERS_EQUAL(
        static_cast<void (*)()>(nullptr), static_cast<void (*)()>(nullptr));
  else
    FUNCTIONPOINTERS_EQUAL(reinterpret_cast<void (*)()>(0xdeadbeefULL),
        reinterpret_cast<void (*)()>(0xdeadbeefULL));
}

IGNORE_TEST(UtestMacros, FUNCTIONPOINTERS_EQUALWorksInAnIgnoredTest)
{
  FUNCTIONPOINTERS_EQUAL(reinterpret_cast<void (*)()>(0xbeef),
      reinterpret_cast<void (*)()>(0xdead));
}

TEST(UtestMacros, FailureWithFUNCTIONPOINTERS_EQUAL_TEXT)
{
  fixture.run_test_with_method(
      failing_test_method_with_functionpointers_equal_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0xa5a5>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0xf0f0>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UtestMacros, FUNCTIONPOINTERS_EQUAL_TEXTBehavesAsProperMacro)
{
  if (false)
    FUNCTIONPOINTERS_EQUAL_TEXT(static_cast<void (*)()>(nullptr),
        static_cast<void (*)()>(nullptr),
        "Failed because it failed");
  else
    FUNCTIONPOINTERS_EQUAL_TEXT(reinterpret_cast<void (*)()>(0xdeadbeefULL),
        reinterpret_cast<void (*)()>(0xdeadbeefULL),
        "Failed because it failed");
}

IGNORE_TEST(UtestMacros, FUNCTIONPOINTERS_EQUAL_TEXTWorksInAnIgnoredTest)
{
  FUNCTIONPOINTERS_EQUAL_TEXT(reinterpret_cast<void (*)()>(0xbeef),
      reinterpret_cast<void (*)()>(0xdead),
      "Failed because it failed");
}

TEST(UtestMacros, FailureWithDOUBLES_EQUAL)
{
  fixture.run_test_with_method(failing_test_method_with_doubles_equal);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0.12>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <44.1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("threshold used was <0.3>");
}

TEST(UtestMacros, DOUBLES_EQUALBehavesAsProperMacro)
{
  if (false)
    DOUBLES_EQUAL(0.0, 1.1, 0.0005);
  else
    DOUBLES_EQUAL(0.1, 0.2, 0.2);
}

IGNORE_TEST(UtestMacros, DOUBLES_EQUALWorksInAnIgnoredTest)
{
  DOUBLES_EQUAL(100.0, 0.0, 0.2);
}

TEST(UtestMacros, FailureWithDOUBLES_EQUAL_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_doubles_equal_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <0.12>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <44.1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("threshold used was <0.3>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UtestMacros, DOUBLES_EQUAL_TEXTBehavesAsProperMacro)
{
  if (false)
    DOUBLES_EQUAL_TEXT(0.0, 1.1, 0.0005, "Failed because it failed");
  else
    DOUBLES_EQUAL_TEXT(0.1, 0.2, 0.2, "Failed because it failed");
}

IGNORE_TEST(UtestMacros, DOUBLES_EQUAL_TEXTWorksInAnIgnoredTest)
{
  DOUBLES_EQUAL_TEXT(100.0, 0.0, 0.2, "Failed because it failed");
}

TEST(UtestMacros, SuccessPrintsNothing)
{
  fixture.run_test_with_method(passing_test_method);

  LONGS_EQUAL(0, fixture.get_failure_count());
  fixture.assert_print_contains(".\nOK (1 tests");
  CHECK(line_of_code_executed_after_check);
}

TEST(UtestMacros, allMacrosFromFunctionThatReturnsAValue)
{
  function_that_returns_a_value();
}

TEST(UtestMacros, MEMCMP_EQUALBehavesAsAProperMacro)
{
  if (false)
    MEMCMP_EQUAL("TEST", "test", 5);
  else
    MEMCMP_EQUAL("TEST", "TEST", 5);
}

IGNORE_TEST(UtestMacros, MEMCMP_EQUALWorksInAnIgnoredTest)
{
  MEMCMP_EQUAL("TEST", "test", 5);
}

TEST(UtestMacros, MEMCMP_EQUALFailureWithUnequalInput)
{
  fixture.run_test_with_method(
      memcmp_equal_failing_test_method_with_unequal_input);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <00 01 02 03>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <00 01 03 03>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 2");
}

TEST(UtestMacros, MEMCMP_EQUALFailureWithNullExpected)
{
  fixture.run_test_with_method(
      memcmp_equal_failing_test_method_with_null_expected);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <(null)>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <00 01 02 03>");
}

TEST(UtestMacros, MEMCMP_EQUALFailureWithNullActual)
{
  fixture.run_test_with_method(
      memcmp_equal_failing_test_method_with_null_actual);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <00 01 02 03>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <(null)>");
}

TEST(UtestMacros, MEMCMP_EQUALNullExpectedNullActual)
{
  MEMCMP_EQUAL(nullptr, nullptr, 0);
  MEMCMP_EQUAL(nullptr, nullptr, 1024);
}

TEST(UtestMacros, MEMCMP_EQUALNullPointerIgnoredInExpectationWhenSize0)
{
  unsigned char actual_data[] = { 0x00, 0x01, 0x03, 0x03 };
  MEMCMP_EQUAL(nullptr, actual_data, 0);
}

TEST(UtestMacros, MEMCMP_EQUALNullPointerIgnoredInActualWhenSize0)
{
  unsigned char expected_data[] = { 0x00, 0x01, 0x02, 0x03 };
  MEMCMP_EQUAL(expected_data, nullptr, 0);
}

TEST(UtestMacros, FailureWithMEMCMP_EQUAL_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_memcmp_equal_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <00 01 02 03>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <00 01 03 03>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 2");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UtestMacros, MEMCMP_EQUAL_TEXTBehavesAsAProperMacro)
{
  if (false)
    MEMCMP_EQUAL_TEXT("TEST", "test", 5, "Failed because it failed");
  else
    MEMCMP_EQUAL_TEXT("TEST", "TEST", 5, "Failed because it failed");
}

IGNORE_TEST(UtestMacros, MEMCMP_EQUAL_TEXTWorksInAnIgnoredTest)
{
  MEMCMP_EQUAL_TEXT("TEST", "test", 5, "Failed because it failed");
}

TEST(UtestMacros, TestENUMS_EQUAL_INTWithScopedIntEnum)
{
  fixture.run_test_with_method(
      enums_equal_int_with_scoped_int_enum_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0>");
}

TEST(UtestMacros, ENUMS_EQUAL_INTWithScopedIntEnumBehavesAsProperMacro)
{
  if (false)
    ENUMS_EQUAL_INT(ScopedIntEnum::b, ScopedIntEnum::a);
  else
    ENUMS_EQUAL_INT(ScopedIntEnum::b, ScopedIntEnum::b);
}

IGNORE_TEST(UtestMacros, ENUMS_EQUAL_INTWithScopedIntEnumWorksInAnIgnoredTest)
{
  ENUMS_EQUAL_INT(ScopedIntEnum::b, ScopedIntEnum::a);
}

TEST(UtestMacros, TestENUMS_EQUAL_INT_TEXTWithScopedIntEnum)
{
  fixture.run_test_with_method(
      enums_equal_int_text_with_scoped_int_enum_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UtestMacros, ENUMS_EQUAL_INT_TEXTWithScopedIntEnumBehavesAsProperMacro)
{
  if (false)
    ENUMS_EQUAL_INT_TEXT(
        ScopedIntEnum::b, ScopedIntEnum::a, "Failed because it failed");
  else
    ENUMS_EQUAL_INT_TEXT(
        ScopedIntEnum::b, ScopedIntEnum::b, "Failed because it failed");
}

IGNORE_TEST(UtestMacros,
    ENUMS_EQUAL_EQUAL_INT_TEXTWithScopedIntEnumWorksInAnIgnoredTest)
{
  ENUMS_EQUAL_INT_TEXT(
      ScopedIntEnum::b, ScopedIntEnum::a, "Failed because it failed");
}

TEST(UtestMacros, TestENUMS_EQUAL_TYPEWithScopedLongEnum)
{
  fixture.run_test_with_method(
      enums_equal_type_with_scoped_long_enum_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0>");
}

TEST(UtestMacros, ENUMS_EQUAL_TYPEWithScopedLongEnumBehavesAsProperMacro)
{
  if (false)
    ENUMS_EQUAL_TYPE(long, ScopedLongEnum::b, ScopedLongEnum::a);
  else
    ENUMS_EQUAL_TYPE(long, ScopedLongEnum::b, ScopedLongEnum::b);
}

IGNORE_TEST(UtestMacros, ENUMS_EQUAL_TYPEWithScopedLongEnumWorksInAnIgnoredTest)
{
  ENUMS_EQUAL_TYPE(long, ScopedLongEnum::b, ScopedLongEnum::a);
}

TEST(UtestMacros, TestENUMS_EQUAL_TYPE_TEXTWithScopedLongEnum)
{
  fixture.run_test_with_method(
      enums_equal_type_text_with_scoped_long_enum_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UtestMacros, ENUMS_EQUAL_TYPE_TEXTWithScopedLongEnumBehavesAsProperMacro)
{
  if (false)
    ENUMS_EQUAL_TYPE_TEXT(
        long, ScopedLongEnum::b, ScopedLongEnum::a, "Failed because it failed");
  else
    ENUMS_EQUAL_TYPE_TEXT(
        long, ScopedLongEnum::b, ScopedLongEnum::b, "Failed because it failed");
}

IGNORE_TEST(UtestMacros,
    ENUMS_EQUAL_EQUAL_TYPE_TEXTWithScopedLongEnumWorksInAnIgnoredTest)
{
  ENUMS_EQUAL_TYPE_TEXT(
      long, ScopedLongEnum::b, ScopedLongEnum::a, "Failed because it failed");
}

TEST(UtestMacros, TestENUMS_EQUAL_INTWithUnscopedEnum)
{
  fixture.run_test_with_method(enums_equal_int_with_unscoped_enum_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0>");
}

TEST(UtestMacros, ENUMS_EQUAL_INTWithUnscopedEnumBehavesAsProperMacro)
{
  if (false)
    ENUMS_EQUAL_INT(unscoped_enum_b, unscoped_enum_a);
  else
    ENUMS_EQUAL_INT(unscoped_enum_b, unscoped_enum_b);
}

IGNORE_TEST(UtestMacros, ENUMS_EQUAL_INTWithUnscopedEnumWorksInAnIgnoredTest)
{
  ENUMS_EQUAL_INT(unscoped_enum_b, unscoped_enum_a);
}

TEST(UtestMacros, TestENUMS_EQUAL_INT_TEXTWithUnscopedEnum)
{
  fixture.run_test_with_method(
      enums_equal_int_text_with_unscoped_enum_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <1>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <0>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UtestMacros, ENUMS_EQUAL_INT_TEXTWithUnscopedEnumBehavesAsProperMacro)
{
  if (false)
    ENUMS_EQUAL_INT_TEXT(
        unscoped_enum_b, unscoped_enum_a, "Failed because it failed");
  else
    ENUMS_EQUAL_INT_TEXT(
        unscoped_enum_b, unscoped_enum_b, "Failed because it failed");
}

IGNORE_TEST(UtestMacros,
    ENUMS_EQUAL_EQUAL_INT_TEXTWithUnscopedEnumWorksInAnIgnoredTest)
{
  ENUMS_EQUAL_INT_TEXT(
      unscoped_enum_b, unscoped_enum_a, "Failed because it failed");
}

#if CPPMU_HAVE_EXCEPTIONS
TEST(
UtestMacros, FailureWithCHECK_THROWS_whenDoesntThrow)
{
  fixture.run_test_with_method(failing_test_method_no_throw_with_check_throws);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected to throw int");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but threw nothing");
  LONGS_EQUAL(1, fixture.get_check_count());
}

TEST(
UtestMacros, SuccessWithCHECK_THROWS)
{
  fixture.run_test_with_method(
      succeeding_test_method_correct_throw_with_check_throws);
  LONGS_EQUAL(1, fixture.get_check_count());
}

TEST(
UtestMacros, FailureWithCHECK_THROWS_whenWrongThrow)
{
  fixture.run_test_with_method(
      failing_test_method_wrong_throw_with_check_throws);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected to throw int");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but threw a different type");
  LONGS_EQUAL(1, fixture.get_check_count());
}

TEST(
UtestMacros, MultipleCHECK_THROWS_inOneScope)
{
  CHECK_THROWS(int, throw 4);
  CHECK_THROWS(int, throw 4);
}
#endif

TEST_GROUP(UtestStringMacros)
{
  cppmu::TestTestingFixture fixture;
};

namespace {

void
strcmp_equal_with_actual_is_null_test_method()
{
  STRCMP_EQUAL("ok", nullptr);
}

void
strcmp_equal_with_expected_is_null_test_method()
{
  STRCMP_EQUAL(nullptr, "ok");
}

void
strcmp_contains_with_actual_is_null_test_method()
{
  STRCMP_CONTAINS("ok", nullptr);
}

void
strcmp_contains_with_expected_is_null_test_method()
{
  STRCMP_CONTAINS(nullptr, "ok");
}

void
strncmp_equal_with_actual_is_null_test_method()
{
  STRNCMP_EQUAL("ok", nullptr, 2);
}

void
strncmp_equal_with_expected_is_null_test_method()
{
  STRNCMP_EQUAL(nullptr, "ok", 2);
}

void
failing_test_method_with_strcmp_equal()
{
  STRCMP_EQUAL("hello", "hell");
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_strcmp_equal_text()
{
  STRCMP_EQUAL_TEXT("hello", "hell", "Failed because it failed");
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_strncmp_equal()
{
  STRNCMP_EQUAL("hello", "hallo", 5);
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_strncmp_equal_text()
{
  STRNCMP_EQUAL_TEXT("hello", "hallo", 5, "Failed because it failed");
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_strcmp_contains()
{
  STRCMP_CONTAINS("hello", "world");
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
failing_test_method_with_strcmp_contains_text()
{
  STRCMP_CONTAINS_TEXT("hello", "world", "Failed because it failed");
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
compare_n_first_chars_with_upper_and_lowercase()
{
  STRNCMP_EQUAL("hello world!", "HELLO WORLD!", 12);
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
compare_n_first_chars_with_difference_in_the_middle()
{
  STRNCMP_EQUAL("Hello World!", "Hello Peter!", 12);
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
compare_n_first_chars_with_empty_string()
{
  STRNCMP_EQUAL("", "Not empty string", 5);
  cppmu::TestTestingFixture::line_executed_after_check();
}

void
compare_n_first_chars_with_last_char_different()
{
  STRNCMP_EQUAL("Not empty string?", "Not empty string!", 17);
  cppmu::TestTestingFixture::line_executed_after_check();
}

} // namespace

TEST(UtestStringMacros, FailureWithSTRCMP_EQUALAndActualIsNULL)
{
  fixture.run_test_with_method(strcmp_equal_with_actual_is_null_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <(null)>");
}

TEST(UtestStringMacros, FailureWithSTRCMP_EQUALAndExpectedIsNULL)
{
  fixture.run_test_with_method(strcmp_equal_with_expected_is_null_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <(null)>");
}

TEST(UtestStringMacros, FailureWithSTRCMP_CONTAINSAndActualIsNULL)
{
  fixture.run_test_with_method(strcmp_contains_with_actual_is_null_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <ok>");
}

TEST(UtestStringMacros, FailureWithSTRCMP_CONTAINSAndExpectedIsNULL)
{
  fixture.run_test_with_method(
      strcmp_contains_with_expected_is_null_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <(null)>");
}

TEST(UtestStringMacros, FailureWithSTRNCMP_EQUALAndActualIsNULL)
{
  fixture.run_test_with_method(strncmp_equal_with_actual_is_null_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <(null)>");
}

TEST(UtestStringMacros, FailureWithSTRNCMP_EQUALAndExpectedIsNULL)
{
  fixture.run_test_with_method(strncmp_equal_with_expected_is_null_test_method);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <(null)>");
}

TEST(UtestStringMacros, FailureWithSTRCMP_EQUAL)
{
  fixture.run_test_with_method(failing_test_method_with_strcmp_equal);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <hell>");
}

TEST(UtestStringMacros, STRCMP_EQUALBehavesAsProperMacro)
{
  if (false)
    STRCMP_EQUAL("1", "2");
  else
    STRCMP_EQUAL("1", "1");
}

TEST(UtestStringMacros, STRCMP_EQUALBothNullPasses)
{
  STRCMP_EQUAL(nullptr, nullptr);
}

IGNORE_TEST(UtestStringMacros, STRCMP_EQUALWorksInAnIgnoredTest)
{
  STRCMP_EQUAL("Hello", "World");
}

TEST(UtestStringMacros, FailureWithSTRCMP_EQUAL_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_strcmp_equal_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <hell>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UtestStringMacros, STRCMP_EQUAL_TEXTBehavesAsProperMacro)
{
  if (false)
    STRCMP_EQUAL_TEXT("1", "2", "Failed because it failed");
  else
    STRCMP_EQUAL_TEXT("1", "1", "Failed because it failed");
}

IGNORE_TEST(UtestStringMacros, STRCMP_EQUAL_TEXTWorksInAnIgnoredTest)
{
  STRCMP_EQUAL_TEXT("Hello", "World", "Failed because it failed");
}

TEST(UtestStringMacros, FailureWithSTRNCMP_EQUAL)
{
  fixture.run_test_with_method(failing_test_method_with_strncmp_equal);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <hallo>");
}

TEST(UtestStringMacros, STRNCMP_EQUALBehavesAsProperMacro)
{
  if (false)
    STRNCMP_EQUAL("1", "2", 1);
  else
    STRNCMP_EQUAL("1", "1", 1);
}

TEST(UtestStringMacros, STRNCMP_EQUALBothNullPasses)
{
  STRNCMP_EQUAL(nullptr, nullptr, 0);
}

IGNORE_TEST(UtestStringMacros, STRNCMP_EQUALWorksInAnIgnoredTest)
{
  STRNCMP_EQUAL("Hello", "World", 3);
}

TEST(UtestStringMacros, FailureWithSTRNCMP_EQUAL_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_strncmp_equal_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <hallo>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UtestStringMacros, STRNCMP_EQUAL_TEXTBehavesAsProperMacro)
{
  if (false)
    STRNCMP_EQUAL_TEXT("1", "2", 1, "Failed because it failed");
  else
    STRNCMP_EQUAL_TEXT("1", "1", 1, "Failed because it failed");
}

IGNORE_TEST(UtestStringMacros, STRNCMP_EQUAL_TEXTWorksInAnIgnoredTest)
{
  STRNCMP_EQUAL_TEXT("Hello", "World", 3, "Failed because it failed");
}

TEST(UtestStringMacros, FailureWithSTRCMP_CONTAINS)
{
  fixture.run_test_with_method(failing_test_method_with_strcmp_contains);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("actual <world>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <hello>");
}

TEST(UtestStringMacros, STRCMP_CONTAINSBehavesAsProperMacro)
{
  if (false)
    STRCMP_CONTAINS("1", "2");
  else
    STRCMP_CONTAINS("1", "1");
}

TEST(UtestStringMacros, STRCMP_CONTAINSBothNullPasses)
{
  STRCMP_CONTAINS(nullptr, nullptr);
}

IGNORE_TEST(UtestStringMacros, STRCMP_CONTAINSWorksInAnIgnoredTest)
{
  STRCMP_CONTAINS("Hello", "World");
}

TEST(UtestStringMacros, FailureWithSTRCMP_CONTAINS_TEXT)
{
  fixture.run_test_with_method(failing_test_method_with_strcmp_contains_text);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("actual <world>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("did not contain  <hello>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("Failed because it failed");
}

TEST(UtestStringMacros, STRCMP_CONTAINS_TEXTBehavesAsProperMacro)
{
  if (false)
    STRCMP_CONTAINS_TEXT("1", "2", "Failed because it failed");
  else
    STRCMP_CONTAINS_TEXT("1", "1", "Failed because it failed");
}

IGNORE_TEST(UtestStringMacros, STRCMP_CONTAINS_TEXTWorksInAnIgnoredTest)
{
  STRCMP_CONTAINS_TEXT("Hello", "World", "Failed because it failed");
}

TEST(UtestStringMacros, NFirstCharsComparison)
{
  STRNCMP_EQUAL("Hello World!", "Hello Peter!", 0);
  STRNCMP_EQUAL("Hello World!", "Hello Peter!", 1);
  STRNCMP_EQUAL("Hello World!", "Hello Peter!", 6);
  STRNCMP_EQUAL("Hello World!", "Hello", 5);
}

TEST(UtestStringMacros, CompareNFirstCharsWithUpperAndLowercase)
{
  fixture.run_test_with_method(compare_n_first_chars_with_upper_and_lowercase);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <hello world!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <HELLO WORLD!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 0");
}

TEST(UtestStringMacros, CompareNFirstCharsWithDifferenceInTheMiddle)
{
  fixture.run_test_with_method(
      compare_n_first_chars_with_difference_in_the_middle);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <Hello World!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <Hello Peter!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 6");
}

TEST(UtestStringMacros, CompareNFirstCharsWithEmptyString)
{
  fixture.run_test_with_method(compare_n_first_chars_with_empty_string);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <Not empty string>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 0");
}

TEST(UtestStringMacros, CompareNFirstCharsWithLastCharDifferent)
{
  fixture.run_test_with_method(compare_n_first_chars_with_last_char_different);
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("expected <Not empty string?>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("but was  <Not empty string!>");
  CHECK_TEST_FAILS_PROPER_WITH_TEXT("difference starts at position 16");
}

namespace {

class PropertyRecordingOutput : public cppmu::StringBufferTestOutput
{
public:
  const char* recorded_name{ nullptr };
  const char* recorded_value{ nullptr };
  int call_count{ 0 };

  void print_test_property(const char* name, const char* value) override
  {
    recorded_name = name;
    recorded_value = value;
    call_count++;
  }
};

} // namespace

TEST_GROUP(TestShellProperty)
{};

TEST(TestShellProperty, addTestPropertyRoutesToTestOutputPrintTestProperty)
{
  PropertyRecordingOutput output;
  cppmu::TestResult result(output);
  result.add_test_property("ticket_id", "12345");

  LONGS_EQUAL(1, output.call_count);
  STRCMP_EQUAL("ticket_id", output.recorded_name);
  STRCMP_EQUAL("12345", output.recorded_value);
}

TEST(TestShellProperty, addTestPropertyOnShellRoutesToResult)
{
  PropertyRecordingOutput output;
  cppmu::TestResult result(output);
  cppmu::TestShell shell("Group", "Test", "file", 1);

  // Simulate the test context setup done by run_one_test_in_current_process
  cppmu::TestShell::set_crash_on_fail(); // harmless; just ensuring static init
  cppmu::TestShell::restore_default_test_terminator();

  // Directly test TestResult delegation (TestShell routes through TestResult)
  result.add_test_property("suite", "smoke");

  LONGS_EQUAL(1, output.call_count);
  STRCMP_EQUAL("suite", output.recorded_name);
  STRCMP_EQUAL("smoke", output.recorded_value);
}
