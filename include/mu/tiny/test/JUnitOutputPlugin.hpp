/**
 * @file
 * @brief @ref mu::tiny::test::Plugin that writes JUnit-compatible XML output.
 */

#ifndef INCLUDED_MU_TINY_TEST_JUNITOUTPUTPLUGIN_HPP
#define INCLUDED_MU_TINY_TEST_JUNITOUTPUTPLUGIN_HPP

#include "mu/tiny/test/Plugin.hpp"

#include "mu/tiny/export.h"

namespace mu {
namespace tiny {
namespace test {

class Output;

/**
 * @brief @ref Plugin that produces a JUnit-style XML report alongside console
 * output.
 *
 * Install this plugin before calling
 * @ref CommandLineRunner::run_all_tests_main() to enable the
 * `-pjunit[=<name>]` command-line argument. When activated,
 * @ref create_output() returns a JUnitOutput that writes XML alongside
 * console output.
 */
class MUTINY_EXPORT JUnitOutputPlugin : public Plugin
{
public:
  /**
   * @brief Construct the plugin with an optional default package name.
   *
   * @param default_package_name  Used as the JUnit package name when
   *   @c -pjunit is given without an explicit @c =<name> suffix.
   *   Pass the executable basename to reproduce the standard behaviour.
   *   Defaults to empty, which leaves the package name unset.
   */
  explicit JUnitOutputPlugin(const String& default_package_name = "");

  /**
   * @brief Handle the `-pjunit[=<name>]` command-line argument.
   *
   * @param argc   Remaining argument count (@p argv[0] through end).
   * @param argv   Pointer to the current argument.
   * @return true if the argument was consumed.
   */
  bool parse_arguments(int argc, const char* const* argv) override;

  /**
   * @brief Return help text for the `-pjunit` option.
   * @return A String describing the argument.
   */
  String get_help() const override;

  /**
   * @brief Create the JUnit XML output object when `-pjunit` was supplied.
   * @return A new JUnitOutput, or nullptr if the plugin is not active.
   */
  Output* create_output() override;

private:
  bool active_{ false };
  String default_package_name_;
  String package_name_;
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
