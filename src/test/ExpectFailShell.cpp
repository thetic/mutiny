#include "mutiny/test/ExpectFailShell.hpp"

#include "mutiny/test/Failure.hpp"
#include "mutiny/test/Plugin.hpp"
#include "mutiny/test/Result.hpp"
#include "mutiny/test/StringBufferOutput.hpp"

namespace mu {
namespace tiny {
namespace test {

ExpectFailShell::ExpectFailShell(
    const char* group_name,
    const char* test_name,
    const char* file_name,
    size_t line_number
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
