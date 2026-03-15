#ifndef INCLUDED_CPPUTEST_EXTENSIONS_IEEE754EXCEPTIONSPLUGIN_HPP
#define INCLUDED_CPPUTEST_EXTENSIONS_IEEE754EXCEPTIONSPLUGIN_HPP

#ifndef CPPUTEST_HAVE_FENV
#define CPPUTEST_HAVE_FENV 0
#endif

#include "CppUTest/TestPlugin.hpp"

namespace cpputest {
namespace extensions {

class IEEE754ExceptionsPlugin : public cpputest::TestPlugin
{
public:
  IEEE754ExceptionsPlugin(
      const cpputest::String& name = "IEEE754ExceptionsPlugin");

  virtual void pre_test_action(cpputest::TestShell& test,
      cpputest::TestResult& result) override;
  virtual void post_test_action(cpputest::TestShell& test,
      cpputest::TestResult& result) override;

  static void disable_inexact(void);
  static void enable_inexact(void);
  static bool check_ieee754_overflow_exception_flag();
  static bool check_ieee754_underflow_exception_flag();
  static bool check_ieee754_inexact_exception_flag();
  static bool check_ieee754_div_by_zero_exception_flag();

private:
  void ieee754_check(cpputest::TestShell& test,
      cpputest::TestResult& result,
      int flag,
      const char* text);
  static bool inexact_disabled_;
};

} // namespace extensions
} // namespace cpputest

#endif
