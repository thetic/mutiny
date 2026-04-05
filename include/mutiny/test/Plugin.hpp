/**
 * @file
 * @brief Extension point for running code before/after every test.
 *
 * Subclass @ref mu::tiny::test::Plugin to intercept the test lifecycle,
 * contribute a custom @ref mu::tiny::test::Output, or handle extra
 * command-line arguments. Plugins form a singly-linked list; install them with
 * @ref mu::tiny::test::CommandLineRunner before calling @ref
 * mu::tiny::test::CommandLineRunner::run_all_tests_main(). See also @ref
 * mu::tiny::mock::SupportPlugin and @ref mu::tiny::test::SetPointerPlugin for
 * built-in plugin implementations.
 */

#ifndef INCLUDED_MUTINY_TEST_PLUGIN_HPP
#define INCLUDED_MUTINY_TEST_PLUGIN_HPP

#include "mutiny/String.hpp"
#include "mutiny/export.h"

namespace mu {
namespace tiny {
namespace test {

class Output;
class Shell;
class Result;

/**
 * @brief Base class for test lifecycle extensions.
 *
 * Plugins are linked together in a chain; all pre_test_action(),
 * post_test_action(), and parse_arguments() calls propagate through the
 * entire chain automatically via the run_all_*() and parse_all_*() helpers.
 */
class MUTINY_EXPORT Plugin
{
public:
  /**
   * @brief Construct a named plugin.
   *
   * @param name  Identifier used for logging and retrieval by name.
   */
  Plugin(const String& name);
  virtual ~Plugin() = default;

  /**
   * @brief Called once before each test body executes.
   *
   * Override to set up per-test state. The default implementation does nothing.
   *
   * @param test    The test shell about to run.
   * @param result  The active test result accumulator.
   */
  virtual void pre_test_action(Shell& test, Result& result);

  /**
   * @brief Called once after each test body completes (pass or fail).
   *
   * Override to tear down per-test state. The default implementation does
   * nothing.
   *
   * @param test    The test shell that just ran.
   * @param result  The active test result accumulator.
   */
  virtual void post_test_action(Shell& test, Result& result);

  /**
   * @brief Handle a custom command-line argument.
   *
   * Called for each argument position @p index. Return true to indicate the
   * argument was consumed (the runner will skip to the next one).
   *
   * @param argc   Total argument count.
   * @param argv   Argument vector.
   * @param index  Current argument index to inspect.
   * @return true if the argument at @p index was consumed.
   */
  virtual bool parse_arguments(int argc, const char* const* argv, int index);

  /**
   * @brief Return a help string for this plugin's command-line arguments.
   *
   * @return Help text, or an empty string if no arguments are accepted.
   */
  virtual String get_help() const { return ""; }

  /**
   * @brief Create an @ref Output for this plugin, if any.
   *
   * @return A new @ref Output, or nullptr if this plugin produces no output.
   */
  virtual Output* create_output() { return nullptr; }

  /**
   * @brief Collect all outputs from the plugin chain.
   *
   * Calls create_output() on each enabled plugin in the chain and wires them
   * together as a composite output.
   *
   * @return Combined Output for the entire plugin chain.
   */
  virtual Output* create_all_outputs();

  /** @brief Invoke pre_test_action() on this plugin and all following plugins.
   */
  virtual void run_all_pre_test_action(Shell& test, Result& result);
  /** @brief Invoke post_test_action() on this plugin and all following plugins.
   */
  virtual void run_all_post_test_action(Shell& test, Result& result);

  /**
   * @brief Invoke parse_arguments() on each plugin in the chain.
   * @param argc   Argument count.
   * @param argv   Argument vector (const).
   * @param index  Current argument index.
   * @return true if any plugin consumed the argument.
   */
  virtual bool parse_all_arguments(
      int argc,
      const char* const* argv,
      int index
  );
  /**
   * @brief Overload accepting a non-const argv.
   * @param argc   Argument count.
   * @param argv   Argument vector.
   * @param index  Current argument index.
   * @return true if any plugin consumed the argument.
   */
  virtual bool parse_all_arguments(int argc, char** argv, int index);

  /** @return Concatenated help strings from all plugins in the chain. */
  virtual String get_all_help() const;

  /**
   * @brief Append @p plugin to the end of the plugin chain.
   *
   * @param plugin  Plugin to add; must not already be in a chain.
   * @return @p plugin.
   */
  virtual Plugin* add_plugin(Plugin* plugin);

  /**
   * @brief Remove the first plugin with the given @p name from the chain.
   *
   * @param name  Name to search for.
   * @return The removed plugin, or nullptr if not found.
   */
  virtual Plugin* remove_plugin_by_name(const String& name);

  /** @return The next plugin in the chain, or nullptr. */
  virtual Plugin* get_next();

  /** @brief Disable this plugin (pre_test_action/post_test_action become
   * no-ops). */
  virtual void disable();
  /** @brief Re-enable this plugin. */
  virtual void enable();
  /** @return true if this plugin is currently enabled. */
  virtual bool is_enabled();

  /** @return This plugin's name. */
  const String& get_name();

  /**
   * @brief Find a plugin by name in this chain.
   *
   * @param name  Name to search for.
   * @return Pointer to the matching plugin, or nullptr.
   */
  Plugin* get_plugin_by_name(const String& name);

protected:
  /**
   * @brief Construct from an existing chain link (used internally for head
   * nodes).
   *
   * @param next  Pointer to the next plugin in the list.
   */
  Plugin(Plugin* next);

private:
  Plugin* next_;
  String name_;
  bool enabled_;
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
