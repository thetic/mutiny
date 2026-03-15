#ifndef INCLUDED_CPPUTEST_COMPOSITETESTOUTPUT_HPP
#define INCLUDED_CPPUTEST_COMPOSITETESTOUTPUT_HPP

#include "CppUTest/TestOutput.hpp"

namespace cpputest {

class CompositeTestOutput : public TestOutput
{
public:
  virtual void setOutputOne(TestOutput* output);
  virtual void setOutputTwo(TestOutput* output);

  CompositeTestOutput();
  virtual ~CompositeTestOutput() override;

  virtual void printTestsStarted() override;
  virtual void printTestsEnded(const TestResult& result) override;

  virtual void printCurrentTestStarted(const TestShell& test) override;
  virtual void printCurrentTestEnded(const TestResult& res) override;
  virtual void printCurrentGroupStarted(const TestShell& test) override;
  virtual void printCurrentGroupEnded(const TestResult& res) override;

  virtual void verbose(VerbosityLevel level) override;
  virtual void color() override;
  virtual void printBuffer(const char*) override;
  virtual void print(const char*) override;
  virtual void print(long) override;
  virtual void print(size_t) override;
  virtual void printDouble(double) override;
  virtual void printFailure(const TestFailure& failure) override;
  virtual void setProgressIndicator(const char*) override;

  virtual void printVeryVerbose(const char*) override;

  virtual void flush() override;

protected:
  CompositeTestOutput(const TestOutput&);
  CompositeTestOutput& operator=(const TestOutput&);

private:
  TestOutput* outputOne_;
  TestOutput* outputTwo_;
};

} // namespace cpputest

#endif
