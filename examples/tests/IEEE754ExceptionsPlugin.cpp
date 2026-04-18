#include "IEEE754ExceptionsPlugin.hpp"

#include "mu/tiny/test.hpp"

#if MUTINY_HAVE_FENV
#include <fenv.h>
#endif

#define IEEE754_CHECK_CLEAR(test, result, flag)                                \
  ieee754_check(test, result, flag, #flag)

bool IEEE754ExceptionsPlugin::inexact_disabled_ = true;

IEEE754ExceptionsPlugin::IEEE754ExceptionsPlugin(const mu::tiny::String& name)
  : Plugin(name)
{
}

void IEEE754ExceptionsPlugin::pre_test_action(
    mu::tiny::test::Shell&,
    mu::tiny::test::Result&
)
{
#if MUTINY_HAVE_FENV
  CHECK(!feclearexcept(FE_ALL_EXCEPT));
#endif
}

void IEEE754ExceptionsPlugin::post_test_action(
    mu::tiny::test::Shell& test,
    mu::tiny::test::Result& result
)
{
#if MUTINY_HAVE_FENV
  if (!test.has_failed()) {
    IEEE754_CHECK_CLEAR(test, result, FE_DIVBYZERO);
    IEEE754_CHECK_CLEAR(test, result, FE_OVERFLOW);
    IEEE754_CHECK_CLEAR(test, result, FE_UNDERFLOW);
    IEEE754_CHECK_CLEAR(test, result, FE_INVALID);
    IEEE754_CHECK_CLEAR(test, result, FE_INEXACT);
  }
#else
  static_cast<void>(test);
  static_cast<void>(result);
#endif
}

void IEEE754ExceptionsPlugin::disable_inexact()
{
  inexact_disabled_ = true;
}

void IEEE754ExceptionsPlugin::enable_inexact()
{
  inexact_disabled_ = false;
}

bool IEEE754ExceptionsPlugin::check_ieee754_overflow_exception_flag()
{
#if MUTINY_HAVE_FENV
  return fetestexcept(FE_OVERFLOW) != 0;
#else
  return false;
#endif
}

bool IEEE754ExceptionsPlugin::check_ieee754_underflow_exception_flag()
{
#if MUTINY_HAVE_FENV
  return fetestexcept(FE_UNDERFLOW) != 0;
#else
  return false;
#endif
}

bool IEEE754ExceptionsPlugin::check_ieee754_inexact_exception_flag()
{
#if MUTINY_HAVE_FENV
  return fetestexcept(FE_INEXACT) != 0;
#else
  return false;
#endif
}

bool IEEE754ExceptionsPlugin::check_ieee754_div_by_zero_exception_flag()
{
#if MUTINY_HAVE_FENV
  return fetestexcept(FE_DIVBYZERO) != 0;
#else
  return false;
#endif
}

void IEEE754ExceptionsPlugin::ieee754_check(
    mu::tiny::test::Shell& test,
    mu::tiny::test::Result& result,
    int flag,
    const char* text
)
{
#if MUTINY_HAVE_FENV
  result.count_check();
  if (inexact_disabled_)
    CHECK(!feclearexcept(FE_INEXACT));
  if (fetestexcept(flag)) {
    CHECK(!feclearexcept(FE_ALL_EXCEPT));
    mu::tiny::test::CheckFailure failure(
        &test, __FILE__, __LINE__, "IEEE754_CHECK_CLEAR", text
    );
    result.add_failure(failure);
  }
#else
  static_cast<void>(test);
  static_cast<void>(result);
  static_cast<void>(flag);
  static_cast<void>(text);
#endif
}
