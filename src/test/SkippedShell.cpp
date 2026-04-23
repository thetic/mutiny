#include "mu/tiny/test/SkippedShell.hpp"

#include "mu/tiny/test/Plugin.hpp"
#include "mu/tiny/test/Result.hpp"

namespace mu {
namespace tiny {
namespace test {

SkippedShell::SkippedShell() noexcept
  : run_ignored_(false)
{
}

SkippedShell::SkippedShell(
    const char* group_name,
    const char* test_name,
    const char* file_name,
    int line_number
) noexcept
  : Shell(group_name, test_name, file_name, line_number)
  , run_ignored_(false)
{
}

bool SkippedShell::will_run() const
{
  if (run_ignored_) {
    return Shell::will_run();
  }

  return false;
}

String SkippedShell::get_macro_name() const
{
  if (run_ignored_) {
    return "TEST";
  }

  return "SKIPPED_TEST";
}

void SkippedShell::run_one_test(Plugin* plugin, Result& result)
{
  if (run_ignored_) {
    Shell::run_one_test(plugin, result);
    return;
  }

  result.count_skipped();
}

void SkippedShell::set_run_skipped()
{
  run_ignored_ = true;
}

} // namespace test
} // namespace tiny
} // namespace mu
