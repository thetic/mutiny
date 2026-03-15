#ifndef INCLUDED_IEEE754EXCEPTIONSPLUGIN_HPP
#define INCLUDED_IEEE754EXCEPTIONSPLUGIN_HPP

#ifndef CPPMU_HAVE_FENV
#define CPPMU_HAVE_FENV 0
#endif

#include "CppMu/TestPlugin.hpp"

class IEEE754ExceptionsPlugin : public cppmu::TestPlugin
{
public:
  IEEE754ExceptionsPlugin(
      const cppmu::String& name = "IEEE754ExceptionsPlugin");

  virtual void pre_test_action(cppmu::TestShell& test,
      cppmu::TestResult& result) override;
  virtual void post_test_action(cppmu::TestShell& test,
      cppmu::TestResult& result) override;

  static void disable_inexact(void);
  static void enable_inexact(void);
  static bool check_ieee754_overflow_exception_flag();
  static bool check_ieee754_underflow_exception_flag();
  static bool check_ieee754_inexact_exception_flag();
  static bool check_ieee754_div_by_zero_exception_flag();

private:
  void ieee754_check(cppmu::TestShell& test,
      cppmu::TestResult& result,
      int flag,
      const char* text);
  static bool inexact_disabled_;
};

#endif
