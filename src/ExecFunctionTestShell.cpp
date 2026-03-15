#include "CppUTest/ExecFunctionTestShell.hpp"

namespace cpputest {

ExecFunctionTestShell::~ExecFunctionTestShell() {}

//////////////////// ExecFunction

ExecFunction::ExecFunction() {}

ExecFunction::~ExecFunction() {}

ExecFunctionWithoutParameters::ExecFunctionWithoutParameters(void (*fn)())
  : test_function(fn)
{
}

ExecFunctionWithoutParameters::~ExecFunctionWithoutParameters() {}

void
ExecFunctionWithoutParameters::exec()
{
  if (test_function)
    test_function();
}

//////////////////// ExecFunctionTest

ExecFunctionTest::ExecFunctionTest(ExecFunctionTestShell* shell)
  : shell_(shell)
{
}

void
ExecFunctionTest::test_body()
{
  if (shell_->test_function)
    shell_->test_function->exec();
}

void
ExecFunctionTest::setup()
{
  if (shell_->setup)
    shell_->setup();
}

void
ExecFunctionTest::teardown()
{
  if (shell_->teardown)
    shell_->teardown();
}

} // namespace cpputest
