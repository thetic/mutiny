/**
 * @file
 * @brief RAII helper that registers a @ref mu::tiny::test::Shell with the
 * global @ref mu::tiny::test::Registry.
 */

#ifndef INCLUDED_MU_TINY_TEST_INSTALLER_HPP
#define INCLUDED_MU_TINY_TEST_INSTALLER_HPP

#include "mu/tiny/export.h"

#include <stddef.h>
#include <stdint.h>

namespace mu {
namespace tiny {
namespace test {

class Shell;

/**
 * @brief Registers a @ref Shell with the current @ref Registry at construction
 * time.
 *
 * The @ref TEST() and @ref TEST_GROUP() macros create static Installer
 * instances to auto-register each test shell before main() runs. Call @ref
 * un_do() to remove a shell that was registered by mistake (used internally by
 * @ref TestingFixture).
 */
class MUTINY_EXPORT Installer
{
public:
  /**
   * @brief Register @p shell with the current Registry.
   *
   * @param shell        The test shell to register.
   * @param group_name   Name of the test group.
   * @param test_name    Name of the individual test case.
   * @param file_name    Source file where the test is defined.
   * @param line_number  Line number of the test definition.
   */
  explicit Installer(
      Shell& shell,
      const char* group_name,
      const char* test_name,
      const char* file_name,
      int_least32_t line_number
  ) noexcept;
  virtual ~Installer() = default;
  Installer(const Installer&) = delete;
  Installer& operator=(const Installer&) = delete;

  /** @brief Remove the associated shell from the Registry. */
  static void un_do();
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
