#ifndef INCLUDED_TEAMCITYOUTPUTPLUGIN_HPP
#define INCLUDED_TEAMCITYOUTPUTPLUGIN_HPP

#include "mutiny/test/ConsoleOutput.hpp"
#include "mutiny/test/Plugin.hpp"

class TeamCityTestOutput : public mu::tiny::test::ConsoleTestOutput
{
public:
  TeamCityTestOutput();
  ~TeamCityTestOutput() override = default;

  void print_current_test_started(
      const mu::tiny::test::TestShell& test
  ) override;
  void print_current_test_ended(const mu::tiny::test::TestResult& res) override;
  void print_current_group_started(
      const mu::tiny::test::TestShell& test
  ) override;
  void print_current_group_ended(
      const mu::tiny::test::TestResult& res
  ) override;

  void print_failure(const mu::tiny::test::TestFailure& failure) override;

private:
  void print_escaped(const char* s);
  const mu::tiny::test::TestShell* currtest_{ nullptr };
  mu::tiny::test::String curr_group_;
};

class TeamCityOutputPlugin : public mu::tiny::test::TestPlugin
{
public:
  TeamCityOutputPlugin();

  bool parse_arguments(int argc, const char* const* argv, int index) override;
  mu::tiny::test::String get_help() const override;
  mu::tiny::test::TestOutput* create_output() override;

private:
  bool active_{ false };
};

#endif
