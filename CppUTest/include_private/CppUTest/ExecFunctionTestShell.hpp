#ifndef INCLUDED_CPPUTEST_EXECFUNCTIONTESTSHELL_HPP
#define INCLUDED_CPPUTEST_EXECFUNCTIONTESTSHELL_HPP

#include "CppUTest/Test.hpp"
#include "CppUTest/TestShell.hpp"

namespace cpputest {

class ExecFunctionTestShell;

class ExecFunctionTest : public Test
{
public:
  ExecFunctionTest(ExecFunctionTestShell* shell);
  void test_body() override;
  virtual void setup() override;
  virtual void teardown() override;

private:
  ExecFunctionTestShell* shell_;
};

//////////////////// ExecFunction

class ExecFunction
{
public:
  ExecFunction();
  virtual ~ExecFunction();

  virtual void exec() = 0;
};

class ExecFunctionWithoutParameters : public ExecFunction
{
public:
  void (*test_function)();

  ExecFunctionWithoutParameters(void (*test_function)());
  virtual ~ExecFunctionWithoutParameters() override;

  virtual void exec() override;
};

//////////////////// ExecFunctionTestShell

class ExecFunctionTestShell : public TestShell
{
public:
  void (*setup)();
  void (*teardown)();
  ExecFunction* test_function;

  ExecFunctionTestShell(void (*set)() = nullptr, void (*tear)() = nullptr)
    : TestShell("ExecFunction", "ExecFunction", "ExecFunction", 1)
    , setup(set)
    , teardown(tear)
    , test_function(nullptr)
  {
  }

  Test* create_test() override { return new ExecFunctionTest(this); }
  virtual ~ExecFunctionTestShell() override;
};

} // namespace cpputest

#endif
