#include "IEEE754ExceptionsPlugin.hpp"

#include "mutiny/test/CommandLineRunner.hpp"
#include "mutiny/test/Registry.hpp"

#include "mutiny/test.hpp"

#if MUTINY_HAVE_FENV

#include <fenv.h>
#include <math.h>

/*
 * To see a demonstration of tests failing as a result of
 * IEEE754ExceptionsPlugin picking up floating point errors, run the test
 * executable with the -ri option.
 *
 */

TEST_GROUP(FEDemo)
{
  void setup() override { IEEE754ExceptionsPlugin::disable_inexact(); }
};

EXPECT_FAIL_TEST(FEDemo, should_fail_when_FE_DIVBYZERO_is_set)
{
  float f = 1.0f;
  volatile float zero = 0.0f;
  CHECK((f /= zero) >= INFINITY);
}

EXPECT_FAIL_TEST(FEDemo, should_fail_when_FE_UNDERFLOW_is_set)
{
  volatile float f = 0.01f;
  while (f > 0.0f)
    f = f * f;
  CHECK(f == 0.0f);
}

EXPECT_FAIL_TEST(FEDemo, should_fail_when_FE_OVERFLOW_is_set)
{
  volatile float f = 1000.0f;
  while (f < INFINITY)
    f = f * f;
  CHECK(f >= INFINITY);
}

EXPECT_FAIL_TEST(FEDemo, should_fail_when_FE_INEXACT_is_set)
{
  IEEE754ExceptionsPlugin::enable_inexact();
  volatile float f = 10.0f;
  CHECK_APPROX(
      static_cast<double>(f / 3.0f),
      static_cast<double>(3.333f),
      static_cast<double>(0.001f)
  );
}

TEST(FEDemo, should_succeed_when_no_flags_are_set)
{
  CHECK(5.0f == 15.0f / 3.0f);
}

#endif
