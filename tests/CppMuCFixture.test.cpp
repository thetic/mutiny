#include "fail_method_c_body.h"

#include "CppMu/CppMu.hpp"
#include "CppMu/TestTestingFixture.hpp"

/*
 * These tests verify that the C failure path (longjmp-based) does not trigger
 * C++ stack unwinding, so destructors of locals in the call stack are not
 * called.
 */

namespace {

bool has_destructor_of_the_destructor_checked_been_called;

class HasTheDestructorBeenCalledChecker
{
public:
  HasTheDestructorBeenCalledChecker() = default;
  ~HasTheDestructorBeenCalledChecker()
  {
    has_destructor_of_the_destructor_checked_been_called = true;
  }
};

void fail_method()
{
  HasTheDestructorBeenCalledChecker checker;
  cppmu_fail_method_c_body();
}

bool cppmu_has_crashed;

void crash_method()
{
  cppmu_has_crashed = true;
}

} // namespace

TEST_GROUP(CppMuCFixture)
{
  cppmu::TestTestingFixture* fixture;
  TEST_SETUP()
  {
    has_destructor_of_the_destructor_checked_been_called = false;
    fixture = new cppmu::TestTestingFixture();
  }
  TEST_TEARDOWN() { delete fixture; }
};

TEST(CppMuCFixture, checkFail)
{
  fixture->set_test_function(fail_method);
  fixture->run_all_tests();
  LONGS_EQUAL(1, fixture->get_failure_count());
  fixture->assert_print_contains("Booo");
  fixture->assert_print_contains("fail_method_c_body");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(CppMuCFixture, doesNotCrashIfNotSetToCrash)
{
  cppmu_has_crashed = false;
  cppmu::TestShell::set_crash_method(crash_method);
  fixture->set_test_function(fail_method);

  fixture->run_all_tests();

  CHECK_FALSE(cppmu_has_crashed);
  LONGS_EQUAL(1, fixture->get_failure_count());
  CHECK(!has_destructor_of_the_destructor_checked_been_called);

  cppmu::TestShell::reset_crash_method();
}

TEST(CppMuCFixture, doesCrashIfSetToCrash)
{
  cppmu_has_crashed = false;
  cppmu::TestShell::set_crash_on_fail();
  cppmu::TestShell::set_crash_method(crash_method);
  fixture->set_test_function(fail_method);

  fixture->run_all_tests();

  CHECK(cppmu_has_crashed);
  CHECK(!has_destructor_of_the_destructor_checked_been_called);

  cppmu::TestShell::restore_default_test_terminator();
  cppmu::TestShell::reset_crash_method();
}
