#include "TeamCityOutputPlugin.hpp"

#include "CppMu/TestFailure.hpp"
#include "CppMu/TestResult.hpp"
#include "CppMu/TestShell.hpp"

TeamCityTestOutput::TeamCityTestOutput()
  : curr_group_()
{
}

void TeamCityTestOutput::print_current_test_started(
    const cppmu::TestShell& test
)
{
  print("##teamcity[testStarted name='");
  print_escaped(test.get_name().c_str());
  print("']\n");
  if (!test.will_run()) {
    print("##teamcity[testIgnored name='");
    print_escaped(test.get_name().c_str());
    print("']\n");
  }
  currtest_ = &test;
}

void TeamCityTestOutput::print_current_test_ended(const cppmu::TestResult& res)
{
  if (!currtest_)
    return;

  print("##teamcity[testFinished name='");
  print_escaped(currtest_->get_name().c_str());
  print("' duration='");
  print(res.get_current_test_total_execution_time());
  print("']\n");
}

void TeamCityTestOutput::print_current_group_started(
    const cppmu::TestShell& test
)
{
  curr_group_ = test.get_group();
  print("##teamcity[testSuiteStarted name='");
  print_escaped(curr_group_.c_str());
  print("']\n");
}

void TeamCityTestOutput::
    print_current_group_ended(const cppmu::TestResult& /*res*/)
{
  if (curr_group_ == "")
    return;

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

void TeamCityTestOutput::print_failure(const cppmu::TestFailure& failure)
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
  : cppmu::TestPlugin("TeamCityOutputPlugin")
{
}

bool TeamCityOutputPlugin::parse_arguments(
    int /*argc*/,
    const char* const* argv,
    int index
)
{
  cppmu::String arg = argv[index];
  if (arg.size() > 2 && cppmu::String(arg.c_str() + 2) == "teamcity") {
    active_ = true;
    return true;
  }
  return false;
}

cppmu::TestOutput* TeamCityOutputPlugin::create_output()
{
  if (active_)
    return new TeamCityTestOutput;
  return nullptr;
}
