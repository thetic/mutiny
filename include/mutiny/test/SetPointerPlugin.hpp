#ifndef INCLUDED_MUTINY_TEST_SETPOINTERPLUGIN_HPP
#define INCLUDED_MUTINY_TEST_SETPOINTERPLUGIN_HPP

/**
 * @file SetPointerPlugin.hpp
 * @brief Automatic pointer restoration after each test.
 *
 * SetPointerPlugin records the original address stored in a pointer variable
 * before it is overwritten and restores it at the end of every test, without
 * requiring explicit teardown code.
 *
 * Use the MUTINY_PTR_SET macro (the preferred interface) or call
 * SetPointerPlugin::set_pointer() directly.
 *
 * @code
 * // In your test group, install the plugin once:
 * SetPointerPlugin setPlugin;
 * // (Alternatively, register it with CommandLineRunner.)
 *
 * TEST(Foo, uses_global)
 * {
 *   MUTINY_PTR_SET(g_driver, &stub_driver);
 *   // g_driver is automatically restored to its original value after the test.
 * }
 * @endcode
 *
 * @see MUTINY_PTR_SET
 */

#include "mutiny/test/Plugin.hpp"

namespace mu {
namespace tiny {
namespace test {

/**
 * @brief Plugin that restores pointer variables to their original values.
 *
 * Up to @ref max_set pointers may be registered per test. The plugin records
 * the address of each pointer variable before it is overwritten; on
 * post_test_action() it writes the saved address back.
 */
class SetPointerPlugin : public Plugin
{
public:
  /** @brief Construct the plugin and prepare the storage array. */
  SetPointerPlugin();

  /**
   * @brief Restore all registered pointer variables to their original values.
   *
   * Called automatically by the test runner after each test.
   *
   * @param test    The test shell that just ran.
   * @param result  The active test result accumulator.
   */
  void post_test_action(Shell& test, Result& result) override;

  /** @brief Maximum number of pointers that can be registered per test. */
  static constexpr int max_set = 32;
  /** @brief Plugin name used for identification in the plugin chain. */
  static constexpr const char* name{ "SetPointerPlugin" };

  /**
   * @brief Save the original value of @p a, then set @p a = @p b.
   *
   * The original value is restored automatically at end-of-test via
   * post_test_action(). Prefer the MUTINY_PTR_SET macro.
   *
   * @tparam T  Pointee type; deduced from @p a.
   * @param a   Reference to the pointer variable to overwrite.
   * @param b   New value to assign to @p a.
   */
  template<typename T>
  static void set_pointer(T*& a, T* b)
  {
    store(reinterpret_cast<void**>(&(a)));
    a = b;
  }

private:
  static void store(void** location);
};

}
}
} // namespace mu::tiny::test

/**
 * @brief Save the original value of pointer @p a, then set @p a = @p b.
 *
 * The original value is automatically restored after the test ends.
 * Requires SetPointerPlugin to be installed in the test runner's plugin chain.
 *
 * @param a  Pointer variable to overwrite (must be an lvalue).
 * @param b  New pointer value.
 *
 * @see SetPointerPlugin::set_pointer()
 */
#define MUTINY_PTR_SET(a, b)                                                   \
  mu::tiny::test::SetPointerPlugin::set_pointer((a), b)

#endif
