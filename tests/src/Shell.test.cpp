#include "mu/tiny/test/Shell.hpp"

#include "mu/tiny/test/Output.hpp"
#include "mu/tiny/test/Plugin.hpp"
#include "mu/tiny/test/StringBufferOutput.hpp"
#include "mu/tiny/test/TestingFixture.hpp"

#include "mu/tiny/test.hpp"

#include <limits.h>
#include <math.h>

namespace {
class MockPlugin : public mu::tiny::test::Plugin
{
public:
  MockPlugin()
    : mu::tiny::test::Plugin("MockPlugin")
  {
  }
  void run_all_pre_test_action(
      mu::tiny::test::Shell& test,
      mu::tiny::test::Result& result
  ) override
  {
    pre_test_action(test, result);
  }
  void run_all_post_test_action(
      mu::tiny::test::Shell& test,
      mu::tiny::test::Result& result
  ) override
  {
    post_test_action(test, result);
  }
};
} // namespace

#define CHECK_TEST_FAILS_PROPER_WITH_TEXT(text)                                \
  fixture.check_test_fails_with_proper_test_location(text, __FILE__, __LINE__)

TEST_GROUP(Shell)
{
  mu::tiny::test::TestingFixture fixture;
};

namespace {
void fail_method()
{
  FAIL_TEST("This test fails");
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
  PASS_TEST();
  FAIL_TEST("Should not get here");
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
  FAIL_TEST("fail");
  stop_after_failure++;
}

int code_after_skip = 0;
void skip_test_method()
{
  SKIP_TEST("skipping this test");
  code_after_skip++;
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

TEST(Shell, compareDoubles)
{
  CHECK(mu::tiny::test::approx_equal(1.0, 1.001, 0.01));
  CHECK(!mu::tiny::test::approx_equal(1.0, 1.1, 0.05));
  double a = 1.2345678;
  CHECK(mu::tiny::test::approx_equal(a, a, 0.000000001));
}

#ifdef NAN
TEST(Shell, compareDoublesNaN)
{
  CHECK(!mu::tiny::test::approx_equal(static_cast<double>(NAN), 1.001, 0.01));
  CHECK(!mu::tiny::test::approx_equal(1.0, static_cast<double>(NAN), 0.01));
  CHECK(!mu::tiny::test::approx_equal(1.0, 1.001, static_cast<double>(NAN)));
}
#endif

#ifdef INFINITY
TEST(Shell, compareDoublesInf)
{
  CHECK(
      !mu::tiny::test::approx_equal(static_cast<double>(INFINITY), 1.0, 0.01)
  );
  CHECK(
      !mu::tiny::test::approx_equal(1.0, static_cast<double>(INFINITY), 0.01)
  );
  CHECK(mu::tiny::test::approx_equal(1.0, -1.0, static_cast<double>(INFINITY)));
  CHECK(
      mu::tiny::test::approx_equal(
          static_cast<double>(INFINITY), static_cast<double>(INFINITY), 0.01
      )
  );
  CHECK(
      mu::tiny::test::approx_equal(
          static_cast<double>(INFINITY),
          static_cast<double>(INFINITY),
          static_cast<double>(INFINITY)
      )
  );
}
#endif

TEST(Shell, compareFloats)
{
  CHECK(mu::tiny::test::approx_equal(1.0f, 1.001f, 0.01f));
  CHECK(!mu::tiny::test::approx_equal(1.0f, 1.1f, 0.05f));
}

#ifdef NAN
TEST(Shell, compareFloatsNaN)
{
  CHECK(!mu::tiny::test::approx_equal(NAN, 1.0f, 0.01f));
  CHECK(!mu::tiny::test::approx_equal(1.0f, NAN, 0.01f));
  CHECK(!mu::tiny::test::approx_equal(1.0f, 1.001f, NAN));
}
#endif

TEST(Shell, compareIntegers)
{
  CHECK(mu::tiny::test::approx_equal(1000, 1001, 10));
  CHECK(!mu::tiny::test::approx_equal(1000, 1020, 10));
  CHECK(mu::tiny::test::approx_equal(-5, 5, 10));
  CHECK(!mu::tiny::test::approx_equal(-5, 6, 10));
  CHECK(mu::tiny::test::approx_equal(3u, 5u, 10u));
  CHECK(!mu::tiny::test::approx_equal(3u, 5u, 1u));
  CHECK(mu::tiny::test::approx_equal(5u, 3u, 10u));
  CHECK(!mu::tiny::test::approx_equal(5u, 3u, 1u));
}

TEST(Shell, FailWillIncreaseTheAmountOfChecks)
{
  fixture.set_test_function(fail_method);
  fixture.run_all_tests();
  CHECK_EQUAL(size_t{ 1 }, fixture.get_check_count());
}

TEST(Shell, PassedCheckEqualWillIncreaseTheAmountOfChecks)
{
  fixture.set_test_function(passing_check_equal_test_method);
  fixture.run_all_tests();
  CHECK_EQUAL(size_t{ 1 }, fixture.get_check_count());
}

TEST(Shell, SetTestFunctionExecFunctionOverloadRunsTheFunction)
{
  PassingExecFunction exec_func;
  fixture.set_test_function(&exec_func);
  fixture.run_all_tests();
  CHECK(exec_func.has_run);
}

SKIPPED_TEST(Shell, IgnoreTestAccessingFixture)
{
  CHECK(fixture.get_check_count() == 0);
}

TEST(Shell, MacrosUsedInSetup)
{
  fixture.set_setup(fail_method);
  fixture.set_test_function(simple_passing_method);
  fixture.run_all_tests();
  CHECK_EQUAL(size_t{ 1 }, fixture.get_failure_count());
}

TEST(Shell, MacrosUsedInTearDown)
{
  fixture.set_teardown(fail_method);
  fixture.set_test_function(simple_passing_method);
  fixture.run_all_tests();
  CHECK_EQUAL(size_t{ 1 }, fixture.get_failure_count());
}

TEST(Shell, ExitLeavesQuietly)
{
  fixture.set_test_function(exit_test_method);
  fixture.run_all_tests();
  CHECK_EQUAL(size_t{ 0 }, fixture.get_failure_count());
}

TEST(Shell, FailWillNotCrashIfNotEnabled)
{
  mutiny_has_crashed = false;
  mu::tiny::test::Shell::set_crash_method(crash_method);

  fixture.set_test_function(fail_method);
  fixture.run_all_tests();

  CHECK(!mutiny_has_crashed);
  CHECK_EQUAL(size_t{ 1 }, fixture.get_failure_count());

  mu::tiny::test::Shell::reset_crash_method();
}

TEST(Shell, FailWillCrashIfEnabled)
{
  mutiny_has_crashed = false;
  mu::tiny::test::Shell::set_crash_on_fail();
  mu::tiny::test::Shell::set_crash_method(crash_method);

  fixture.set_test_function(fail_method);
  fixture.run_all_tests();

  CHECK(mutiny_has_crashed);

  mu::tiny::test::Shell::restore_default_test_terminator();
  mu::tiny::test::Shell::reset_crash_method();
}

TEST(Shell, TeardownCalledAfterTestFailure)
{
  teardown_called = 0;
  fixture.set_teardown(teardown_method);
  fixture.set_test_function(fail_method);
  fixture.run_all_tests();
  CHECK_EQUAL(size_t{ 1 }, fixture.get_failure_count());
  CHECK_EQUAL(1, teardown_called);
}

TEST(Shell, TestStopsAfterTestFailure)
{
  stop_after_failure = 0;
  fixture.set_test_function(stop_after_failure_method);
  fixture.run_all_tests();
  CHECK(fixture.has_test_failed());
  CHECK_EQUAL(size_t{ 1 }, fixture.get_failure_count());
  CHECK_EQUAL(0, stop_after_failure);
}

TEST(Shell, TestStopsAfterSetupFailure)
{
  stop_after_failure = 0;
  fixture.set_setup(stop_after_failure_method);
  fixture.set_teardown(stop_after_failure_method);
  fixture.set_test_function(fail_method);
  fixture.run_all_tests();
  CHECK_EQUAL(size_t{ 2 }, fixture.get_failure_count());
  CHECK_EQUAL(0, stop_after_failure);
}

TEST(Shell, SkipTestStopsExecutionWithNoFailure)
{
  code_after_skip = 0;
  fixture.set_test_function(skip_test_method);
  fixture.run_all_tests();
  CHECK_EQUAL(size_t{ 0 }, fixture.get_failure_count());
  CHECK_EQUAL(0, code_after_skip);
}

#if MUTINY_HAVE_EXCEPTIONS

TEST(Shell, TestStopsAfterUnknownExceptionIsThrown)
{
  bool initial_rethrow_exceptions =
      mu::tiny::test::Shell::is_rethrowing_exceptions();
  mu::tiny::test::Shell::set_rethrow_exceptions(false);
  stop_after_failure = 0;
  should_throw_exception = true;
  fixture.set_test_function(thrown_unknown_exception_method);
  fixture.run_all_tests();
  CHECK_EQUAL(size_t{ 1 }, fixture.get_failure_count());
  fixture.assert_print_contains(
      "Unexpected exception of unknown type was thrown"
  );
  CHECK_EQUAL(0, stop_after_failure);
  mu::tiny::test::Shell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

TEST(Shell, NoExceptionIsRethrownIfEnabledButNotThrown)
{
  bool initial_rethrow_exceptions =
      mu::tiny::test::Shell::is_rethrowing_exceptions();
  bool exception_rethrown = false;
  stop_after_failure = 0;
  mu::tiny::test::Shell::set_rethrow_exceptions(true);
  should_throw_exception = false;
  fixture.set_test_function(thrown_unknown_exception_method);
  try {
    fixture.run_all_tests();
  } catch (...) {
    exception_rethrown = true;
  }
  CHECK(!exception_rethrown);
  CHECK_EQUAL(size_t{ 0 }, fixture.get_failure_count());
  CHECK_EQUAL(1, stop_after_failure);
  mu::tiny::test::Shell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

TEST(Shell, UnknownExceptionIsRethrownIfEnabled)
{
  bool initial_rethrow_exceptions =
      mu::tiny::test::Shell::is_rethrowing_exceptions();
  bool exception_rethrown = false;
  stop_after_failure = 0;
  mu::tiny::test::Shell::set_rethrow_exceptions(true);
  should_throw_exception = true;
  fixture.set_test_function(thrown_unknown_exception_method);
  try {
    fixture.run_all_tests();
    stop_after_failure++;
  } catch (...) {
    exception_rethrown = true;
  }
  CHECK(exception_rethrown);
  CHECK_EQUAL(size_t{ 1 }, fixture.get_failure_count());
  fixture.assert_print_contains(
      "Unexpected exception of unknown type was thrown"
  );
  CHECK_EQUAL(0, stop_after_failure);
  mu::tiny::test::Shell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

#if MUTINY_USE_STD_CPP_LIB

TEST(Shell, TestStopsAfterStandardExceptionIsThrown)
{
  bool initial_rethrow_exceptions =
      mu::tiny::test::Shell::is_rethrowing_exceptions();
  mu::tiny::test::Shell::set_rethrow_exceptions(false);
  stop_after_failure = 0;
  should_throw_exception = true;
  fixture.set_test_function(thrown_standard_exception_method);
  fixture.run_all_tests();
  CHECK_EQUAL(size_t{ 1 }, fixture.get_failure_count());
#if MUTINY_HAVE_RTTI
  fixture.assert_print_contains("Unexpected exception of type '");
  fixture.assert_print_contains("runtime_error");
  fixture.assert_print_contains("' was thrown: exception text");
#else
  fixture.assert_print_contains(
      "Unexpected exception of unknown type was thrown"
  );
#endif
  CHECK_EQUAL(0, stop_after_failure);
  mu::tiny::test::Shell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

TEST(Shell, StandardExceptionIsRethrownIfEnabled)
{
  bool initial_rethrow_exceptions =
      mu::tiny::test::Shell::is_rethrowing_exceptions();
  bool exception_rethrown = false;
  stop_after_failure = 0;
  mu::tiny::test::Shell::set_rethrow_exceptions(true);
  should_throw_exception = true;
  fixture.set_test_function(thrown_standard_exception_method);
  try {
    fixture.run_all_tests();
    stop_after_failure++;
  } catch (const std::exception&) {
    exception_rethrown = true;
  }
  CHECK(exception_rethrown);
  CHECK_EQUAL(size_t{ 1 }, fixture.get_failure_count());
  fixture.assert_print_contains("Unexpected exception of type '");
  fixture.assert_print_contains("runtime_error");
  fixture.assert_print_contains("' was thrown: exception text");
  CHECK_EQUAL(0, stop_after_failure);
  mu::tiny::test::Shell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

#endif // MUTINY_USE_STD_CPP_LIB

TEST(Shell, TeardownStopsAfterUnknownExceptionIsThrown)
{
  bool initial_rethrow_exceptions =
      mu::tiny::test::Shell::is_rethrowing_exceptions();
  mu::tiny::test::Shell::set_rethrow_exceptions(false);
  stop_after_failure = 0;
  should_throw_exception = true;
  fixture.set_teardown(thrown_unknown_exception_method);
  fixture.run_all_tests();
  CHECK_EQUAL(size_t{ 1 }, fixture.get_failure_count());
  fixture.assert_print_contains(
      "Unexpected exception of unknown type was thrown"
  );
  CHECK_EQUAL(0, stop_after_failure);
  mu::tiny::test::Shell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

TEST(Shell, TeardownUnknownExceptionIsRethrownIfEnabled)
{
  bool initial_rethrow_exceptions =
      mu::tiny::test::Shell::is_rethrowing_exceptions();
  bool exception_rethrown = false;
  stop_after_failure = 0;
  mu::tiny::test::Shell::set_rethrow_exceptions(true);
  should_throw_exception = true;
  fixture.set_teardown(thrown_unknown_exception_method);
  try {
    fixture.run_all_tests();
    stop_after_failure++;
  } catch (...) {
    exception_rethrown = true;
  }
  CHECK(exception_rethrown);
  CHECK_EQUAL(size_t{ 1 }, fixture.get_failure_count());
  fixture.assert_print_contains(
      "Unexpected exception of unknown type was thrown"
  );
  CHECK_EQUAL(0, stop_after_failure);
  mu::tiny::test::Shell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

#if MUTINY_USE_STD_CPP_LIB

TEST(Shell, TeardownStopsAfterStandardExceptionIsThrown)
{
  bool initial_rethrow_exceptions =
      mu::tiny::test::Shell::is_rethrowing_exceptions();
  mu::tiny::test::Shell::set_rethrow_exceptions(false);
  stop_after_failure = 0;
  should_throw_exception = true;
  fixture.set_teardown(thrown_standard_exception_method);
  fixture.run_all_tests();
  CHECK_EQUAL(size_t{ 1 }, fixture.get_failure_count());
#if MUTINY_HAVE_RTTI
  fixture.assert_print_contains("Unexpected exception of type '");
  fixture.assert_print_contains("runtime_error");
  fixture.assert_print_contains("' was thrown: exception text");
#else
  fixture.assert_print_contains(
      "Unexpected exception of unknown type was thrown"
  );
#endif
  CHECK_EQUAL(0, stop_after_failure);
  mu::tiny::test::Shell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

TEST(Shell, TeardownStandardExceptionIsRethrownIfEnabled)
{
  bool initial_rethrow_exceptions =
      mu::tiny::test::Shell::is_rethrowing_exceptions();
  bool exception_rethrown = false;
  stop_after_failure = 0;
  mu::tiny::test::Shell::set_rethrow_exceptions(true);
  should_throw_exception = true;
  fixture.set_teardown(thrown_standard_exception_method);
  try {
    fixture.run_all_tests();
    stop_after_failure++;
  } catch (const std::exception&) {
    exception_rethrown = true;
  }
  CHECK(exception_rethrown);
  CHECK_EQUAL(size_t{ 1 }, fixture.get_failure_count());
  fixture.assert_print_contains("Unexpected exception of type '");
  fixture.assert_print_contains("runtime_error");
  fixture.assert_print_contains("' was thrown: exception text");
  CHECK_EQUAL(0, stop_after_failure);
  mu::tiny::test::Shell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

#endif // MUTINY_USE_STD_CPP_LIB
#endif // MUTINY_HAVE_EXCEPTIONS

TEST(Shell, veryVebose)
{
  mu::tiny::test::Shell shell("Group", "name", __FILE__, __LINE__);
  mu::tiny::test::StringBufferOutput normal_output;
  normal_output.verbose(
      mu::tiny::test::Output::MutinyVerbosityLevel::very_verbose
  );
  MockPlugin plugin;

  mu::tiny::test::Result result(normal_output);
  shell.run_one_test_in_current_process(&plugin, result);
  STRCMP_CONTAINS(
      "\n------ before runTest", normal_output.get_output().c_str()
  );
}

class DefaultTestShell : public mu::tiny::test::Shell
{};

TEST(Shell, this_test_covers_the_TestShell_createTest_and_Utest_testBody_methods)
{
  DefaultTestShell shell;
  fixture.add_test(&shell);
  fixture.run_all_tests();
  CHECK_EQUAL(size_t{ 2 }, fixture.get_test_count());
}

TEST(Shell, getIgnoreCountReturnsNumberOfIgnoredTests)
{
  mu::tiny::test::IgnoredShell ignored;
  fixture.add_test(&ignored);
  fixture.run_all_tests();
  CHECK_EQUAL(size_t{ 1 }, fixture.get_ignore_count());
}

TEST(Shell, checkTestFailsWithProperTestLocationFailsWhenFailureCountIsNotOne)
{
  mu::tiny::test::TestingFixture inner;
  inner.set_test_function([]() {
    mu::tiny::test::TestingFixture f;
    // f has 0 failures — check_test_fails_with_proper_test_location must fire
    f.check_test_fails_with_proper_test_location("text", __FILE__, __LINE__);
  });
  inner.run_all_tests();
  CHECK(inner.has_test_failed());
}

TEST(Shell, checkTestFailsWithProperTestLocationFailsWhenLineWasExecutedAfterCheck)
{
  mu::tiny::test::TestingFixture inner;
  inner.set_test_function([]() {
    mu::tiny::test::TestingFixture f;
    f.set_test_function([]() { FAIL_TEST("deliberate"); });
    f.run_all_tests();
    // Simulate a check macro that didn't halt execution
    mu::tiny::test::TestingFixture::line_executed_after_check();
    // f has 1 failure and the flag is set — the second guard must fire
    f.check_test_fails_with_proper_test_location(
        "deliberate", __FILE__, __LINE__
    );
  });
  inner.run_all_tests();
  CHECK(inner.has_test_failed());
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
  FAIL_TEST("fail");
}
} // namespace

TEST(Shell, DestructorIsCalledForLocalObjectsWhenTheTestFails)
{
  fixture.set_test_function(destructor_called_for_local_objects);
  fixture.run_all_tests();
  CHECK(destructor_was_called_on_failed_test);
}

#endif
