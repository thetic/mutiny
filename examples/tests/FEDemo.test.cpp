#include "IEEE754ExceptionsPlugin.hpp"

#include "mu/tiny/test.hpp"

#if MUTINY_HAVE_FENV

#include <math.h>

/*
 * To see a demonstration of tests failing as a result of
 * IEEE754ExceptionsPlugin picking up floating point errors, run the test
 * executable with the -rs option.
 *
 */

TEST_GROUP(FEDemo)
{
  void setup() override { IEEE754ExceptionsPlugin::disable_inexact(); }
};

XFAIL_TEST(FEDemo, should_fail_when_FE_DIVBYZERO_is_set)
{
  float f = 1.0F;
  volatile float zero = 0.0F;
  CHECK((f /= zero) >= INFINITY);
}

XFAIL_TEST(FEDemo, should_fail_when_FE_UNDERFLOW_is_set)
{
  volatile float f = 0.01F;
  while (f > 0.0F) {
    f = f * f;
  }
  CHECK(f == 0.0F);
}

XFAIL_TEST(FEDemo, should_fail_when_FE_OVERFLOW_is_set)
{
  volatile float f = 1000.0F;
  while (f < INFINITY) {
    f = f * f;
  }
  CHECK(f >= INFINITY);
}

XFAIL_TEST(FEDemo, should_fail_when_FE_INEXACT_is_set)
{
  IEEE754ExceptionsPlugin::enable_inexact();
  volatile float f = 10.0F;
  CHECK_APPROX(f / 3.0F, 3.333F, 0.001F);
}

TEST(FEDemo, should_succeed_when_no_flags_are_set)
{
  CHECK(5.0F == 15.0F / 3.0F);
}

#endif
