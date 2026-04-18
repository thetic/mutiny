/**
 * @file
 * @brief Tests with a deterministic execution order based on a numeric level.
 *
 * By default mutiny runs tests in an unspecified order. Ordered tests run in
 * ascending level order relative to each other, and all run after the
 * unordered tests in the registry.
 *
 * @see TEST_ORDERED
 */

#ifndef INCLUDED_MU_TINY_TEST_ORDERED_HPP
#define INCLUDED_MU_TINY_TEST_ORDERED_HPP

#include "mu/tiny/test/Shell.hpp"

#include "mu/tiny/export.h"

namespace mu {
namespace tiny {
namespace test {

/**
 * @brief Shell subclass that participates in ordered test execution.
 *
 * Maintains a separate singly-linked list of ordered tests sorted by level.
 * The test runner walks this list after processing all unordered tests.
 * Users interact with this class only through the TEST_ORDERED macro.
 */
class MUTINY_EXPORT OrderedShell : public mu::tiny::test::Shell
{
public:
  OrderedShell() noexcept = default;
  ~OrderedShell() override = default;

  /**
   * @brief Insert @p test into this shell's ordered-test list.
   *
   * @param test  Shell to append.
   * @return @p test.
   */
  virtual OrderedShell* add_ordered_test(OrderedShell* test);

  /** @return The next shell in the ordered list, or nullptr at the end. */
  virtual OrderedShell* get_next_ordered_test();

  /** @return true — ordered tests report themselves as ordered. */
  bool is_ordered() const override;

  /** @return This shell's execution level. */
  int get_level();

  /**
   * @brief Set this shell's execution level.
   *
   * @param level  Numeric level; lower values run first.
   */
  void set_level(int level);

  /**
   * @brief Prepend @p test to the global ordered-test list.
   *
   * Called by OrderedInstaller during static initialisation.
   *
   * @param test  Shell to add.
   */
  static void add_ordered_test_to_head(OrderedShell* test);

  /** @return The head of the global ordered-test linked list. */
  static OrderedShell* get_ordered_test_head();

  /** @return true if no ordered tests have been registered yet. */
  static bool first_ordered_test();

  /**
   * @brief Replace the head of the global ordered-test list.
   *
   * @param test  New head (may be nullptr to clear the list).
   */
  static void set_ordered_test_head(OrderedShell* test);

private:
  static OrderedShell* ordered_tests_head_;
  OrderedShell* next_ordered_test_{ nullptr };

  int level_{ 0 };
};

/**
 * @brief Registers an OrderedShell into the sorted ordered-test list.
 *
 * Instantiated once per TEST_ORDERED declaration as a namespace-scope static.
 * Users do not interact with this class directly.
 */
class MUTINY_EXPORT OrderedInstaller
{
public:
  /**
   * @brief Register @p test in the ordered list at the appropriate position.
   *
   * @param test         The shell to register.
   * @param group_name   Test group name string.
   * @param test_name    Test name string.
   * @param file_name    Source file path.
   * @param line_number  Source line number.
   * @param level        Execution level; lower values run first.
   */
  explicit OrderedInstaller(
      OrderedShell& test,
      const char* group_name,
      const char* test_name,
      const char* file_name,
      size_t line_number,
      int level
  ) noexcept;
  virtual ~OrderedInstaller() = default;

private:
  void add_ordered_test_in_order(OrderedShell* test);
  void add_ordered_test_in_order_not_at_head_position(OrderedShell* test);
};

} // namespace test
} // namespace tiny
} // namespace mu

/**
 * @brief Define a test that runs at a specific position relative to other
 * ordered tests.
 *
 * Like @ref TEST(), but execution is deferred until all unordered tests have
 * run. Among ordered tests, those with a lower @p testLevel run first. Tests
 * with equal levels run in an unspecified order relative to each other.
 *
 * @code{.cpp}
 * TEST_ORDERED(Lifecycle, init,    10) { system_init(); }
 * TEST_ORDERED(Lifecycle, run,     20) { system_run(); }
 * TEST_ORDERED(Lifecycle, cleanup, 30) { system_cleanup(); }
 * @endcode
 *
 * @param testGroup  Test group (must be declared with @ref TEST_GROUP
 *                   beforehand).
 * @param testName   Unique test name within the group.
 * @param testLevel  Integer level; lower values run first.
 *
 * Use @ref TEST_ORDERED_C_WRAPPER for bridging C test functions.
 */
#define TEST_ORDERED(testGroup, testName, testLevel)                           \
  /* declarations for compilers */                                             \
  class TEST_##testGroup##_##testName##_TestShell;                             \
  extern TEST_##testGroup##_##testName##_TestShell                             \
      TEST_##testGroup##_##testName##_Instance;                                \
  class TEST_##testGroup##_##testName##_Test                                   \
    : public TEST_GROUP_##MutinyGroup##testGroup                               \
  {                                                                            \
  public:                                                                      \
    TEST_##testGroup##_##testName##_Test()                                     \
      : TEST_GROUP_##MutinyGroup##testGroup()                                  \
    {                                                                          \
    }                                                                          \
    void test_body() override;                                                 \
  };                                                                           \
  class TEST_##testGroup##_##testName##_TestShell                              \
    : public mu::tiny::test::OrderedShell                                      \
  {                                                                            \
    virtual mu::tiny::test::Test* create_test() override                       \
    {                                                                          \
      return new TEST_##testGroup##_##testName##_Test;                         \
    }                                                                          \
  } TEST_##testGroup##_##testName##_Instance;                                  \
  namespace {                                                                  \
  mu::tiny::test::OrderedInstaller TEST_##testGroup##_##testName##_Installer(  \
      TEST_##testGroup##_##testName##_Instance,                                \
      #testGroup,                                                              \
      #testName,                                                               \
      __FILE__,                                                                \
      __LINE__,                                                                \
      testLevel                                                                \
  );                                                                           \
  } /* namespace */                                                            \
  void TEST_##testGroup##_##testName##_Test::test_body()

/**
 * @brief Bridge a C test function into a @ref TEST_ORDERED.
 *
 * @param group_name  Test group.
 * @param test_name   Test name.
 * @param testLevel   Execution level.
 */
#define TEST_ORDERED_C_WRAPPER(group_name, test_name, testLevel)               \
  extern "C" void test_##group_name##_##test_name##_wrapper_c(void);           \
  TEST_ORDERED(group_name, test_name, testLevel)                               \
  {                                                                            \
    test_##group_name##_##test_name##_wrapper_c();                             \
  }

#endif
