#ifndef INCLUDED_CPPUTEST_TEAMCITYTESTOUTPUT_HPP
#define INCLUDED_CPPUTEST_TEAMCITYTESTOUTPUT_HPP

#include "CppUTest/ConsoleTestOutput.hpp"
#include "CppUTest/String.hpp"

namespace cpputest {

class TeamCityTestOutput : public ConsoleTestOutput
{
public:
  TeamCityTestOutput(void);
  virtual ~TeamCityTestOutput(void) override;

  virtual void printCurrentTestStarted(const TestShell& test) override;
  virtual void printCurrentTestEnded(const TestResult& res) override;
  virtual void printCurrentGroupStarted(const TestShell& test) override;
  virtual void printCurrentGroupEnded(const TestResult& res) override;

  virtual void printFailure(const TestFailure& failure) override;

protected:
private:
  void printEscaped(const char* s);
  const TestShell* currtest_;
  String currGroup_;
};

} // namespace cpputest

#endif
