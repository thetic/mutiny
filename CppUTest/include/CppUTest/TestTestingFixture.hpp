#ifndef INCLUDED_CPPUTEST_TESTTESTINGFIXTURE_HPP
#define INCLUDED_CPPUTEST_TESTTESTINGFIXTURE_HPP

#include "CppUTest/StringBufferTestOutput.hpp"
#include "CppUTest/TestRegistry.hpp"
#include "CppUTest/TestShell.hpp"

namespace cpputest {

class ExecFunction;
class ExecFunctionTestShell;

class TestTestingFixture
{
public:
  TestTestingFixture();
  virtual ~TestTestingFixture();
  void flushOutputAndResetResult();

  void addTest(TestShell* test);
  void installPlugin(TestPlugin* plugin);

  void setTestFunction(void (*testFunction)());
  void setTestFunction(ExecFunction* testFunction);
  void setSetup(void (*setupFunction)());
  void setTeardown(void (*teardownFunction)());

  void setOutputVerbose();

  void runTestWithMethod(void (*method)());
  void runAllTests();

  size_t getFailureCount();
  size_t getCheckCount();
  size_t getIgnoreCount();
  size_t getRunCount();
  size_t getTestCount();
  const String& getOutput();
  TestRegistry* getRegistry();

  bool hasTestFailed();
  void assertPrintContains(const String& contains);
  void assertPrintContainsNot(const String& contains);
  void checkTestFailsWithProperTestLocation(const char* text,
      const char* file,
      size_t line);

  static void lineExecutedAfterCheck();

private:
  void clearExecFunction();

  static bool lineOfCodeExecutedAfterCheck;

  TestRegistry* registry_;
  ExecFunctionTestShell* genTest_;
  bool ownsExecFunction_;
  StringBufferTestOutput* output_;
  TestResult* result_;
};

} // namespace cpputest

#endif
