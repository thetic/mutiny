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

  virtual void print_current_test_started(const TestShell& test) override;
  virtual void print_current_test_ended(const TestResult& res) override;
  virtual void print_current_group_started(const TestShell& test) override;
  virtual void print_current_group_ended(const TestResult& res) override;

  virtual void print_failure(const TestFailure& failure) override;

protected:
private:
  void print_escaped(const char* s);
  const TestShell* currtest_;
  String curr_group_;
};

} // namespace cpputest

#endif
