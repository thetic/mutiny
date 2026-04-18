#ifndef INCLUDED_MU_TINY_TEST_EXECFUNCTIONSHELL_HPP
#define INCLUDED_MU_TINY_TEST_EXECFUNCTIONSHELL_HPP

#include "mu/tiny/test/ExecFunction.hpp"
#include "mu/tiny/test/Shell.hpp"
#include "mu/tiny/test/Test.hpp"

namespace mu {
namespace tiny {
namespace test {

class ExecFunctionShell;

class ExecFunctionTest : public Test
{
public:
  ExecFunctionTest(ExecFunctionShell* shell);
  void test_body() override;
  void setup() override;
  void teardown() override;

private:
  ExecFunctionShell* shell_;
};

class ExecFunctionWithoutParameters : public ExecFunction
{
public:
  void (*test_function)();

  ExecFunctionWithoutParameters(void (*test_function)());
  ~ExecFunctionWithoutParameters() override = default;

  void exec() override;
};

//////////////////// ExecFunctionShell

class ExecFunctionShell : public Shell
{
public:
  void (*setup)();
  void (*teardown)();
  ExecFunction* test_function{ nullptr };

  ExecFunctionShell(void (*set)() = nullptr, void (*tear)() = nullptr)
    : Shell("ExecFunction", "ExecFunction", "ExecFunction", 1)
    , setup(set)
    , teardown(tear)

  {
  }

  Test* create_test() override { return new ExecFunctionTest(this); }
  ~ExecFunctionShell() override = default;
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
