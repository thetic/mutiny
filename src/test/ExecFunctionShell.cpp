#include "mu/tiny/test/ExecFunctionShell.hpp"

namespace mu {
namespace tiny {
namespace test {

//////////////////// ExecFunction

ExecFunctionWithoutParameters::ExecFunctionWithoutParameters(void (*fn)())
  : test_function(fn)
{
}

void ExecFunctionWithoutParameters::exec()
{
  if (test_function != nullptr) {
    test_function();
  }
}

//////////////////// ExecFunctionTest

ExecFunctionTest::ExecFunctionTest(ExecFunctionShell* shell)
  : shell_(shell)
{
}

void ExecFunctionTest::test_body()
{
  if (shell_->test_function != nullptr) {
    shell_->test_function->exec();
  }
}

void ExecFunctionTest::setup()
{
  if (shell_->setup != nullptr) {
    shell_->setup();
  }
}

void ExecFunctionTest::teardown()
{
  if (shell_->teardown != nullptr) {
    shell_->teardown();
  }
}

} // namespace test
} // namespace tiny
} // namespace mu
