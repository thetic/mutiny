#include "IEEE754ExceptionsPlugin.hpp"

#include "CppMu/CommandLineTestRunner.hpp"
#include "CppMu/CppMu.hpp"
#include "CppMu/TestRegistry.hpp"

#if CPPMU_HAVE_FENV

#include <limits>

#include <fenv.h>

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

IGNORE_TEST(FEDemo, should_fail_when_FE_DIVBYZERO_is_set)
{
  float f = 1.0f;
  CHECK((f /= 0.0f) >= std::numeric_limits<float>::infinity());
}

IGNORE_TEST(FEDemo, should_fail_when_FE_UNDERFLOW_is_set)
{
  volatile float f = 0.01f;
  while (f > 0.0f)
    f = f * f;
  CHECK(f == 0.0f);
}

IGNORE_TEST(FEDemo, should_fail_when_FE_OVERFLOW_is_set)
{
  volatile float f = 1000.0f;
  while (f < std::numeric_limits<float>::infinity())
    f = f * f;
  CHECK(f >= std::numeric_limits<float>::infinity());
}

IGNORE_TEST(FEDemo, should_fail_when_FE_INEXACT_is_set)
{
  IEEE754ExceptionsPlugin::enable_inexact();
  float f = 10.0f;
  DOUBLES_EQUAL(static_cast<double>(f / 3.0f),
      static_cast<double>(3.333f),
      static_cast<double>(0.001f));
}

TEST(FEDemo, should_succeed_when_no_flags_are_set)
{
  CHECK(5.0f == 15.0f / 3.0f);
}

#endif
