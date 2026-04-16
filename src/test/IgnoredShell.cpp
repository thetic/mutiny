#include "mutiny/test/IgnoredShell.hpp"

#include "mutiny/test/Plugin.hpp"
#include "mutiny/test/Result.hpp"

namespace mu {
namespace tiny {
namespace test {

IgnoredShell::IgnoredShell() noexcept
  : run_ignored_(false)
{
}

IgnoredShell::IgnoredShell(
    const char* group_name,
    const char* test_name,
    const char* file_name,
    size_t line_number
) noexcept
  : Shell(group_name, test_name, file_name, line_number)
  , run_ignored_(false)
{
}

bool IgnoredShell::will_run() const
{
  if (run_ignored_)
    return Shell::will_run();

  return false;
}

String IgnoredShell::get_macro_name() const
{
  if (run_ignored_)
    return "TEST";

  return "SKIPPED_TEST";
}

void IgnoredShell::run_one_test(Plugin* plugin, Result& result)
{
  if (run_ignored_) {
    Shell::run_one_test(plugin, result);
    return;
  }

  result.count_ignored();
}

void IgnoredShell::set_run_ignored()
{
  run_ignored_ = true;
}

} // namespace test
} // namespace tiny
} // namespace mu
