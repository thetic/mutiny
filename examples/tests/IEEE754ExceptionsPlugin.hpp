#ifndef INCLUDED_IEEE754EXCEPTIONSPLUGIN_HPP
#define INCLUDED_IEEE754EXCEPTIONSPLUGIN_HPP

#ifndef MUTINY_HAVE_FENV
#define MUTINY_HAVE_FENV 0
#endif

#include "mu/tiny/test/Plugin.hpp"

class IEEE754ExceptionsPlugin : public mu::tiny::test::Plugin
{
public:
  IEEE754ExceptionsPlugin(
      const mu::tiny::String& name = "IEEE754ExceptionsPlugin"
  );

  void pre_test_action(
      mu::tiny::test::Shell& test,
      mu::tiny::test::Result& result
  ) override;
  void post_test_action(
      mu::tiny::test::Shell& test,
      mu::tiny::test::Result& result
  ) override;

  static void disable_inexact(void);
  static void enable_inexact(void);
  static bool check_ieee754_overflow_exception_flag();
  static bool check_ieee754_underflow_exception_flag();
  static bool check_ieee754_inexact_exception_flag();
  static bool check_ieee754_div_by_zero_exception_flag();

private:
  static void ieee754_check(
      mu::tiny::test::Shell& test,
      mu::tiny::test::Result& result,
      int flag,
      const char* text
  );
  static bool inexact_disabled_;
};

#endif
