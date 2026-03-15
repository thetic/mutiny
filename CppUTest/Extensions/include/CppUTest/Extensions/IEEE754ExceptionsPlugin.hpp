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

  virtual void preTestAction(cpputest::TestShell& test,
      cpputest::TestResult& result) override;
  virtual void postTestAction(cpputest::TestShell& test,
      cpputest::TestResult& result) override;

  static void disableInexact(void);
  static void enableInexact(void);
  static bool checkIeee754OverflowExceptionFlag();
  static bool checkIeee754UnderflowExceptionFlag();
  static bool checkIeee754InexactExceptionFlag();
  static bool checkIeee754DivByZeroExceptionFlag();

private:
  void ieee754Check(cpputest::TestShell& test,
      cpputest::TestResult& result,
      int flag,
      const char* text);
  static bool inexactDisabled_;
};

} // namespace extensions
} // namespace cpputest

#endif
