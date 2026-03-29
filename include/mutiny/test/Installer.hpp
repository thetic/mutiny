/**
 * @file
 * @brief RAII helper that registers a Shell with the global Registry.
 *
 * @see Shell, Registry
 */

#ifndef INCLUDED_MUTINY_TEST_INSTALLER_HPP
#define INCLUDED_MUTINY_TEST_INSTALLER_HPP

#include "mutiny/export.h"

#include <stddef.h>

namespace mu {
namespace tiny {
namespace test {

class Shell;

/**
 * @brief Registers a Shell with the current Registry at construction time.
 *
 * The TEST() and TEST_GROUP() macros create static Installer instances to
 * auto-register each test shell before main() runs. Call un_do() to remove
 * a shell that was registered by mistake (used internally by TestingFixture).
 *
 * @see Shell, Registry
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
      size_t line_number
  ) noexcept;
  virtual ~Installer() = default;
  Installer(const Installer&) = delete;
  Installer& operator=(const Installer&) = delete;

  /** @brief Remove the associated shell from the Registry. */
  void un_do();
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
