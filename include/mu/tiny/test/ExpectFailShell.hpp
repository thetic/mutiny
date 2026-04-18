/**
 * @file
 * @brief @ref mu::tiny::test::Shell subclass for tests expected to fail.
 *
 * Used by @ref XFAIL_TEST and @ref XFAIL_TEST_C_WRAPPER.
 */

#ifndef INCLUDED_MU_TINY_TEST_EXPECTFAILSHELL_HPP
#define INCLUDED_MU_TINY_TEST_EXPECTFAILSHELL_HPP

#include "mu/tiny/test/Shell.hpp"

#include "mu/tiny/export.h"

namespace mu {
namespace tiny {
namespace test {

/**
 * @brief Shell that inverts the pass/fail outcome of a test.
 *
 * Used by the @ref XFAIL_TEST() macro. The test body is expected to
 * trigger a failure; if it passes without failing, the shell marks it as
 * failed. See also @ref XFAIL_TEST_C_WRAPPER.
 */
class MUTINY_EXPORT ExpectFailShell : public Shell
{
public:
  ExpectFailShell() = default;
  /**
   * @brief Construct and register an expect-fail test.
   *
   * @param group_name   Name of the test group.
   * @param test_name    Name of the individual test case.
   * @param file_name    Source file where the test is defined.
   * @param line_number  Line number of the test definition.
   */
  explicit ExpectFailShell(
      const char* group_name,
      const char* test_name,
      const char* file_name,
      size_t line_number
  );
  ~ExpectFailShell() override = default;
  ExpectFailShell(const ExpectFailShell&) = delete;
  ExpectFailShell& operator=(const ExpectFailShell&) = delete;

protected:
  /** @return The macro name reported in failure messages
   * (`"XFAIL_TEST"`).
   */
  String get_macro_name() const override;

  /**
   * @brief Run the test body and invert the result.
   *
   * @param plugin  The active plugin chain.
   * @param result  The active result accumulator.
   */
  void run_one_test(Plugin* plugin, Result& result) override;
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
