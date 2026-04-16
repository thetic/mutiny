/**
 * @file
 * @brief @ref mu::tiny::test::Shell subclass for tests marked as ignored.
 *
 * Used by @ref SKIPPED_TEST and @ref SKIPPED_TEST_C_WRAPPER.
 */

#ifndef INCLUDED_MUTINY_TEST_IGNOREDSHELL_HPP
#define INCLUDED_MUTINY_TEST_IGNOREDSHELL_HPP

#include "mutiny/test/Shell.hpp"

#include "mutiny/export.h"

namespace mu {
namespace tiny {
namespace test {

/**
 * @brief Shell for a test that is skipped unless `-r` (run-ignored) is set.
 *
 * Used by the @ref SKIPPED_TEST() macro. The test body is still compiled and
 * registered, but @ref will_run() returns false until @ref set_run_ignored()
 * is called. See also @ref SKIPPED_TEST_C_WRAPPER.
 */
class MUTINY_EXPORT IgnoredShell : public Shell
{
public:
  /** @brief Construct an empty ignored shell (not yet registered). */
  IgnoredShell() noexcept;
  ~IgnoredShell() override = default;
  /**
   * @brief Construct and register an ignored test.
   *
   * @param group_name   Name of the test group.
   * @param test_name    Name of the individual test case.
   * @param file_name    Source file where the test is defined.
   * @param line_number  Line number of the test definition.
   */
  explicit IgnoredShell(
      const char* group_name,
      const char* test_name,
      const char* file_name,
      size_t line_number
  ) noexcept;
  IgnoredShell(const IgnoredShell&) = delete;
  IgnoredShell& operator=(const IgnoredShell&) = delete;

  /**
   * @return false unless set_run_ignored() has been called.
   */
  bool will_run() const override;

  /** @brief Enable running this test (triggered by the `-r` flag). */
  void set_run_ignored() override;

protected:
  /** @return The macro name reported in output (`"SKIPPED_TEST"`). */
  String get_macro_name() const override;

  /**
   * @brief Run the test body (only called when run-ignored mode is active).
   *
   * @param plugin  The active plugin chain.
   * @param result  The active result accumulator.
   */
  void run_one_test(Plugin* plugin, Result& result) override;

private:
  bool run_ignored_;
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
