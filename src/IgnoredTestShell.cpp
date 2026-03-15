#include "CppMu/IgnoredTestShell.hpp"

#include "CppMu/TestPlugin.hpp"
#include "CppMu/TestResult.hpp"

namespace cppmu {

IgnoredTestShell::IgnoredTestShell()
  : run_ignored_(false)
{
}

IgnoredTestShell::IgnoredTestShell(const char* group_name,
    const char* test_name,
    const char* file_name,
    size_t line_number)
  : TestShell(group_name, test_name, file_name, line_number)
  , run_ignored_(false)
{
}

IgnoredTestShell::~IgnoredTestShell() {}

bool
IgnoredTestShell::will_run() const
{
  if (run_ignored_)
    return TestShell::will_run();

  return false;
}

String
IgnoredTestShell::get_macro_name() const
{
  if (run_ignored_)
    return "TEST";

  return "IGNORE_TEST";
}

void
IgnoredTestShell::run_one_test(TestPlugin* plugin, TestResult& result)
{
  if (run_ignored_) {
    TestShell::run_one_test(plugin, result);
    return;
  }

  result.count_ignored();
}

void
IgnoredTestShell::set_run_ignored()
{
  run_ignored_ = true;
}

} // namespace cppmu
