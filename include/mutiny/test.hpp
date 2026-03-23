#ifndef INCLUDED_MUTINY_MUTINY_HPP
#define INCLUDED_MUTINY_MUTINY_HPP

/**
 * @file mutiny/test.hpp
 * @brief Primary test-definition header for mutiny.
 *
 * Include this header in every C++ test file. It provides the macros needed
 * to declare test groups and individual tests: TEST_GROUP, TEST, IGNORE_TEST,
 * EXPECT_FAIL_TEST, and the C-interop wrappers. Assertion macros are provided
 * by TestShell.hpp, which this header includes transitively.
 *
 * Typical usage:
 * @code
 * #include "mutiny/test.hpp"
 *
 * TEST_GROUP(MyGroup)
 * {
 *   void setup() override { ... }
 *   void teardown() override { ... }
 * };
 *
 * TEST(MyGroup, does_something)
 * {
 *   CHECK_EQUAL(42, compute());
 * }
 * @endcode
 *
 * @see TestShell.hpp for assertion macros
 * @see OrderedTest.hpp for tests with a defined execution order
 */

#include "mutiny/test/ExpectFailShell.hpp"
#include "mutiny/test/Failure.hpp"
#include "mutiny/test/IgnoredShell.hpp"
#include "mutiny/test/Installer.hpp"
#include "mutiny/test/Plugin.hpp"
#include "mutiny/test/Result.hpp"
#include "mutiny/test/SetPointerPlugin.hpp"
#include "mutiny/test/Shell.hpp"
#include "mutiny/test/ShellPointerArray.hpp"
#include "mutiny/test/String.hpp"
#include "mutiny/test/Test.hpp"

/**
 * @brief Define a custom base class for test groups.
 *
 * Creates a struct that derives from mu::tiny::test::Test. Use with
 * @ref TEST_GROUP_BASE to share helper state across groups, or standalone to
 * define a named base for multiple groups.
 *
 * @param testBaseClass  Name of the new base struct.
 */
#define TEST_BASE(testBaseClass)                                               \
  struct testBaseClass : public mu::tiny::test::Test

/**
 * @brief Define a test group with a custom base class.
 *
 * All tests in the group share the same setup() and teardown() methods
 * (both optional). setup() runs before each test body; teardown() runs after,
 * even if the test fails.
 *
 * Use this variant to provide a custom base class (created with @ref TEST_BASE)
 * that carries shared fixtures or helpers. For the common case, prefer
 * @ref TEST_GROUP which uses mu::tiny::test::Test directly.
 *
 * @param testGroup  Identifier for the group (used in filtering and output).
 * @param baseclass  Base class that must derive from mu::tiny::test::Test.
 *
 * @see TEST_GROUP, TEST_BASE
 */
#define TEST_GROUP_BASE(testGroup, baseclass)                                  \
  extern int externTestGroup##testGroup;                                       \
  int externTestGroup##testGroup = 0;                                          \
  struct TEST_GROUP_##MutinyGroup##testGroup : public baseclass

/**
 * @brief Define a named test group using mu::tiny::test::Test as the base
 * class.
 *
 * All tests in the group share the same setup() and teardown() methods.
 * setup() runs before each test body; teardown() runs after, even if the
 * test fails. A group body may also declare helper member variables and
 * methods accessible to all tests in the group.
 *
 * @code
 * TEST_GROUP(Calculator)
 * {
 *   Calculator* calc;
 *
 *   void setup() override    { calc = new Calculator; }
 *   void teardown() override { delete calc; }
 * };
 * @endcode
 *
 * @param testGroup  Identifier for the group (used in filtering and output).
 *
 * @see TEST_GROUP_BASE, TEST_SETUP, TEST_TEARDOWN
 */
#define TEST_GROUP(testGroup) TEST_GROUP_BASE(testGroup, mu::tiny::test::Test)

/**
 * @brief Define the setup method for a test group.
 *
 * The body you write immediately after this macro runs before the body of
 * each TEST in the group. Must appear inside a TEST_GROUP or TEST_GROUP_BASE
 * struct body.
 *
 * @see TEST_TEARDOWN
 */
#define TEST_SETUP() virtual void setup() override

/**
 * @brief Define the teardown method for a test group.
 *
 * The body you write immediately after this macro runs after each TEST in the
 * group, even when the test fails. Must appear inside a TEST_GROUP or
 * TEST_GROUP_BASE struct body.
 *
 * @see TEST_SETUP
 */
#define TEST_TEARDOWN() virtual void teardown() override

/**
 * @brief Define an individual test.
 *
 * The test body follows this macro in braces. setup() is called first,
 * then the body, then teardown(). A test failure aborts the body but still
 * calls teardown().
 *
 * @code
 * TEST(Calculator, add_two_numbers)
 * {
 *   CHECK_EQUAL(5, calc->add(2, 3));
 * }
 * @endcode
 *
 * @param testGroup  The group this test belongs to (must be declared first).
 * @param testName   Unique name within the group.
 *
 * @see IGNORE_TEST, EXPECT_FAIL_TEST
 */
#define TEST(testGroup, testName)                                              \
  /* External declarations for strict compilers */                             \
  class TEST_##testGroup##_##testName##_TestShell;                             \
  extern TEST_##testGroup##_##testName##_TestShell                             \
      TEST_##testGroup##_##testName##_TestShell_instance;                      \
                                                                               \
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
    : public mu::tiny::test::TestShell                                         \
  {                                                                            \
    virtual mu::tiny::test::Test* create_test() override                       \
    {                                                                          \
      return new TEST_##testGroup##_##testName##_Test;                         \
    }                                                                          \
  } TEST_##testGroup##_##testName##_TestShell_instance;                        \
  namespace {                                                                  \
  mu::tiny::test::TestInstaller TEST_##testGroup##_##testName##_Installer(     \
      TEST_##testGroup##_##testName##_TestShell_instance,                      \
      #testGroup,                                                              \
      #testName,                                                               \
      __FILE__,                                                                \
      __LINE__                                                                 \
  );                                                                           \
  } /* namespace */                                                            \
  void TEST_##testGroup##_##testName##_Test::test_body()

/**
 * @brief Define a test that is skipped during a test run.
 *
 * The test is registered and counted as "ignored" but its body is never
 * executed. Useful for temporarily disabling a test without deleting it.
 *
 * @param testGroup  The group this test belongs to.
 * @param testName   Name of the test.
 *
 * @see TEST, EXPECT_FAIL_TEST
 */
#define IGNORE_TEST(testGroup, testName)                                       \
  /* External declarations for strict compilers */                             \
  class IGNORE##testGroup##_##testName##_TestShell;                            \
  extern IGNORE##testGroup##_##testName##_TestShell                            \
      IGNORE##testGroup##_##testName##_TestShell_instance;                     \
                                                                               \
  class IGNORE##testGroup##_##testName##_Test                                  \
    : public TEST_GROUP_##MutinyGroup##testGroup                               \
  {                                                                            \
  public:                                                                      \
    IGNORE##testGroup##_##testName##_Test()                                    \
      : TEST_GROUP_##MutinyGroup##testGroup()                                  \
    {                                                                          \
    }                                                                          \
                                                                               \
  public:                                                                      \
    void test_body() override;                                                 \
  };                                                                           \
  class IGNORE##testGroup##_##testName##_TestShell                             \
    : public mu::tiny::test::IgnoredTestShell                                  \
  {                                                                            \
    virtual mu::tiny::test::Test* create_test() override                       \
    {                                                                          \
      return new IGNORE##testGroup##_##testName##_Test;                        \
    }                                                                          \
  } IGNORE##testGroup##_##testName##_TestShell_instance;                       \
  namespace {                                                                  \
  mu::tiny::test::TestInstaller TEST_##testGroup##testName##_Installer(        \
      IGNORE##testGroup##_##testName##_TestShell_instance,                     \
      #testGroup,                                                              \
      #testName,                                                               \
      __FILE__,                                                                \
      __LINE__                                                                 \
  );                                                                           \
  } /* namespace */                                                            \
  void IGNORE##testGroup##_##testName##_Test::test_body()

/**
 * @brief Define a test that is expected to fail.
 *
 * If the test body passes (no assertion failure), the outer run reports an
 * error. Use this to document known failures that must stay failing — the
 * test run breaks immediately if the code is accidentally fixed without
 * removing the EXPECT_FAIL_TEST declaration.
 *
 * @param testGroup  The group this test belongs to.
 * @param testName   Name of the test.
 *
 * @see TEST, IGNORE_TEST
 */
#define EXPECT_FAIL_TEST(testGroup, testName)                                  \
  /* External declarations for strict compilers */                             \
  class EXPECT_FAIL##testGroup##_##testName##_TestShell;                       \
  extern EXPECT_FAIL##testGroup##_##testName##_TestShell                       \
      EXPECT_FAIL##testGroup##_##testName##_TestShell_instance;                \
                                                                               \
  class EXPECT_FAIL##testGroup##_##testName##_Test                             \
    : public TEST_GROUP_##MutinyGroup##testGroup                               \
  {                                                                            \
  public:                                                                      \
    EXPECT_FAIL##testGroup##_##testName##_Test()                               \
      : TEST_GROUP_##MutinyGroup##testGroup()                                  \
    {                                                                          \
    }                                                                          \
    void test_body() override;                                                 \
  };                                                                           \
  class EXPECT_FAIL##testGroup##_##testName##_TestShell                        \
    : public mu::tiny::test::ExpectFailTestShell                               \
  {                                                                            \
    virtual mu::tiny::test::Test* create_test() override                       \
    {                                                                          \
      return new EXPECT_FAIL##testGroup##_##testName##_Test;                   \
    }                                                                          \
  } EXPECT_FAIL##testGroup##_##testName##_TestShell_instance;                  \
  namespace {                                                                  \
  mu::tiny::test::                                                             \
      TestInstaller EXPECT_FAIL##testGroup##_##testName##_Installer(           \
          EXPECT_FAIL##testGroup##_##testName##_TestShell_instance,            \
          #testGroup,                                                          \
          #testName,                                                           \
          __FILE__,                                                            \
          __LINE__                                                             \
      );                                                                       \
  } /* namespace */                                                            \
  void EXPECT_FAIL##testGroup##_##testName##_Test::test_body()

/**
 * @brief Attach a key/value property to the currently running test.
 *
 * Properties appear in JUnit XML output inside the @c \<properties\> element,
 * making it easy to annotate tests with environment data or feature flags.
 *
 * @param name   Property name string literal or const char*.
 * @param value  Property value string literal or const char*.
 */
#define TEST_PROPERTY(name, value)                                             \
  mu::tiny::test::TestShell::get_current()->add_test_property((name), (value))

/**
 * @brief Declare a test group that bridges C setup/teardown functions.
 *
 * Equivalent to @ref TEST_GROUP but additionally declares the extern "C"
 * setup and teardown wrapper symbols expected by
 * @ref TEST_GROUP_C_SETUP_WRAPPER and @ref TEST_GROUP_C_TEARDOWN_WRAPPER.
 *
 * @param group_name  Name of the group (also forms the C wrapper symbol names).
 *
 * @see TEST_GROUP_C_SETUP_WRAPPER, TEST_GROUP_C_TEARDOWN_WRAPPER,
 * TEST_C_WRAPPER
 */
#define TEST_GROUP_C_WRAPPER(group_name)                                       \
  extern "C" void group_##group_name##_setup_wrapper_c(void);                  \
  extern "C" void group_##group_name##_teardown_wrapper_c(void);               \
  TEST_GROUP(group_name)

/**
 * @brief Define the C setup wrapper for a group declared with
 * TEST_GROUP_C_WRAPPER.
 *
 * The body you write delegates to the C function
 * @c group_##group_name##_setup_wrapper_c, which is defined in the .c file.
 *
 * @param group_name  Group name, must match the TEST_GROUP_C_WRAPPER
 * declaration.
 *
 * @see TEST_GROUP_C_WRAPPER
 */
#define TEST_GROUP_C_SETUP_WRAPPER(group_name)                                 \
  void setup() override                                                        \
  {                                                                            \
    group_##group_name##_setup_wrapper_c();                                    \
  }

/**
 * @brief Define the C teardown wrapper for a group declared with
 * TEST_GROUP_C_WRAPPER.
 *
 * @param group_name  Group name, must match the TEST_GROUP_C_WRAPPER
 * declaration.
 *
 * @see TEST_GROUP_C_WRAPPER
 */
#define TEST_GROUP_C_TEARDOWN_WRAPPER(group_name)                              \
  void teardown() override                                                     \
  {                                                                            \
    group_##group_name##_teardown_wrapper_c();                                 \
  }

/**
 * @brief Bridge a C test function into a TEST.
 *
 * Creates a TEST whose body calls the C function
 * @c test_##group_name##_##test_name##_wrapper_c. The corresponding C-side
 * definition uses TEST() from mutiny/test.h in the .c file.
 *
 * @param group_name  Test group.
 * @param test_name   Test name.
 *
 * @see TEST_GROUP_C_WRAPPER, IGNORE_TEST_C_WRAPPER, EXPECT_FAIL_TEST_C_WRAPPER
 */
#define TEST_C_WRAPPER(group_name, test_name)                                  \
  extern "C" void test_##group_name##_##test_name##_wrapper_c();               \
  TEST(group_name, test_name)                                                  \
  {                                                                            \
    test_##group_name##_##test_name##_wrapper_c();                             \
  }

/**
 * @brief Bridge a C IGNORE_TEST into an IGNORE_TEST.
 *
 * @param group_name  Test group.
 * @param test_name   Test name.
 *
 * @see TEST_C_WRAPPER
 */
#define IGNORE_TEST_C_WRAPPER(group_name, test_name)                           \
  extern "C" void ignore_##group_name##_##test_name##_wrapper_c();             \
  IGNORE_TEST(group_name, test_name)                                           \
  {                                                                            \
    ignore_##group_name##_##test_name##_wrapper_c();                           \
  }

/**
 * @brief Bridge a C EXPECT_FAIL_TEST into an EXPECT_FAIL_TEST.
 *
 * @param group_name  Test group.
 * @param test_name   Test name.
 *
 * @see TEST_C_WRAPPER
 */
#define EXPECT_FAIL_TEST_C_WRAPPER(group_name, test_name)                      \
  extern "C" void expect_fail_##group_name##_##test_name##_wrapper_c();        \
  EXPECT_FAIL_TEST(group_name, test_name)                                      \
  {                                                                            \
    expect_fail_##group_name##_##test_name##_wrapper_c();                      \
  }

#endif
