#include "CppUTest/Extensions/IEEE754ExceptionsPlugin.hpp"

#include "IEEE754ExceptionsPlugin.h"

#include "CppUTest/CommandLineTestRunner.hpp"
#include "CppUTest/CppUTest.hpp"
#include "CppUTest/TestRegistry.hpp"
#include "CppUTest/TestTestingFixture.hpp"

TEST_GROUP(IEEE754ExceptionsPlugin)
{
  cpputest::TestTestingFixture fixture;
  cpputest::extensions::IEEE754ExceptionsPlugin ieee754_plugin;

  void setup(void) override { fixture.install_plugin(&ieee754_plugin); }
};

#if CPPUTEST_HAVE_FENV

TEST(IEEE754ExceptionsPlugin, should_fail_when_FE_DIVBYZERO_is_set)
{
  fixture.set_test_function(set_divisionbyzero_c);
  fixture.run_all_tests();
  fixture.assert_print_contains("IEEE754_CHECK_CLEAR(FE_DIVBYZERO) failed");
}

TEST(IEEE754ExceptionsPlugin, should_fail_when_FE_OVERFLOW_is_set)
{
  fixture.set_test_function(set_overflow_c);
  fixture.run_all_tests();
  fixture.assert_print_contains("IEEE754_CHECK_CLEAR(FE_OVERFLOW) failed");
}

TEST(IEEE754ExceptionsPlugin, should_fail_when_FE_UNDERFLOW_is_set)
{
  fixture.set_test_function(set_underflow_c);
  fixture.run_all_tests();
  fixture.assert_print_contains("IEEE754_CHECK_CLEAR(FE_UNDERFLOW) failed");
}

TEST(IEEE754ExceptionsPlugin, should_fail_when_FE_INEXACT_is_set_and_enabled)
{
  cpputest::extensions::IEEE754ExceptionsPlugin::enable_inexact();
  fixture.set_test_function(set_inexact_c);
  fixture.run_all_tests();
  fixture.assert_print_contains("IEEE754_CHECK_CLEAR(FE_INEXACT) failed");
}

TEST(IEEE754ExceptionsPlugin,
    should_succeed_when_FE_INEXACT_is_set_and_disabled)
{
  cpputest::extensions::IEEE754ExceptionsPlugin::enable_inexact();
  cpputest::extensions::IEEE754ExceptionsPlugin::disable_inexact();
  fixture.set_test_function(set_inexact_c);
  fixture.run_all_tests();
  fixture.assert_print_contains("OK");
}

TEST(IEEE754ExceptionsPlugin,
    should_succeed_with_5_checks_when_no_flags_are_set)
{
  cpputest::extensions::IEEE754ExceptionsPlugin::enable_inexact();
  fixture.set_test_function(set_nothing_c);
  fixture.run_all_tests();
  fixture.assert_print_contains(
      "OK (1 tests, 1 ran, 5 checks, 0 ignored, 0 filtered out");
  cpputest::extensions::IEEE754ExceptionsPlugin::disable_inexact();
}

TEST(IEEE754ExceptionsPlugin, should_check_five_times_when_all_flags_are_set)
{
  fixture.set_test_function(set_everything_c);
  fixture.run_all_tests();
  LONGS_EQUAL(5, fixture.get_check_count());
}

TEST(IEEE754ExceptionsPlugin, should_fail_only_once_when_all_flags_are_set)
{
  fixture.set_test_function(set_everything_c);
  fixture.run_all_tests();
  LONGS_EQUAL(1, fixture.get_failure_count());
}

namespace {
void
set_everything_but_already_failed(void)
{
  set_everything_c();
  CHECK(1 == 2);
}
}

TEST(IEEE754ExceptionsPlugin,
    should_not_fail_again_when_test_has_already_failed)
{
  fixture.set_test_function(set_everything_but_already_failed);
  fixture.run_all_tests();
  CHECK(cpputest::extensions::IEEE754ExceptionsPlugin::
          check_ieee754_overflow_exception_flag());
  CHECK(cpputest::extensions::IEEE754ExceptionsPlugin::
          check_ieee754_underflow_exception_flag());
  CHECK(cpputest::extensions::IEEE754ExceptionsPlugin::
          check_ieee754_inexact_exception_flag());
  CHECK(cpputest::extensions::IEEE754ExceptionsPlugin::
          check_ieee754_div_by_zero_exception_flag());
  LONGS_EQUAL(1, fixture.get_check_count());
  LONGS_EQUAL(1, fixture.get_failure_count());
}

#endif

namespace {
cpputest::extensions::IEEE754ExceptionsPlugin ip;
}

TEST_GROUP(IEEE754ExceptionsPlugin2)
{
  void setup(void) override
  {
    cpputest::TestRegistry::get_current_registry()->install_plugin(&ip);
  }
};

IGNORE_TEST(IEEE754ExceptionsPlugin2, should_not_fail_in_ignored_test)
{
  set_everything_c();
}
