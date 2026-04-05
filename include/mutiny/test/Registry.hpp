/**
 * @file
 * @brief Global collection of tests that can be discovered, filtered, and run.
 *
 * Used by @ref mu::tiny::test::CommandLineRunner to drive the test suite.
 * Tests are registered into the Registry by @ref mu::tiny::test::Shell
 * instances (via @ref mu::tiny::test::Installer) and executed via @ref
 * mu::tiny::test::Plugin hooks.
 */

#ifndef INCLUDED_MUTINY_TEST_REGISTRY_HPP
#define INCLUDED_MUTINY_TEST_REGISTRY_HPP

#include "mutiny/test/Filter.hpp"

#include "mutiny/String.hpp"
#include "mutiny/export.h"

#include <stddef.h>

namespace mu {
namespace tiny {
namespace test {

class Shell;
class Result;
class Plugin;

/**
 * @brief Central repository of all registered tests.
 *
 * Tests are registered automatically at static-initialization time by
 * @ref Installer instances created by the @ref TEST() and @ref TEST_GROUP()
 * macros. @ref CommandLineRunner retrieves the current registry via
 * @ref get_current_registry() and delegates to @ref run_all_tests() to execute
 * them.
 */
class MUTINY_EXPORT Registry
{
public:
  /** @brief Construct an empty registry. */
  Registry();
  virtual ~Registry() = default;

  /**
   * @brief Append @p test to the registered test list.
   * @param test  Shell to register.
   */
  virtual void add_test(Shell* test);

  /** @brief Remove the most recently added test (used by
   * @ref Installer::un_do()). */
  virtual void un_do_last_add_test();

  /** @return Number of registered tests. */
  virtual size_t count_tests();

  /**
   * @brief Run all tests that pass the current filters.
   * @param result  Accumulator for results and output.
   */
  virtual void run_all_tests(Result& result);

  /**
   * @brief Shuffle the test execution order using a seeded algorithm.
   * @param seed  Random seed.
   */
  virtual void shuffle_tests(size_t seed);

  /** @brief Reverse the test execution order. */
  virtual void reverse_tests();

  /**
   * @brief Print all registered group names to @p result.
   * @param result  Output sink.
   */
  virtual void list_test_group_names(Result& result);

  /**
   * @brief Print all `group.case` names to @p result.
   * @param result  Output sink.
   */
  virtual void list_test_group_and_case_names(Result& result);

  /**
   * @brief Print all test source locations to @p result.
   * @param result  Output sink.
   */
  virtual void list_test_locations(Result& result);

  /**
   * @brief Print all ordered-test source locations to @p result.
   * @param result  Output sink.
   */
  virtual void list_ordered_test_locations(Result& result);

  /**
   * @brief Set the name filters applied when running tests.
   * @param filters  Linked list of name filters; may be nullptr.
   */
  virtual void set_name_filters(const Filter* filters);

  /**
   * @brief Set the group filters applied when running tests.
   * @param filters  Linked list of group filters; may be nullptr.
   */
  virtual void set_group_filters(const Filter* filters);

  /**
   * @brief Append @p plugin to the plugin chain.
   * @param plugin  Plugin to install.
   */
  virtual void install_plugin(Plugin* plugin);

  /** @brief Remove all plugins from the chain. */
  virtual void reset_plugins();

  /** @return The first plugin in the chain, or nullptr. */
  virtual Plugin* get_first_plugin();

  /**
   * @brief Find a plugin by name.
   * @param name  Name to search for.
   * @return The matching plugin, or nullptr.
   */
  virtual Plugin* get_plugin_by_name(const String& name);

  /**
   * @brief Remove the first plugin with the given name.
   * @param name  Name to search for and remove.
   */
  virtual void remove_plugin_by_name(const String& name);

  /** @return Number of installed plugins. */
  virtual int count_plugins();

  /** @return The first registered test, or nullptr. */
  virtual Shell* get_first_test();

  /**
   * @brief Return the test after @p test in registration order.
   * @param test  Current test.
   * @return The next test, or nullptr.
   */
  virtual Shell* get_test_with_next(Shell* test);

  /**
   * @brief Find the first test whose name matches @p name.
   * @param name  Test name to search for.
   * @return The matching Shell, or nullptr.
   */
  virtual Shell* find_test_with_name(const String& name);

  /**
   * @brief Find the first test in the group named @p name.
   * @param name  Group name to search for.
   * @return The matching Shell, or nullptr.
   */
  virtual Shell* find_test_with_group(const String& name);

  /**
   * @return The process-wide current registry (used by the @ref TEST() macro
   *         and @ref CommandLineRunner).
   */
  static Registry* get_current_registry();

  /**
   * @brief Replace the process-wide current registry.
   * @param registry  New current registry.
   */
  virtual void set_current_registry(Registry* registry);

  /** @return The current repetition number (1-based during a multi-run). */
  int get_current_repetition();

  /** @brief Enable running @ref IGNORE_TEST() tests in this registry. */
  void set_run_ignored();

private:
  bool test_should_run(Shell* test, Result& result);
  bool end_of_group(Shell* test);

  Shell* tests_{ nullptr };
  const Filter* name_filters_{ nullptr };
  const Filter* group_filters_{ nullptr };
  Plugin* first_plugin_;
  static Registry* current_registry_;
  int current_repetition_{ 0 };
  bool run_ignored_{ false };
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
