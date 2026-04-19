/**
 * @file
 * @brief @ref mu::tiny::test::Plugin that writes TAP version 13 output.
 *
 * See @ref mu::tiny::test::TapOutputPlugin::name for the canonical plugin
 * identifier.
 */

#ifndef INCLUDED_MU_TINY_TEST_TAPOUTPUTPLUGIN_HPP
#define INCLUDED_MU_TINY_TEST_TAPOUTPUTPLUGIN_HPP

#include "mu/tiny/test/Plugin.hpp"

#include "mu/tiny/export.h"

namespace mu {
namespace tiny {
namespace test {

class Output;

/**
 * @brief @ref Plugin that produces TAP version 13 output on stdout.
 *
 * Install this plugin before calling
 * @ref CommandLineRunner::run_all_tests_main() to enable the @c -ptap
 * command-line argument. When activated, @ref create_output() returns a
 * TapOutput that writes TAP to stdout, replacing the normal console output.
 * SKIP directives are emitted for ignored and skipped tests; YAML diagnostic
 * blocks carry the failure message and location for failing tests.
 */
class MUTINY_EXPORT TapOutputPlugin : public Plugin
{
public:
  /** @brief Construct and name the plugin. */
  TapOutputPlugin();

  /**
   * @brief Handle the @c -ptap command-line argument.
   *
   * @param argc   Argument count.
   * @param argv   Argument vector.
   * @param index  Current argument index.
   * @return true if the argument was consumed.
   */
  bool parse_arguments(int argc, const char* const* argv, int index) override;

  /**
   * @brief Return help text for the @c -ptap option.
   * @return A String describing the argument.
   */
  String get_help() const override;

  /**
   * @brief Create the TAP output object when @c -ptap was supplied.
   * @return A new TapOutput, or nullptr if the plugin is not active.
   */
  Output* create_output() override;

  /** @brief Canonical plugin name; used for look-up by
   * @ref Registry::get_plugin_by_name(). */
  static constexpr const char* name{ "TapOutputPlugin" };

private:
  bool active_{ false };
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
