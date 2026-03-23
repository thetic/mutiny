#ifndef INCLUDED_MUTINY_MOCK_SUPPORTPLUGIN_HPP
#define INCLUDED_MUTINY_MOCK_SUPPORTPLUGIN_HPP

/**
 * @file SupportPlugin.hpp
 * @brief Plugin that automates mock check/clear and comparator lifetime.
 *
 * Install SupportPlugin once in your test runner (or in the test group) to
 * have the framework automatically call mock().check_expectations() and
 * mock().clear() after each test, eliminating the need to write teardown code
 * for every test group.
 *
 * It also provides install_comparator() and install_copier() methods whose
 * lifetime is tied to the plugin scope, ensuring comparators and copiers are
 * removed after each test run.
 *
 * @code{.cpp}
 * int main(int argc, char** argv)
 * {
 *   mu::tiny::mock::SupportPlugin mock_plugin;
 *   return mu::tiny::test::CommandLineRunner::run_all_tests(argc, argv);
 * }
 * @endcode
 *
 * @see Support, Plugin
 */

#include "mutiny/mock/NamedValueComparator.hpp"
#include "mutiny/mock/NamedValueComparatorsAndCopiersRepository.hpp"
#include "mutiny/mock/NamedValueCopier.hpp"

#include "mutiny/test/Plugin.hpp"

namespace mu {
namespace tiny {
namespace mock {

/**
 * @brief Plugin that manages mock lifecycle and comparator/copier
 * registration.
 *
 * pre_test_action() installs the plugin's comparator/copier repository into the
 * global mock scope. post_test_action() calls check_expectations() and clear()
 * on the global mock, then removes the repository.
 */
class SupportPlugin : public mu::tiny::test::Plugin
{
public:
  /**
   * @brief Construct the plugin with an optional name.
   *
   * @param name  Plugin identifier; defaults to "SupportPlugin".
   */
  SupportPlugin(const mu::tiny::test::String& name = "SupportPlugin");
  ~SupportPlugin() override;

  /**
   * @brief Install the comparator/copier repository before the test runs.
   *
   * @param test    The test shell about to run.
   * @param result  The active test result accumulator.
   */
  void pre_test_action(
      mu::tiny::test::Shell& test,
      mu::tiny::test::Result& result
  ) override;

  /**
   * @brief Call mock().check_expectations() and mock().clear() after the test.
   *
   * @param test    The test shell that just ran.
   * @param result  The active test result accumulator.
   */
  void post_test_action(
      mu::tiny::test::Shell& test,
      mu::tiny::test::Result& result
  ) override;

  /**
   * @brief Register a comparator for @p name that persists across tests.
   *
   * The comparator is installed into the plugin's internal repository and
   * made available to all tests that run while this plugin is active.
   *
   * @param name        Type name key.
   * @param comparator  Comparator whose lifetime must exceed this plugin.
   */
  virtual void install_comparator(
      const mu::tiny::test::String& name,
      NamedValueComparator& comparator
  );

  /**
   * @brief Register a copier for @p name that persists across tests.
   *
   * @param name    Type name key.
   * @param copier  Copier whose lifetime must exceed this plugin.
   */
  virtual void install_copier(
      const mu::tiny::test::String& name,
      NamedValueCopier& copier
  );

  /** @brief Remove all registered comparators and copiers from this plugin. */
  void clear();

private:
  NamedValueComparatorsAndCopiersRepository repository_;
};

} // namespace mock
} // namespace tiny
} // namespace mu

#endif
