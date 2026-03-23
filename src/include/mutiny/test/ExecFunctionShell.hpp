#ifndef INCLUDED_MUTINY_TEST_EXECFUNCTIONSHELL_HPP
#define INCLUDED_MUTINY_TEST_EXECFUNCTIONSHELL_HPP

#include "mutiny/test/Shell.hpp"
#include "mutiny/test/Test.hpp"

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

}
}
} // namespace mu::tiny::test

#endif
