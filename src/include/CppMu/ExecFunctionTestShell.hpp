#ifndef INCLUDED_CPPMU_EXECFUNCTIONTESTSHELL_HPP
#define INCLUDED_CPPMU_EXECFUNCTIONTESTSHELL_HPP

#include "CppMu/Test.hpp"
#include "CppMu/TestShell.hpp"

namespace cppmu {

class ExecFunctionTestShell;

class ExecFunctionTest : public Test
{
public:
  ExecFunctionTest(ExecFunctionTestShell* shell);
  void test_body() override;
  void setup() override;
  void teardown() override;

private:
  ExecFunctionTestShell* shell_;
};

//////////////////// ExecFunction

class ExecFunction
{
public:
  ExecFunction() = default;
  virtual ~ExecFunction() = default;

  virtual void exec() = 0;
};

class ExecFunctionWithoutParameters : public ExecFunction
{
public:
  void (*test_function)();

  ExecFunctionWithoutParameters(void (*test_function)());
  ~ExecFunctionWithoutParameters() override = default;

  void exec() override;
};

//////////////////// ExecFunctionTestShell

class ExecFunctionTestShell : public TestShell
{
public:
  void (*setup)();
  void (*teardown)();
  ExecFunction* test_function{ nullptr };

  ExecFunctionTestShell(void (*set)() = nullptr, void (*tear)() = nullptr)
    : TestShell("ExecFunction", "ExecFunction", "ExecFunction", 1)
    , setup(set)
    , teardown(tear)

  {
  }

  Test* create_test() override { return new ExecFunctionTest(this); }
  ~ExecFunctionTestShell() override = default;
};

} // namespace cppmu

#endif
