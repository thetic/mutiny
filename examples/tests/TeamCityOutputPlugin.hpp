#ifndef INCLUDED_TEAMCITYOUTPUTPLUGIN_HPP
#define INCLUDED_TEAMCITYOUTPUTPLUGIN_HPP

#include "CppMu/ConsoleTestOutput.hpp"
#include "CppMu/TestPlugin.hpp"

namespace cppmu {
class TestFailure;
class TestResult;
class TestShell;
} // namespace cppmu

class TeamCityTestOutput : public cppmu::ConsoleTestOutput
{
public:
  TeamCityTestOutput();
  ~TeamCityTestOutput() override = default;

  void print_current_test_started(const cppmu::TestShell& test) override;
  void print_current_test_ended(const cppmu::TestResult& res) override;
  void print_current_group_started(const cppmu::TestShell& test) override;
  void print_current_group_ended(const cppmu::TestResult& res) override;

  void print_failure(const cppmu::TestFailure& failure) override;

private:
  void print_escaped(const char* s);
  const cppmu::TestShell* currtest_{ nullptr };
  cppmu::String curr_group_;
};

class TeamCityOutputPlugin : public cppmu::TestPlugin
{
public:
  TeamCityOutputPlugin();

  bool parse_arguments(int argc, const char* const* argv, int index) override;
  cppmu::TestOutput* create_output() override;

private:
  bool active_{ false };
};

#endif
