#ifndef INCLUDED_TEAMCITYOUTPUTPLUGIN_HPP
#define INCLUDED_TEAMCITYOUTPUTPLUGIN_HPP

#include "mu/tiny/test/ConsoleOutput.hpp"
#include "mu/tiny/test/Plugin.hpp"

class TeamCityTestOutput : public mu::tiny::test::ConsoleOutput
{
public:
  TeamCityTestOutput() = default;
  ~TeamCityTestOutput() override = default;

  void print_current_test_started(const mu::tiny::test::Shell& test) override;
  void print_current_test_ended(const mu::tiny::test::Result& res) override;
  void print_current_group_started(const mu::tiny::test::Shell& test) override;
  void print_current_group_ended(const mu::tiny::test::Result& res) override;

  void print_failure(const mu::tiny::test::Failure& failure) override;

private:
  void print_escaped(const char* s);
  const mu::tiny::test::Shell* currtest_{ nullptr };
  mu::tiny::String curr_group_;
};

class TeamCityOutputPlugin : public mu::tiny::test::Plugin
{
public:
  TeamCityOutputPlugin();

  bool parse_arguments(int argc, const char* const* argv, int index) override;
  mu::tiny::String get_help() const override;
  mu::tiny::test::Output* create_output() override;

private:
  bool active_{ false };
};

#endif
