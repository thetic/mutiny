#include "fail_method_c_body.h"

#include "mutiny/test/TestingFixture.hpp"

#include "mutiny/test.hpp"

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
  mutiny_fail_method_c_body();
}

bool mutiny_has_crashed;

void crash_method()
{
  mutiny_has_crashed = true;
}

} // namespace

TEST_GROUP(MutinyCFixture)
{
  mu::tiny::test::TestingFixture* fixture;
  void setup() override
  {
    has_destructor_of_the_destructor_checked_been_called = false;
    fixture = new mu::tiny::test::TestingFixture();
  }
  void teardown() override { delete fixture; }
};

TEST(MutinyCFixture, checkFail)
{
  fixture->set_test_function(fail_method);
  fixture->run_all_tests();
  CHECK_EQUAL(size_t{ 1 }, fixture->get_failure_count());
  fixture->assert_print_contains("Booo");
  fixture->assert_print_contains("fail_method_c_body");
  CHECK(!has_destructor_of_the_destructor_checked_been_called);
}

TEST(MutinyCFixture, doesNotCrashIfNotSetToCrash)
{
  mutiny_has_crashed = false;
  mu::tiny::test::Shell::set_crash_method(crash_method);
  fixture->set_test_function(fail_method);

  fixture->run_all_tests();

  CHECK_FALSE(mutiny_has_crashed);
  CHECK_EQUAL(size_t{ 1 }, fixture->get_failure_count());
  CHECK(!has_destructor_of_the_destructor_checked_been_called);

  mu::tiny::test::Shell::reset_crash_method();
}

TEST(MutinyCFixture, doesCrashIfSetToCrash)
{
  mutiny_has_crashed = false;
  mu::tiny::test::Shell::set_crash_on_fail();
  mu::tiny::test::Shell::set_crash_method(crash_method);
  fixture->set_test_function(fail_method);

  fixture->run_all_tests();

  CHECK(mutiny_has_crashed);
  CHECK(!has_destructor_of_the_destructor_checked_been_called);

  mu::tiny::test::Shell::restore_default_test_terminator();
  mu::tiny::test::Shell::reset_crash_method();
}
