#include "CppUTest/Extensions/IEEE754ExceptionsPlugin.hpp"

#include "IEEE754PluginTest_c.h"

#include "CppUTest/CommandLineTestRunner.hpp"
#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestRegistry.hpp"
#include "CppUTest/TestTestingFixture.hpp"

TEST_GROUP(IEEE754ExceptionsPlugin)
{
  cpputest::TestTestingFixture fixture;
  cpputest::extensions::IEEE754ExceptionsPlugin ieee754Plugin;

  void setup(void) override { fixture.installPlugin(&ieee754Plugin); }
};

#if CPPUTEST_HAVE_FENV

TEST(IEEE754ExceptionsPlugin, should_fail_when_FE_DIVBYZERO_is_set)
{
  fixture.setTestFunction(set_divisionbyzero_c);
  fixture.runAllTests();
  fixture.assertPrintContains("IEEE754_CHECK_CLEAR(FE_DIVBYZERO) failed");
}

TEST(IEEE754ExceptionsPlugin, should_fail_when_FE_OVERFLOW_is_set)
{
  fixture.setTestFunction(set_overflow_c);
  fixture.runAllTests();
  fixture.assertPrintContains("IEEE754_CHECK_CLEAR(FE_OVERFLOW) failed");
}

TEST(IEEE754ExceptionsPlugin, should_fail_when_FE_UNDERFLOW_is_set)
{
  fixture.setTestFunction(set_underflow_c);
  fixture.runAllTests();
  fixture.assertPrintContains("IEEE754_CHECK_CLEAR(FE_UNDERFLOW) failed");
}

TEST(IEEE754ExceptionsPlugin, should_fail_when_FE_INEXACT_is_set_and_enabled)
{
  cpputest::extensions::IEEE754ExceptionsPlugin::enableInexact();
  fixture.setTestFunction(set_inexact_c);
  fixture.runAllTests();
  fixture.assertPrintContains("IEEE754_CHECK_CLEAR(FE_INEXACT) failed");
}

TEST(IEEE754ExceptionsPlugin,
    should_succeed_when_FE_INEXACT_is_set_and_disabled)
{
  cpputest::extensions::IEEE754ExceptionsPlugin::enableInexact();
  cpputest::extensions::IEEE754ExceptionsPlugin::disableInexact();
  fixture.setTestFunction(set_inexact_c);
  fixture.runAllTests();
  fixture.assertPrintContains("OK");
}

TEST(IEEE754ExceptionsPlugin,
    should_succeed_with_5_checks_when_no_flags_are_set)
{
  cpputest::extensions::IEEE754ExceptionsPlugin::enableInexact();
  fixture.setTestFunction(set_nothing_c);
  fixture.runAllTests();
  fixture.assertPrintContains(
      "OK (1 tests, 1 ran, 5 checks, 0 ignored, 0 filtered out");
  cpputest::extensions::IEEE754ExceptionsPlugin::disableInexact();
}

TEST(IEEE754ExceptionsPlugin, should_check_five_times_when_all_flags_are_set)
{
  fixture.setTestFunction(set_everything_c);
  fixture.runAllTests();
  LONGS_EQUAL(5, fixture.getCheckCount());
}

TEST(IEEE754ExceptionsPlugin, should_fail_only_once_when_all_flags_are_set)
{
  fixture.setTestFunction(set_everything_c);
  fixture.runAllTests();
  LONGS_EQUAL(1, fixture.getFailureCount());
}

static void
set_everything_but_already_failed(void)
{
  set_everything_c();
  CHECK(1 == 2);
}

TEST(IEEE754ExceptionsPlugin,
    should_not_fail_again_when_test_has_already_failed)
{
  fixture.setTestFunction(set_everything_but_already_failed);
  fixture.runAllTests();
  CHECK(cpputest::extensions::IEEE754ExceptionsPlugin::
          checkIeee754OverflowExceptionFlag());
  CHECK(cpputest::extensions::IEEE754ExceptionsPlugin::
          checkIeee754UnderflowExceptionFlag());
  CHECK(cpputest::extensions::IEEE754ExceptionsPlugin::
          checkIeee754InexactExceptionFlag());
  CHECK(cpputest::extensions::IEEE754ExceptionsPlugin::
          checkIeee754DivByZeroExceptionFlag());
  LONGS_EQUAL(1, fixture.getCheckCount());
  LONGS_EQUAL(1, fixture.getFailureCount());
}

#endif

static cpputest::extensions::IEEE754ExceptionsPlugin ip;

TEST_GROUP(IEEE754ExceptionsPlugin2)
{
  void setup(void) override
  {
    cpputest::TestRegistry::getCurrentRegistry()->installPlugin(&ip);
  }
};

IGNORE_TEST(IEEE754ExceptionsPlugin2, should_not_fail_in_ignored_test)
{
  set_everything_c();
}
