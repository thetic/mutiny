#ifndef INCLUDED_CPPMU_TEAMCITYTESTOUTPUT_HPP
#define INCLUDED_CPPMU_TEAMCITYTESTOUTPUT_HPP

#include "CppMu/ConsoleTestOutput.hpp"
#include "CppMu/String.hpp"

namespace cppmu {

class TeamCityTestOutput : public ConsoleTestOutput
{
public:
  TeamCityTestOutput(void);
  ~TeamCityTestOutput(void) override;

  void print_current_test_started(const TestShell& test) override;
  void print_current_test_ended(const TestResult& res) override;
  void print_current_group_started(const TestShell& test) override;
  void print_current_group_ended(const TestResult& res) override;

  void print_failure(const TestFailure& failure) override;

protected:
private:
  void print_escaped(const char* s);
  const TestShell* currtest_{ nullptr };
  String curr_group_;
};

} // namespace cppmu

#endif
