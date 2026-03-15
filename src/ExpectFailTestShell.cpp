#include "CppUTest/ExpectFailTestShell.hpp"

#include "CppUTest/StringBufferTestOutput.hpp"
#include "CppUTest/TestFailure.hpp"
#include "CppUTest/TestPlugin.hpp"
#include "CppUTest/TestResult.hpp"

namespace cpputest {

ExpectFailTestShell::ExpectFailTestShell() {}

ExpectFailTestShell::ExpectFailTestShell(const char* group_name,
    const char* test_name,
    const char* file_name,
    size_t line_number)
  : TestShell(group_name, test_name, file_name, line_number)
{
}

ExpectFailTestShell::~ExpectFailTestShell() {}

String
ExpectFailTestShell::get_macro_name() const
{
  return "EXPECT_FAIL_TEST";
}

void
ExpectFailTestShell::run_one_test(TestPlugin* plugin, TestResult& result)
{
  StringBufferTestOutput shadow_output;
  TestResult shadow_result(shadow_output);
  TestShell::run_one_test(plugin, shadow_result);

  result.count_run();
  if (shadow_result.get_failure_count() == 0) {
    result.add_failure(FailFailure(this,
        get_file().c_str(),
        get_line_number(),
        "EXPECT_FAIL_TEST: test was expected to fail but it passed"));
  }
}

} // namespace cpputest
