#include "mu/tiny/test/ExpectFailShell.hpp"

#include "mu/tiny/test/Failure.hpp"
#include "mu/tiny/test/Plugin.hpp"
#include "mu/tiny/test/Result.hpp"
#include "mu/tiny/test/StringBufferOutput.hpp"

namespace mu {
namespace tiny {
namespace test {

ExpectFailShell::ExpectFailShell(
    const char* group_name,
    const char* test_name,
    const char* file_name,
    int_least32_t line_number
)
  : Shell(group_name, test_name, file_name, line_number)
{
}

String ExpectFailShell::get_macro_name() const
{
  return "XFAIL_TEST";
}

void ExpectFailShell::run_one_test(Plugin* plugin, Result& result)
{
  StringBufferOutput shadow_output;
  Result shadow_result(shadow_output);
  Shell::run_one_test(plugin, shadow_result);

  result.count_run();
  if (shadow_result.get_failure_count() == 0) {
    result.add_failure(FailFailure(
        this,
        get_file(),
        get_line_number(),
        "XFAIL_TEST: test was expected to fail but it passed"
    ));
  }
}

} // namespace test
} // namespace tiny
} // namespace mu
