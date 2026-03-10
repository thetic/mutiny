#include "CppUTest/ExecFunctionTestShell.hpp"

namespace cpputest {

ExecFunctionTestShell::~ExecFunctionTestShell() {}

//////////////////// ExecFunction

ExecFunction::ExecFunction() {}

ExecFunction::~ExecFunction() {}

ExecFunctionWithoutParameters::ExecFunctionWithoutParameters(
    void (*testFunction)())
  : testFunction_(testFunction)
{
}

ExecFunctionWithoutParameters::~ExecFunctionWithoutParameters() {}

void
ExecFunctionWithoutParameters::exec()
{
  if (testFunction_)
    testFunction_();
}

//////////////////// ExecFunctionTest

ExecFunctionTest::ExecFunctionTest(ExecFunctionTestShell* shell)
  : shell_(shell)
{
}

void
ExecFunctionTest::testBody()
{
  if (shell_->testFunction_)
    shell_->testFunction_->exec();
}

void
ExecFunctionTest::setup()
{
  if (shell_->setup_)
    shell_->setup_();
}

void
ExecFunctionTest::teardown()
{
  if (shell_->teardown_)
    shell_->teardown_();
}

} // namespace cpputest
