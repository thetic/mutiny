/**
 * @file
 * @brief @ref mu::tiny::test::Plugin that writes JUnit-compatible XML output.
 *
 * See @ref mu::tiny::test::JUnitOutputPlugin::name for the canonical plugin
 * identifier.
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
  /** @brief Construct and name the plugin. */
  JUnitOutputPlugin();

  /**
   * @brief Handle the `-pjunit[=<name>]` command-line argument.
   *
   * @param argc   Argument count.
   * @param argv   Argument vector.
   * @param index  Current argument index.
   * @return true if the argument was consumed.
   */
  bool parse_arguments(int argc, const char* const* argv, int index) override;

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

  /** @brief Canonical plugin name; used for look-up by
   * @ref Registry::get_plugin_by_name(). */
  static constexpr const char* name{ "JUnitOutputPlugin" };

private:
  bool active_{ false };
  String package_name_;
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
