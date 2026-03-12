#include "CppUTest/Extensions/IEEE754ExceptionsPlugin.hpp"

#include "CppUTest/TestHarness.hpp"

#if CPPUTEST_HAVE_FENV
#include <fenv.h>
#endif

namespace cpputest {
namespace extensions {

#define IEEE754_CHECK_CLEAR(test, result, flag)                                \
  ieee754Check(test, result, flag, #flag)

bool IEEE754ExceptionsPlugin::inexactDisabled_ = true;

IEEE754ExceptionsPlugin::IEEE754ExceptionsPlugin(const String& name)
  : TestPlugin(name)
{
}

void
IEEE754ExceptionsPlugin::preTestAction(cpputest::TestShell&, TestResult&)
{
#if CPPUTEST_HAVE_FENV
  CHECK(!feclearexcept(FE_ALL_EXCEPT));
#endif
}

void
IEEE754ExceptionsPlugin::postTestAction(cpputest::TestShell& test,
    TestResult& result)
{
#if CPPUTEST_HAVE_FENV
  if (!test.hasFailed()) {
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

void
IEEE754ExceptionsPlugin::disableInexact()
{
  inexactDisabled_ = true;
}

void
IEEE754ExceptionsPlugin::enableInexact()
{
  inexactDisabled_ = false;
}

bool
IEEE754ExceptionsPlugin::checkIeee754OverflowExceptionFlag()
{
#if CPPUTEST_HAVE_FENV
  return fetestexcept(FE_OVERFLOW) != 0;
#else
  return false;
#endif
}

bool
IEEE754ExceptionsPlugin::checkIeee754UnderflowExceptionFlag()
{
#if CPPUTEST_HAVE_FENV
  return fetestexcept(FE_UNDERFLOW) != 0;
#else
  return false;
#endif
}

bool
IEEE754ExceptionsPlugin::checkIeee754InexactExceptionFlag()
{
#if CPPUTEST_HAVE_FENV
  return fetestexcept(FE_INEXACT) != 0;
#else
  return false;
#endif
}

bool
IEEE754ExceptionsPlugin::checkIeee754DivByZeroExceptionFlag()
{
#if CPPUTEST_HAVE_FENV
  return fetestexcept(FE_DIVBYZERO) != 0;
#else
  return false;
#endif
}

void
IEEE754ExceptionsPlugin::ieee754Check(cpputest::TestShell& test,
    TestResult& result,
    int flag,
    const char* text)
{
#if CPPUTEST_HAVE_FENV
  result.countCheck();
  if (inexactDisabled_)
    CHECK(!feclearexcept(FE_INEXACT));
  if (fetestexcept(flag)) {
    CHECK(!feclearexcept(FE_ALL_EXCEPT));
    CheckFailure failure(
        &test, __FILE__, __LINE__, "IEEE754_CHECK_CLEAR", text);
    result.addFailure(failure);
  }
#else
  static_cast<void>(test);
  static_cast<void>(result);
  static_cast<void>(flag);
  static_cast<void>(text);
#endif
}

} // namespace extensions
} // namespace cpputest
