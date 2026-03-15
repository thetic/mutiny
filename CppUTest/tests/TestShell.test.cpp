#include "CppUTest/TestShell.hpp"

#include "CppUTest/ExecFunctionTestShell.hpp"
#include "CppUTest/StringBufferTestOutput.hpp"
#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestOutput.hpp"
#include "CppUTest/TestTestingFixture.hpp"

#include <math.h>

TEST_GROUP(TestShell)
{
  cpputest::TestTestingFixture fixture;
};

namespace {
void
fail_method()
{
  FAIL("This test fails");
}

void
passing_test_method()
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

bool cpputest_has_crashed;

void
crash_method()
{
  cpputest_has_crashed = true;
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

#if CPPUTEST_HAVE_EXCEPTIONS
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

#if CPPUTEST_USE_STD_CPP_LIB
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
  CHECK(cpputest::doubles_equal(1.0, 1.001, 0.01));
  CHECK(!cpputest::doubles_equal(1.0, 1.1, 0.05));
  double a = 1.2345678;
  CHECK(cpputest::doubles_equal(a, a, 0.000000001));
}

#ifdef NAN
TEST(TestShell, compareDoublesNaN)
{
  CHECK(!cpputest::doubles_equal(static_cast<double>(NAN), 1.001, 0.01));
  CHECK(!cpputest::doubles_equal(1.0, static_cast<double>(NAN), 0.01));
  CHECK(!cpputest::doubles_equal(1.0, 1.001, static_cast<double>(NAN)));
}
#endif

#ifdef INFINITY
TEST(TestShell, compareDoublesInf)
{
  CHECK(!cpputest::doubles_equal(static_cast<double>(INFINITY), 1.0, 0.01));
  CHECK(!cpputest::doubles_equal(1.0, static_cast<double>(INFINITY), 0.01));
  CHECK(cpputest::doubles_equal(1.0, -1.0, static_cast<double>(INFINITY)));
  CHECK(cpputest::doubles_equal(
      static_cast<double>(INFINITY), static_cast<double>(INFINITY), 0.01));
  CHECK(cpputest::doubles_equal(static_cast<double>(INFINITY),
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

IGNORE_TEST(TestShell, IgnoreTestAccessingFixture)
{
  CHECK(fixture.get_check_count() == 0);
}

TEST(TestShell, MacrosUsedInSetup)
{
  fixture.set_setup(fail_method);
  fixture.set_test_function(passing_test_method);
  fixture.run_all_tests();
  LONGS_EQUAL(1, fixture.get_failure_count());
}

TEST(TestShell, MacrosUsedInTearDown)
{
  fixture.set_teardown(fail_method);
  fixture.set_test_function(passing_test_method);
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
  cpputest_has_crashed = false;
  cpputest::TestShell::set_crash_method(crash_method);

  fixture.set_test_function(fail_method);
  fixture.run_all_tests();

  CHECK_FALSE(cpputest_has_crashed);
  LONGS_EQUAL(1, fixture.get_failure_count());

  cpputest::TestShell::reset_crash_method();
}

TEST(TestShell, FailWillCrashIfEnabled)
{
  cpputest_has_crashed = false;
  cpputest::TestShell::set_crash_on_fail();
  cpputest::TestShell::set_crash_method(crash_method);

  fixture.set_test_function(fail_method);
  fixture.run_all_tests();

  CHECK(cpputest_has_crashed);

  cpputest::TestShell::restore_default_test_terminator();
  cpputest::TestShell::reset_crash_method();
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

#if CPPUTEST_HAVE_EXCEPTIONS

TEST(TestShell, TestStopsAfterUnknownExceptionIsThrown)
{
  bool initial_rethrow_exceptions =
      cpputest::TestShell::is_rethrowing_exceptions();
  cpputest::TestShell::set_rethrow_exceptions(false);
  stop_after_failure = 0;
  should_throw_exception = true;
  fixture.set_test_function(thrown_unknown_exception_method);
  fixture.run_all_tests();
  LONGS_EQUAL(1, fixture.get_failure_count());
  fixture.assert_print_contains(
      "Unexpected exception of unknown type was thrown");
  LONGS_EQUAL(0, stop_after_failure);
  cpputest::TestShell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

TEST(TestShell, NoExceptionIsRethrownIfEnabledButNotThrown)
{
  bool initial_rethrow_exceptions =
      cpputest::TestShell::is_rethrowing_exceptions();
  bool exception_rethrown = false;
  stop_after_failure = 0;
  cpputest::TestShell::set_rethrow_exceptions(true);
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
  cpputest::TestShell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

TEST(TestShell, UnknownExceptionIsRethrownIfEnabled)
{
  bool initial_rethrow_exceptions =
      cpputest::TestShell::is_rethrowing_exceptions();
  bool exception_rethrown = false;
  stop_after_failure = 0;
  cpputest::TestShell::set_rethrow_exceptions(true);
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
  cpputest::TestShell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

#if CPPUTEST_USE_STD_CPP_LIB

TEST(TestShell, TestStopsAfterStandardExceptionIsThrown)
{
  bool initial_rethrow_exceptions =
      cpputest::TestShell::is_rethrowing_exceptions();
  cpputest::TestShell::set_rethrow_exceptions(false);
  stop_after_failure = 0;
  should_throw_exception = true;
  fixture.set_test_function(thrown_standard_exception_method);
  fixture.run_all_tests();
  LONGS_EQUAL(1, fixture.get_failure_count());
#if CPPUTEST_HAVE_RTTI
  fixture.assert_print_contains("Unexpected exception of type '");
  fixture.assert_print_contains("runtime_error");
  fixture.assert_print_contains("' was thrown: exception text");
#else
  fixture.assertPrintContains(
      "Unexpected exception of unknown type was thrown");
#endif
  LONGS_EQUAL(0, stop_after_failure);
  cpputest::TestShell::set_rethrow_exceptions(initial_rethrow_exceptions);
}

TEST(TestShell, StandardExceptionIsRethrownIfEnabled)
{
  bool initial_rethrow_exceptions =
      cpputest::TestShell::is_rethrowing_exceptions();
  bool exception_rethrown = false;
  stop_after_failure = 0;
  cpputest::TestShell::set_rethrow_exceptions(true);
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
  cpputest::TestShell::set_rethrow_exceptions(initial_rethrow_exceptions);
}
#endif // CPPUTEST_USE_STD_CPP_LIB
#endif // CPPUTEST_HAVE_EXCEPTIONS

TEST(TestShell, veryVebose)
{
  cpputest::TestShell shell("Group", "name", __FILE__, __LINE__);
  cpputest::StringBufferTestOutput normal_output;
  normal_output.verbose(cpputest::TestOutput::VerbosityLevel::very_verbose);
  cpputest::NullTestPlugin plugin;

  cpputest::TestResult result(normal_output);
  shell.run_one_test_in_current_process(&plugin, result);
  STRCMP_CONTAINS(
      "\n------ before runTest", normal_output.get_output().c_str());
}

class DefaultTestShell : public cpputest::TestShell
{};

TEST(TestShell,
    this_test_covers_the_TestShell_createTest_and_Utest_testBody_methods)
{
  DefaultTestShell shell;
  fixture.add_test(&shell);
  fixture.run_all_tests();
  LONGS_EQUAL(2, fixture.get_test_count());
}

#if CPPUTEST_HAVE_EXCEPTIONS

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

TEST(TestShell, DestructorIsCalledForLocalObjectsWhenTheTestFails)
{
  fixture.set_test_function(destructor_called_for_local_objects);
  fixture.run_all_tests();
  CHECK(destructor_was_called_on_failed_test);
}

#endif

TEST_BASE(MyOwnTest)
{
  MyOwnTest()
    : in_test(false)
  {
  }
  bool in_test;

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

class NullParameterTest : public cpputest::TestShell
{};

TEST(UtestMyOwn, NullParameters)
{
  NullParameterTest null_test; /* Bug fix tests for creating a test without a
                                 name, fix in String */
  cpputest::TestFilter empty_filter;
  CHECK(null_test.should_run(&empty_filter, &empty_filter));
}
