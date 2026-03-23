#include "mutiny/test.hpp"
#include "mutiny/test/ExecFunctionShell.hpp"
#include "mutiny/test/NullPlugin.hpp"
#include "mutiny/test/Output.hpp"
#include "mutiny/test/Shell.hpp"
#include "mutiny/test/StringBufferOutput.hpp"
#include "mutiny/test/TestingFixture.hpp"

#include <limits.h>
#include <math.h>

#define CHECK_TEST_FAILS_PROPER_WITH_TEXT(text)                                \
  fixture.check_test_fails_with_proper_test_location(text, __FILE__, __LINE__)

TEST_GROUP(TestShell)
{
  mu::tiny::test::TestTestingFixture fixture;
};

namespace {
void fail_method()
{
  FAIL("This test fails");
}

void simple_passing_method()
{
  CHECK(true);
}

void passing_check_equal_test_method()
{
  CHECK_EQUAL(1, 1);
}

void exit_test_method()
{
  TEST_EXIT;
  FAIL("Should not get here");
}

class PassingExecFunction : public mu::tiny::test::ExecFunction
{
public:
  bool has_run{ false };
  void exec() override { has_run = true; }
};

bool mutiny_has_crashed;

void crash_method()
{
  mutiny_has_crashed = true;
}

int teardown_called = 0;

void teardown_method()
{
  teardown_called++;
}

int stop_after_failure = 0;
void stop_after_failure_method()
{
  FAIL("fail");
  stop_after_failure++;
}

#if MUTINY_HAVE_EXCEPTIONS
// Prevents -Wunreachable-code; should always be 'true'
bool should_throw_exception = true;

void thrown_unknown_exception_method()
{
  if (should_throw_exception) {
    throw 33;
  }
  stop_after_failure++;
}

#if MUTINY_USE_STD_CPP_LIB
void thrown_standard_exception_method()
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
  CHECK(mu::tiny::test::doubles_equal(1.0, 1.001, 0.01));
  CHECK(!mu::tiny::test::doubles_equal(1.0, 1.1, 0.05));
  double a = 1.2345678;
  CHECK(mu::tiny::test::doubles_equal(a, a, 0.000000001));
}

#ifdef NAN
TEST(TestShell, compareDoublesNaN)
{
  CHECK(!mu::tiny::test::doubles_equal(static_cast<double>(NAN), 1.001, 0.01));
  CHECK(!mu::tiny::test::doubles_equal(1.0, static_cast<double>(NAN), 0.01));
  CHECK(!mu::tiny::test::doubles_equal(1.0, 1.001, static_cast<double>(NAN)));
}
#endif

#ifdef INFINITY
TEST(TestShell, compareDoublesInf)
{
  CHECK(
      !mu::tiny::test::doubles_equal(static_cast<double>(INFINITY), 1.0, 0.01)
  );
  CHECK(
      !mu::tiny::test::doubles_equal(1.0, static_cast<double>(INFINITY), 0.01)
  );
  CHECK(
      mu::tiny::test::doubles_equal(1.0, -1.0, static_cast<double>(INFINITY))
  );
  CHECK(
      mu::tiny::test::doubles_equal(
          static_cast<double>(INFINITY), static_cast<double>(INFINITY), 0.01
      )
  );
  CHECK(
      mu::tiny::test::doubles_equal(
          static_cast<double>(INFINITY),
          static_cast<double>(INFINITY),
          static_cast<double>(INFINITY)
      )
  );
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
  mutiny_has_crashed = false;
  mu::tiny::test::TestShell::set_crash_method(crash_method);

  fixture.set_test_function(fail_method);
  fixture.run_all_tests();

  CHECK_FALSE(mutiny_has_crashed);
  LONGS_EQUAL(1, fixture.get_failure_count());

  mu::tiny::test::TestShell::reset_crash_method();
}

TEST(TestShell, FailWillCrashIfEnabled)
{
  mutiny_has_crashed = false;
  mu::tiny::test::TestShell::set_crash_on_fail();
  mu::tiny::test::TestShell::set_crash_method(crash_method);

  fixture.set_test_function(fail_method);
  fixture.run_all_tests();

  CHECK(mutiny_has_crashed);

  mu::tiny::test::TestShell::restore_default_test_terminator();
  mu::tiny::test::TestShell::reset_crash_method();
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

#if MUTINY_HAVE_EXCEPTIONS

TEST(TestShell, TestStopsAfterUnknownExceptionIsThrown)
{
  bool initial_rethrow_exceptions =
      mu::tiny::test::TestShell::is_rethrowing_exceptions();
  mu::tiny::test::TestShell::set_rethrow_exceptions(false);
  stop_after_failure = 0;
  should_throw_exception = true;
  fixture.set_test_function(thrown_unknown_exception_method);
  fixture.run_all_tests();
  LONGS_EQUAL(1, fixture.get_failure_count());
  fixture.assert_print_contains(
      "Unexpected exception of unknown type was thrown"
  );
  LONGS_EQUAL(0, stop_after_failure);
  mu::tiny::test::TestShell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

TEST(TestShell, NoExceptionIsRethrownIfEnabledButNotThrown)
{
  bool initial_rethrow_exceptions =
      mu::tiny::test::TestShell::is_rethrowing_exceptions();
  bool exception_rethrown = false;
  stop_after_failure = 0;
  mu::tiny::test::TestShell::set_rethrow_exceptions(true);
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
  mu::tiny::test::TestShell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

TEST(TestShell, UnknownExceptionIsRethrownIfEnabled)
{
  bool initial_rethrow_exceptions =
      mu::tiny::test::TestShell::is_rethrowing_exceptions();
  bool exception_rethrown = false;
  stop_after_failure = 0;
  mu::tiny::test::TestShell::set_rethrow_exceptions(true);
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
      "Unexpected exception of unknown type was thrown"
  );
  LONGS_EQUAL(0, stop_after_failure);
  mu::tiny::test::TestShell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

#if MUTINY_USE_STD_CPP_LIB

TEST(TestShell, TestStopsAfterStandardExceptionIsThrown)
{
  bool initial_rethrow_exceptions =
      mu::tiny::test::TestShell::is_rethrowing_exceptions();
  mu::tiny::test::TestShell::set_rethrow_exceptions(false);
  stop_after_failure = 0;
  should_throw_exception = true;
  fixture.set_test_function(thrown_standard_exception_method);
  fixture.run_all_tests();
  LONGS_EQUAL(1, fixture.get_failure_count());
#if MUTINY_HAVE_RTTI
  fixture.assert_print_contains("Unexpected exception of type '");
  fixture.assert_print_contains("runtime_error");
  fixture.assert_print_contains("' was thrown: exception text");
#else
  fixture.assert_print_contains(
      "Unexpected exception of unknown type was thrown"
  );
#endif
  LONGS_EQUAL(0, stop_after_failure);
  mu::tiny::test::TestShell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

TEST(TestShell, StandardExceptionIsRethrownIfEnabled)
{
  bool initial_rethrow_exceptions =
      mu::tiny::test::TestShell::is_rethrowing_exceptions();
  bool exception_rethrown = false;
  stop_after_failure = 0;
  mu::tiny::test::TestShell::set_rethrow_exceptions(true);
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
  mu::tiny::test::TestShell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

#endif // MUTINY_USE_STD_CPP_LIB

TEST(TestShell, TeardownStopsAfterUnknownExceptionIsThrown)
{
  bool initial_rethrow_exceptions =
      mu::tiny::test::TestShell::is_rethrowing_exceptions();
  mu::tiny::test::TestShell::set_rethrow_exceptions(false);
  stop_after_failure = 0;
  should_throw_exception = true;
  fixture.set_teardown(thrown_unknown_exception_method);
  fixture.run_all_tests();
  LONGS_EQUAL(1, fixture.get_failure_count());
  fixture.assert_print_contains(
      "Unexpected exception of unknown type was thrown"
  );
  LONGS_EQUAL(0, stop_after_failure);
  mu::tiny::test::TestShell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

TEST(TestShell, TeardownUnknownExceptionIsRethrownIfEnabled)
{
  bool initial_rethrow_exceptions =
      mu::tiny::test::TestShell::is_rethrowing_exceptions();
  bool exception_rethrown = false;
  stop_after_failure = 0;
  mu::tiny::test::TestShell::set_rethrow_exceptions(true);
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
      "Unexpected exception of unknown type was thrown"
  );
  LONGS_EQUAL(0, stop_after_failure);
  mu::tiny::test::TestShell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

#if MUTINY_USE_STD_CPP_LIB

TEST(TestShell, TeardownStopsAfterStandardExceptionIsThrown)
{
  bool initial_rethrow_exceptions =
      mu::tiny::test::TestShell::is_rethrowing_exceptions();
  mu::tiny::test::TestShell::set_rethrow_exceptions(false);
  stop_after_failure = 0;
  should_throw_exception = true;
  fixture.set_teardown(thrown_standard_exception_method);
  fixture.run_all_tests();
  LONGS_EQUAL(1, fixture.get_failure_count());
#if MUTINY_HAVE_RTTI
  fixture.assert_print_contains("Unexpected exception of type '");
  fixture.assert_print_contains("runtime_error");
  fixture.assert_print_contains("' was thrown: exception text");
#else
  fixture.assert_print_contains(
      "Unexpected exception of unknown type was thrown"
  );
#endif
  LONGS_EQUAL(0, stop_after_failure);
  mu::tiny::test::TestShell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

TEST(TestShell, TeardownStandardExceptionIsRethrownIfEnabled)
{
  bool initial_rethrow_exceptions =
      mu::tiny::test::TestShell::is_rethrowing_exceptions();
  bool exception_rethrown = false;
  stop_after_failure = 0;
  mu::tiny::test::TestShell::set_rethrow_exceptions(true);
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
  mu::tiny::test::TestShell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

#endif // MUTINY_USE_STD_CPP_LIB
#endif // MUTINY_HAVE_EXCEPTIONS

TEST(TestShell, veryVebose)
{
  mu::tiny::test::TestShell shell("Group", "name", __FILE__, __LINE__);
  mu::tiny::test::StringBufferTestOutput normal_output;
  normal_output.verbose(
      mu::tiny::test::TestOutput::VerbosityLevel::very_verbose
  );
  mu::tiny::test::NullTestPlugin plugin;

  mu::tiny::test::TestResult result(normal_output);
  shell.run_one_test_in_current_process(&plugin, result);
  STRCMP_CONTAINS(
      "\n------ before runTest", normal_output.get_output().c_str()
  );
}

class DefaultTestShell : public mu::tiny::test::TestShell
{};

TEST(TestShell, this_test_covers_the_TestShell_createTest_and_Utest_testBody_methods)
{
  DefaultTestShell shell;
  fixture.add_test(&shell);
  fixture.run_all_tests();
  LONGS_EQUAL(2, fixture.get_test_count());
}

#if MUTINY_HAVE_EXCEPTIONS

namespace {
bool destructor_was_called_on_failed_test = false;

void destructor_called_for_local_objects()
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

TEST(TestShell, DestructorIsCalledForLocalObjectsWhenTheTestFails)
{
  fixture.set_test_function(destructor_called_for_local_objects);
  fixture.run_all_tests();
  CHECK(destructor_was_called_on_failed_test);
}

#endif
