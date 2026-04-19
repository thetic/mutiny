#include "TeamCityOutputPlugin.hpp"

#include "mu/tiny/test/Failure.hpp"
#include "mu/tiny/test/Result.hpp"
#include "mu/tiny/test/Shell.hpp"

TeamCityTestOutput::TeamCityTestOutput()
  : curr_group_()
{
}

void TeamCityTestOutput::print_current_test_started(
    const mu::tiny::test::Shell& test
)
{
  print("##teamcity[testStarted name='");
  print_escaped(test.get_name());
  print("']\n");
  if (!test.will_run()) {
    print("##teamcity[testIgnored name='");
    print_escaped(test.get_name());
    print("']\n");
  }
  currtest_ = &test;
}

void TeamCityTestOutput::print_current_test_ended(
    const mu::tiny::test::Result& res
)
{
  if (!currtest_) {
    return;
  }

  print("##teamcity[testFinished name='");
  print_escaped(currtest_->get_name());
  print("' duration='");
  print(
      mu::tiny::string_from(res.get_current_test_total_execution_time()).c_str()
  );
  print("']\n");
}

void TeamCityTestOutput::print_current_group_started(
    const mu::tiny::test::Shell& test
)
{
  curr_group_ = test.get_group();
  print("##teamcity[testSuiteStarted name='");
  print_escaped(curr_group_.c_str());
  print("']\n");
}

void TeamCityTestOutput::
    print_current_group_ended(const mu::tiny::test::Result& /*res*/)
{
  if (curr_group_ == "") {
    return;
  }

  print("##teamcity[testSuiteFinished name='");
  print_escaped(curr_group_.c_str());
  print("']\n");
}

void TeamCityTestOutput::print_escaped(const char* s)
{
  while (*s) {
    char str[3];
    if ((*s == '\'') || (*s == '|') || (*s == '[') || (*s == ']')) {
      str[0] = '|';
      str[1] = *s;
      str[2] = 0;
    } else if (*s == '\r') {
      str[0] = '|';
      str[1] = 'r';
      str[2] = 0;
    } else if (*s == '\n') {
      str[0] = '|';
      str[1] = 'n';
      str[2] = 0;
    } else {
      str[0] = *s;
      str[1] = 0;
    }
    print_buffer(str);
    s++;
  }
}

void TeamCityTestOutput::print_failure(const mu::tiny::test::Failure& failure)
{
  print("##teamcity[testFailed name='");
  print_escaped(failure.get_test_name_only().c_str());
  print("' message='");
  if (failure.is_outside_test_file() || failure.is_in_helper_function()) {
    print("TEST failed (");
    print(failure.get_test_file_name().c_str());
    print(":");
    print(failure.get_test_line_number());
    print("): ");
  }

  print_escaped(failure.get_file_name().c_str());
  print(":");
  print(failure.get_failure_line_number());

  print("' details='");
  print_escaped(failure.get_message().c_str());
  print("']\n");
}

TeamCityOutputPlugin::TeamCityOutputPlugin()
  : mu::tiny::test::Plugin("TeamCityOutputPlugin")
{
}

bool TeamCityOutputPlugin::parse_arguments(
    int /*argc*/,
    const char* const* argv,
    int index
)
{
  mu::tiny::String arg = argv[index];
  if (arg.size() > 2 && mu::tiny::String(arg.c_str() + 2) == "teamcity") {
    active_ = true;
    return true;
  }
  return false;
}

mu::tiny::String TeamCityOutputPlugin::get_help() const
{
  return "  -pteamcity        - output TeamCity output\n";
}

mu::tiny::test::Output* TeamCityOutputPlugin::create_output()
{
  if (active_) {
    return new TeamCityTestOutput;
  }
  return nullptr;
}
