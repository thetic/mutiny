/**
 * @file
 * @brief @ref mu::tiny::test::Plugin that automates mock check/clear and
 * comparator lifetime.
 *
 * Install @ref mu::tiny::mock::SupportPlugin once in your test runner (or in
 * the test group) to have the framework automatically call @ref
 * mu::tiny::mock::Support::check_expectations() and @ref
 * mu::tiny::mock::Support::clear() after each test, eliminating the need to
 * write teardown code for every test group.
 *
 * It also provides @ref mu::tiny::mock::SupportPlugin::install_comparator() and
 * @ref mu::tiny::mock::SupportPlugin::install_copier() methods whose lifetime
 * is tied to the plugin scope, ensuring comparators and copiers are removed
 * after each test run.
 */

#ifndef INCLUDED_MU_TINY_MOCK_SUPPORTPLUGIN_HPP
#define INCLUDED_MU_TINY_MOCK_SUPPORTPLUGIN_HPP

#include "mu/tiny/mock/NamedValueComparator.hpp"
#include "mu/tiny/mock/NamedValueComparatorsAndCopiersRepository.hpp"
#include "mu/tiny/mock/NamedValueCopier.hpp"

#include "mu/tiny/test/Plugin.hpp"

#include "mu/tiny/export.h"

namespace mu {
namespace tiny {
namespace mock {

/**
 * @brief @ref Plugin that manages mock lifecycle and comparator/copier
 * registration.
 *
 * @ref pre_test_action() installs the plugin's comparator/copier repository
 * into the global mock scope. @ref post_test_action() calls
 * @ref Support::check_expectations() and @ref Support::clear() on the global
 * mock, then removes the repository.
 */
class MUTINY_EXPORT SupportPlugin : public test::Plugin
{
public:
  /**
   * @brief Construct the plugin with an optional name.
   *
   * @param name  Plugin identifier; defaults to "SupportPlugin".
   */
  SupportPlugin(const String& name = "SupportPlugin");
  ~SupportPlugin() override;

  /**
   * @brief Install the comparator/copier repository before the test runs.
   *
   * @param test    The test shell about to run.
   * @param result  The active test result accumulator.
   */
  void pre_test_action(test::Shell& test, test::Result& result) override;

  /**
   * @brief Call mock().check_expectations() and mock().clear() after the test.
   *
   * @param test    The test shell that just ran.
   * @param result  The active test result accumulator.
   */
  void post_test_action(test::Shell& test, test::Result& result) override;

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
      const String& name,
      NamedValueComparator& comparator
  );

  /**
   * @brief Register a copier for @p name that persists across tests.
   *
   * @param name    Type name key.
   * @param copier  Copier whose lifetime must exceed this plugin.
   */
  virtual void install_copier(const String& name, NamedValueCopier& copier);

  /** @brief Remove all registered comparators and copiers from this plugin. */
  void clear();

private:
  NamedValueComparatorsAndCopiersRepository repository_;
};

} // namespace mock
} // namespace tiny
} // namespace mu

#endif
