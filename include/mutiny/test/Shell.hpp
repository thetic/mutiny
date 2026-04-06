/**
 * @file
 * @brief Assertion macros and the Shell base class.
 *
 * This header is the primary source of assertion macros (@ref CHECK, @ref
 * CHECK_EQUAL, @ref STRCMP_EQUAL, @ref DOUBLES_EQUAL, etc.) and the @ref
 * mu::tiny::test::Shell class that backs them. It is included transitively by
 * @ref mutiny/test.hpp; you rarely need to include it directly.
 */

#ifndef INCLUDED_MUTINY_TEST_SHELL_HPP
#define INCLUDED_MUTINY_TEST_SHELL_HPP

#include "mutiny/test/Terminator.hpp"

#include "mutiny/String.hpp"
#include "mutiny/export.h"
#include "mutiny/features.hpp"

#include <stdint.h>

namespace mu {
namespace tiny {
namespace test {

class Result;
class Plugin;
class Failure;
class Filter;

/**
 * @brief Returns true if @p d1 and @p d2 differ by at most @p threshold.
 *
 * Used by the @ref DOUBLES_EQUAL macro. Handles NaN correctly: if either
 * operand is NaN the function returns false.
 *
 * @param d1         First value.
 * @param d2         Second value.
 * @param threshold  Maximum allowed absolute difference (must be >= 0).
 * @return true if |d1 - d2| <= threshold.
 */
MUTINY_EXPORT bool doubles_equal(double d1, double d2, double threshold);

/**
 * @brief Shell for a single test — tracks metadata and drives execution.
 *
 * Each @ref TEST() macro instantiates a Shell subclass and registers it with
 * the @ref Registry at static initialisation time. The test runner then calls
 * @ref run_one_test() for each registered shell.
 *
 * Users interact with Shell primarily through the assertion macros
 * (@ref CHECK, @ref CHECK_EQUAL, @ref FAIL, etc.), which forward to the
 * assert_* virtual methods. This design allows the testing framework's own
 * tests to substitute a mock shell and verify assertion behaviour.
 *
 * The static methods control global runner state: the currently running test,
 * the active @ref Terminator, and crash-on-failure mode.
 */
class MUTINY_EXPORT Shell
{
public:
  /** @return The Shell currently executing, or nullptr between tests. */
  static Shell* get_current();

  /** @return The active test terminator (throws FailedException by default). */
  static const Terminator& get_current_test_terminator();

  /**
   * @return The active test terminator that never throws (used internally
   *         when exception support is disabled or inside CHECK_THROWS).
   */
  static const Terminator& get_current_test_terminator_without_exceptions();

  /**
   * @brief Make the process crash (SIGABRT) instead of throwing on failure.
   *
   * Useful when running under a debugger: the crash drops you straight into
   * the failing assertion with the full call stack.
   */
  static void set_crash_on_fail();

  /** @brief Restore the default (exception-based) Terminator. */
  static void restore_default_test_terminator();

  /**
   * @brief Control whether CHECK_THROWS re-throws unexpected exceptions.
   *
   * When @p rethrow_exceptions is true, an exception of the wrong type caught
   * inside CHECK_THROWS is re-thrown rather than recorded as a failure. This
   * makes it easier to diagnose unexpected crashes during test development.
   *
   * @param rethrow_exceptions  true to propagate unexpected exceptions.
   */
  static void set_rethrow_exceptions(bool rethrow_exceptions);

  /** @return true if unexpected exceptions are currently re-thrown. */
  static bool is_rethrowing_exceptions();

public:
  /**
   * @brief Construct a Shell with source-location metadata.
   *
   * Called by the @ref TEST() macro; users do not construct Shells directly.
   *
   * @param group_name   Name of the test group.
   * @param test_name    Name of the individual test.
   * @param file_name    Source file path (__FILE__).
   * @param line_number  Source line number (__LINE__).
   */
  Shell(
      const char* group_name,
      const char* test_name,
      const char* file_name,
      size_t line_number
  ) noexcept;
  virtual ~Shell() = default;

  /** @brief Append @p test to this shell's linked list. @return @p test. */
  virtual Shell* add_test(Shell* test);
  /** @return The next Shell in the registered list, or nullptr. */
  virtual Shell* get_next() const;
  /** @return Total number of test shells reachable from this node. */
  virtual size_t count_tests();

  /**
   * @brief Decide whether this test should run given the active filters.
   *
   * @param group_filters  Filters applied to the group name (may be nullptr).
   * @param name_filters   Filters applied to the test name (may be nullptr).
   * @return true if the test should be executed.
   */
  bool should_run(
      const Filter* group_filters,
      const Filter* name_filters
  ) const;
  /** @return The test name string. */
  const char* get_name() const;
  /** @return The group name string. */
  const char* get_group() const;
  /** @return A formatted "group::name" string for display. */
  virtual String get_formatted_name() const;
  /** @return The source file path for this test. */
  const char* get_file() const;
  /** @return The source line number for this test. */
  size_t get_line_number() const;
  /** @return true if the test will actually run (not ignored). */
  virtual bool will_run() const;
  /** @return true if the test has recorded at least one failure. */
  virtual bool has_failed() const;
  /** @return true if this is an ordered test. */
  virtual bool is_ordered() const;
  /** @brief Increment the assertion-check count in the active Result. */
  void count_check();

  /** @brief Macro backend: assert @p condition is true. */
  virtual void assert_true(
      bool condition,
      const char* check_string,
      const char* condition_string,
      const char* text,
      const char* file_name,
      size_t line_number,
      const Terminator& test_terminator = get_current_test_terminator()
  );
  /** @brief Macro backend: assert two C strings are equal (strcmp). */
  virtual void assert_cstr_equal(
      const char* expected,
      const char* actual,
      const char* text,
      const char* file_name,
      size_t line_number,
      const Terminator& test_terminator = get_current_test_terminator()
  );
  /** @brief Macro backend: assert first @p length bytes of two C strings match.
   */
  virtual void assert_cstr_n_equal(
      const char* expected,
      const char* actual,
      size_t length,
      const char* text,
      const char* file_name,
      size_t line_number,
      const Terminator& test_terminator = get_current_test_terminator()
  );
  /** @brief Macro backend: assert @p actual contains the substring @p expected.
   */
  virtual void assert_cstr_contains(
      const char* expected,
      const char* actual,
      const char* text,
      const char* file_name,
      size_t line_number
  );
  /**
   * @brief C-interface backend: assert two signed integer values are equal.
   *
   * Both operands have been widened to @c long @c long (the same range as
   * @c intmax_t on all supported platforms) by the caller.
   */
  virtual void assert_intmax_equal(
      intmax_t expected,
      intmax_t actual,
      const char* text,
      const char* file_name,
      size_t line_number,
      const Terminator& test_terminator = get_current_test_terminator()
  );
  /**
   * @brief C-interface backend: assert two unsigned integer values are equal.
   *
   * Both operands have been widened to @c unsigned @c long @c long (the same
   * range as @c uintmax_t on all supported platforms) by the caller.
   */
  virtual void assert_uintmax_equal(
      uintmax_t expected,
      uintmax_t actual,
      const char* text,
      const char* file_name,
      size_t line_number,
      const Terminator& test_terminator = get_current_test_terminator()
  );
  /** @brief Macro backend: assert two pointers are equal. */
  virtual void assert_pointers_equal(
      const void* expected,
      const void* actual,
      const char* text,
      const char* file_name,
      size_t line_number,
      const Terminator& test_terminator = get_current_test_terminator()
  );
  /** @brief Macro backend: assert two function pointers are equal. */
  virtual void assert_function_pointers_equal(
      void (*expected)(),
      void (*actual)(),
      const char* text,
      const char* file_name,
      size_t line_number,
      const Terminator& test_terminator = get_current_test_terminator()
  );
  /** @brief Macro backend: assert two doubles are equal within @p threshold. */
  virtual void assert_doubles_equal(
      double expected,
      double actual,
      double threshold,
      const char* text,
      const char* file_name,
      size_t line_number,
      const Terminator& test_terminator = get_current_test_terminator()
  );
  /** @brief Macro backend: generic equality failure with pre-formatted strings.
   */
  virtual void assert_equals(
      bool failed,
      const char* expected,
      const char* actual,
      const char* text,
      const char* file,
      size_t line_number,
      const Terminator& test_terminator = get_current_test_terminator()
  );
  /** @brief Macro backend: generic equality failure with String arguments. */
  virtual void assert_equals(
      bool failed,
      String expected,
      String actual,
      const char* text,
      const char* file,
      size_t line_number,
      const Terminator& test_terminator = get_current_test_terminator()
  );
  /** @brief Macro backend: assert @p length bytes of two memory regions match.
   */
  virtual void assert_binary_equal(
      const void* expected,
      const void* actual,
      size_t length,
      const char* text,
      const char* file_name,
      size_t line_number,
      const Terminator& test_terminator = get_current_test_terminator()
  );
  /** @brief Macro backend: assert a relational comparison holds. */
  virtual void assert_compare(
      bool comparison,
      const char* check_string,
      const char* comparison_string,
      const char* text,
      const char* file_name,
      size_t line_number,
      const Terminator& test_terminator = get_current_test_terminator()
  );
  /**
   * @brief Unconditionally fail the test with a message.
   *
   * @param text        Human-readable failure reason.
   * @param file_name   Source file (__FILE__).
   * @param line_number Source line (__LINE__).
   * @param test_terminator  Controls how the test is aborted after the failure.
   */
  virtual void fail(
      const char* text,
      const char* file_name,
      size_t line_number,
      const Terminator& test_terminator = get_current_test_terminator()
  );
  /**
   * @brief Exit the test body immediately without marking it as failed.
   *
   * @param test_terminator  Controls how control leaves the test body.
   */
  virtual void exit_test(
      const Terminator& test_terminator = get_current_test_terminator()
  );

  /** @brief Print a message only when verbose output is active. */
  virtual void print_very_verbose(const char* text);

  /** @brief Update the source file stored in this shell (used by ordered
   * tests). */
  void set_file_name(const char* file_name);
  /** @brief Update the source line stored in this shell. */
  void set_line_number(size_t line_number);
  /** @brief Update the group name stored in this shell. */
  void set_group_name(const char* group_name);
  /** @brief Update the test name stored in this shell. */
  void set_test_name(const char* test_name);

  /** @brief Invoke the registered crash function (default: abort). */
  static void crash();
  /**
   * @brief Replace the function called by crash().
   *
   * @param crashme  Function to call when a crash is requested; must not
   * return.
   */
  static void set_crash_method(void (*crashme)());
  /** @brief Restore the default crash function. */
  static void reset_crash_method();

  /** @brief Mark this test to run even though it is an ignored test. */
  virtual void set_run_ignored();

  /** @brief Allocate the Test object for this shell's test group. */
  virtual class Test* create_test();
  /** @brief Destroy a Test object previously returned by create_test(). */
  virtual void destroy_test(class Test* test);

  /** @brief Run this test (create → setup → body → teardown → destroy). */
  virtual void run_one_test(Plugin* plugin, Result& result);
  /** @brief Run this test in the current process (no forking). */
  virtual void run_one_test_in_current_process(Plugin* plugin, Result& result);

  /** @brief Record a test failure into the active Result. */
  virtual void add_failure(const Failure& failure);
  /**
   * @brief Attach a key/value property to this test.
   *
   * Properties appear in JUnit XML output. Prefer the @ref TEST_PROPERTY macro.
   *
   * @param name   Property name.
   * @param value  Property value.
   */
  virtual void add_test_property(const char* name, const char* value);

protected:
  /** @brief Default constructor for use by subclasses (e.g. IgnoredShell).
   */
  Shell() noexcept;

  /** @return The macro keyword used in formatted output (e.g. "TEST"). */
  virtual String get_macro_name() const;
  /** @return The Result for the current run. */
  Result* get_test_result();

private:
  const char* group_;
  const char* name_;
  const char* file_;
  size_t line_number_;
  Shell* next_;
  bool has_failed_;

  void set_test_result(Result* result);
  void set_current_test(Shell* test);
  bool match(const char* target, const Filter* filters) const;

  static Shell* current_test_;
  static Result* test_result_;

  static const Terminator* current_test_terminator_;
  static const Terminator* current_test_terminator_without_exceptions_;
  static bool rethrow_exceptions_;
};

/**
 * @brief Thrown (or longjmp'd) when a test assertion fails.
 *
 * The test runner catches this to terminate the current test body while still
 * allowing teardown() to run. Users rarely interact with this class directly;
 * it is part of the Terminator mechanism.
 */
class FailedException
{
public:
  int dummy;
};

/**
 * @brief Implementation helper for CHECK_EQUAL.
 *
 * Uses the conditional expression's arithmetic-conversion rules to select a
 * common comparison type, avoiding signed/unsigned mismatch warnings. Prefer
 * the CHECK_EQUAL macro.
 *
 * @param expected  Expected value.
 * @param actual    Actual value.
 * @param text      Optional failure message.
 * @param file      Source file path.
 * @param line      Source line number.
 */
template<typename T, typename U>
void check_equal(
    const T& expected,
    const U& actual,
    const char* text,
    const char* file,
    size_t line
)
{
  // Compare with the natural types so that mixed signed/unsigned comparisons
  // produce the same compiler diagnostic they would outside the macro.
  if (expected != actual) {
    Shell::get_current()->assert_equals(
        true,
        string_from(expected).c_str(),
        string_from(actual).c_str(),
        text,
        file,
        line
    );
  } else {
    Shell::get_current()->count_check();
  }
}

/**
 * @brief Implementation helper for CHECK_COMPARE.
 *
 * The boolean result of the relational operation is pre-evaluated at the
 * call site (inside the macro), so @p first and @p second are used only for
 * display via string_from(). Prefer the CHECK_COMPARE macro.
 *
 * @param first      Left-hand operand (display only).
 * @param second     Right-hand operand (display only).
 * @param success    Pre-evaluated result of @p first relop @p second.
 * @param relop_str  String representation of the relational operator.
 * @param text       Optional failure message.
 * @param file       Source file path.
 * @param line       Source line number.
 */
template<typename T, typename U>
void check_compare(
    const T& first,
    const U& second,
    bool success,
    const char* relop_str,
    const char* text,
    const char* file,
    size_t line
)
{
  // The bool result of the relop is pre-computed at the call site (in the
  // macro), so no sign-compare issue here. The parameters first/second are
  // used only for string_from(), not compared against each other.
  if (!success) {
    String condition;
    condition += string_from(first);
    condition += " ";
    condition += relop_str;
    condition += " ";
    condition += string_from(second);
    Shell::get_current()->assert_compare(
        false, "CHECK_COMPARE", condition.c_str(), text, file, line
    );
  } else {
    Shell::get_current()->count_check();
  }
}

/**
 * @brief Implementation helper for ENUMS_EQUAL_TYPE.
 *
 * Casts both enum values to @p UNDERLYING_TYPE before comparing and
 * formatting. Prefer the ENUMS_EQUAL_TYPE or ENUMS_EQUAL_INT macros.
 *
 * @tparam UNDERLYING_TYPE  Integer type used for display (e.g. @c int).
 * @tparam ENUM_TYPE        Enum type being compared.
 * @param expected          Expected enum value.
 * @param actual            Actual enum value.
 * @param text              Optional failure message.
 * @param file              Source file path.
 * @param line              Source line number.
 */
template<typename UNDERLYING_TYPE, typename ENUM_TYPE>
void check_enum_equal(
    ENUM_TYPE expected,
    ENUM_TYPE actual,
    const char* text,
    const char* file,
    size_t line
)
{
  auto e = static_cast<UNDERLYING_TYPE>(expected);
  auto a = static_cast<UNDERLYING_TYPE>(actual);
  if (e != a) {
    Shell::get_current()->assert_equals(
        true, string_from(e).c_str(), string_from(a).c_str(), text, file, line
    );
  } else {
    Shell::get_current()->count_check();
  }
}

} // namespace test
} // namespace tiny
} // namespace mu

// Different checking macros

/**
 * @brief Fail the current test if @p condition is false.
 *
 * Equivalent to CHECK_TRUE. Prefer CHECK when the condition is a plain
 * boolean; use CHECK_TRUE_TEXT when you also want to supply a message.
 *
 * @param condition  Any expression convertible to bool.
 *
 * See also @ref CHECK_TEXT, @ref CHECK_TRUE, and @ref CHECK_FALSE.
 */
#define CHECK(condition)                                                       \
  CHECK_TRUE_LOCATION(condition, "CHECK", #condition, "", __FILE__, __LINE__)

/** @brief @ref CHECK with a custom failure message. */
#define CHECK_TEXT(condition, text)                                            \
  CHECK_TRUE_LOCATION(                                                         \
      static_cast<bool>(condition),                                            \
      "CHECK",                                                                 \
      #condition,                                                              \
      text,                                                                    \
      __FILE__,                                                                \
      __LINE__                                                                 \
  )

/** @brief Fail if @p condition is false (explicit TRUE variant of @ref CHECK).
 */
#define CHECK_TRUE(condition)                                                  \
  CHECK_TRUE_LOCATION(                                                         \
      static_cast<bool>(condition),                                            \
      "CHECK_TRUE",                                                            \
      #condition,                                                              \
      "",                                                                      \
      __FILE__,                                                                \
      __LINE__                                                                 \
  )

/** @brief CHECK_TRUE with a custom failure message. @see CHECK_TRUE */
#define CHECK_TRUE_TEXT(condition, text)                                       \
  CHECK_TRUE_LOCATION(                                                         \
      condition, "CHECK_TRUE", #condition, text, __FILE__, __LINE__            \
  )

/** @brief Fail if @p condition is true. @see CHECK_FALSE_TEXT */
#define CHECK_FALSE(condition)                                                 \
  CHECK_FALSE_LOCATION(                                                        \
      condition, "CHECK_FALSE", #condition, "", __FILE__, __LINE__             \
  )

/** @brief CHECK_FALSE with a custom failure message. @see CHECK_FALSE */
#define CHECK_FALSE_TEXT(condition, text)                                      \
  CHECK_FALSE_LOCATION(                                                        \
      condition, "CHECK_FALSE", #condition, text, __FILE__, __LINE__           \
  )

/** @brief Location-explicit variant of CHECK. Prefer CHECK in test code. */
#define CHECK_TRUE_LOCATION(                                                   \
    condition, checkString, conditionString, text, file, line                  \
)                                                                              \
  do {                                                                         \
    mu::tiny::test::Shell::get_current()->assert_true(                         \
        (condition), checkString, conditionString, text, file, line            \
    );                                                                         \
  } while (0)

/** @brief Location-explicit variant of CHECK_FALSE. Prefer CHECK_FALSE in test
 * code. */
#define CHECK_FALSE_LOCATION(                                                  \
    condition, checkString, conditionString, text, file, line                  \
)                                                                              \
  do {                                                                         \
    mu::tiny::test::Shell::get_current()->assert_true(                         \
        !(condition), checkString, conditionString, text, file, line           \
    );                                                                         \
  } while (0)

/**
 * @brief Fail if @p expected != @p actual.
 *
 * Requires operator!=() and a string_from() overload for the operand types.
 * Uses common-type arithmetic conversion so signed/unsigned pairs are
 * promoted consistently without -Wsign-compare warnings.
 *
 * @param expected  Expected value.
 * @param actual    Actual value.
 */
#define CHECK_EQUAL(expected, actual)                                          \
  CHECK_EQUAL_LOCATION(expected, actual, "", __FILE__, __LINE__)

/** @brief CHECK_EQUAL with a custom failure message. @see CHECK_EQUAL */
#define CHECK_EQUAL_TEXT(expected, actual, text)                               \
  CHECK_EQUAL_LOCATION(expected, actual, text, __FILE__, __LINE__)

/** @brief Location-explicit variant of CHECK_EQUAL. Prefer CHECK_EQUAL in test
 * code. */
#define CHECK_EQUAL_LOCATION(expected, actual, text, file, line)               \
  mu::tiny::test::check_equal((expected), (actual), text, file, line)

/** @brief Fail if @p actual != 0. Equivalent to CHECK_EQUAL(0, actual). */
#define CHECK_EQUAL_ZERO(actual) CHECK_EQUAL(0, (actual))

/** @brief CHECK_EQUAL_ZERO with a custom failure message. @see CHECK_EQUAL_ZERO
 */
#define CHECK_EQUAL_ZERO_TEXT(actual, text)                                    \
  CHECK_EQUAL_TEXT(0, (actual), (text))

/**
 * @brief Fail if @p first relop @p second is false.
 *
 * @p relop must be a relational operator token: @c <, @c <=, @c >, @c >=,
 * @c ==, or @c !=.
 *
 * @code{.cpp}
 * CHECK_COMPARE(a, <, b);   // fails if a >= b
 * @endcode
 *
 * @param first   Left-hand operand.
 * @param relop   Relational operator.
 * @param second  Right-hand operand.
 *
 * @see CHECK_COMPARE_TEXT
 */
#define CHECK_COMPARE(first, relop, second)                                    \
  CHECK_COMPARE_TEXT(first, relop, second, "")

/** @brief CHECK_COMPARE with a custom failure message. @see CHECK_COMPARE */
#define CHECK_COMPARE_TEXT(first, relop, second, text)                         \
  CHECK_COMPARE_LOCATION(first, relop, second, text, __FILE__, __LINE__)

/** @brief Location-explicit variant of CHECK_COMPARE. Prefer CHECK_COMPARE in
 * test code. */
#define CHECK_COMPARE_LOCATION(first, relop, second, text, file, line)         \
  mu::tiny::test::check_compare(                                               \
      (first), (second), (first)relop(second), #relop, text, file, line        \
  )

/**
 * @brief Fail if the C strings @p expected and @p actual differ (strcmp).
 *
 * Use this instead of CHECK_EQUAL for @c const char* values — CHECK_EQUAL
 * compares the pointer addresses, not the string content.
 *
 * @param expected  Expected C string (may be nullptr).
 * @param actual    Actual C string (may be nullptr).
 *
 * @see STRCMP_EQUAL_TEXT, STRNCMP_EQUAL, STRCMP_CONTAINS
 */
#define STRCMP_EQUAL(expected, actual)                                         \
  STRCMP_EQUAL_LOCATION(expected, actual, "", __FILE__, __LINE__)

/** @brief STRCMP_EQUAL with a custom failure message. @see STRCMP_EQUAL */
#define STRCMP_EQUAL_TEXT(expected, actual, text)                              \
  STRCMP_EQUAL_LOCATION(expected, actual, text, __FILE__, __LINE__)

/** @brief Location-explicit variant of STRCMP_EQUAL. Prefer STRCMP_EQUAL in
 * test code. */
#define STRCMP_EQUAL_LOCATION(expected, actual, text, file, line)              \
  do {                                                                         \
    mu::tiny::test::Shell::get_current()->assert_cstr_equal(                   \
        expected, actual, text, file, line                                     \
    );                                                                         \
  } while (0)

/**
 * @brief Fail if the first @p length characters of @p expected and @p actual
 * differ.
 *
 * @param expected  Expected C string prefix.
 * @param actual    Actual C string.
 * @param length    Number of characters to compare.
 *
 * @see STRNCMP_EQUAL_TEXT
 */
#define STRNCMP_EQUAL(expected, actual, length)                                \
  STRNCMP_EQUAL_LOCATION(expected, actual, length, "", __FILE__, __LINE__)

/** @brief STRNCMP_EQUAL with a custom failure message. @see STRNCMP_EQUAL */
#define STRNCMP_EQUAL_TEXT(expected, actual, length, text)                     \
  STRNCMP_EQUAL_LOCATION(expected, actual, length, text, __FILE__, __LINE__)

/** @brief Location-explicit variant of STRNCMP_EQUAL. */
#define STRNCMP_EQUAL_LOCATION(expected, actual, length, text, file, line)     \
  do {                                                                         \
    mu::tiny::test::Shell::get_current()->assert_cstr_n_equal(                 \
        expected, actual, length, text, file, line                             \
    );                                                                         \
  } while (0)

/**
 * @brief Fail if @p actual does not contain the substring @p expected.
 *
 * @param expected  Substring that must be present.
 * @param actual    String to search within.
 *
 * @see STRCMP_CONTAINS_TEXT
 */
#define STRCMP_CONTAINS(expected, actual)                                      \
  STRCMP_CONTAINS_LOCATION(expected, actual, "", __FILE__, __LINE__)

/** @brief STRCMP_CONTAINS with a custom failure message. @see STRCMP_CONTAINS
 */
#define STRCMP_CONTAINS_TEXT(expected, actual, text)                           \
  STRCMP_CONTAINS_LOCATION(expected, actual, text, __FILE__, __LINE__)

/** @brief Location-explicit variant of STRCMP_CONTAINS. */
#define STRCMP_CONTAINS_LOCATION(expected, actual, text, file, line)           \
  do {                                                                         \
    mu::tiny::test::Shell::get_current()->assert_cstr_contains(                \
        expected, actual, text, file, line                                     \
    );                                                                         \
  } while (0)

/**
 * @brief Fail if pointer values @p expected and @p actual differ.
 *
 * Both operands are cast to @c const void* before comparison, so pointers to
 * any object type (including nullptr) can be compared. Raw integer values are
 * not accepted; use reinterpret_cast first if needed.
 *
 * @param expected  Expected pointer.
 * @param actual    Actual pointer.
 *
 * @see POINTERS_EQUAL_TEXT, FUNCTIONPOINTERS_EQUAL
 */
#define POINTERS_EQUAL(expected, actual)                                       \
  POINTERS_EQUAL_LOCATION((expected), (actual), "", __FILE__, __LINE__)

/** @brief POINTERS_EQUAL with a custom failure message. @see POINTERS_EQUAL */
#define POINTERS_EQUAL_TEXT(expected, actual, text)                            \
  POINTERS_EQUAL_LOCATION((expected), (actual), text, __FILE__, __LINE__)

/** @brief Location-explicit variant of POINTERS_EQUAL. */
#define POINTERS_EQUAL_LOCATION(expected, actual, text, file, line)            \
  do {                                                                         \
    mu::tiny::test::Shell::get_current()->assert_pointers_equal(               \
        static_cast<const void*>(expected),                                    \
        static_cast<const void*>(actual),                                      \
        text,                                                                  \
        file,                                                                  \
        line                                                                   \
    );                                                                         \
  } while (0)

/**
 * @brief Fail if function pointer values @p expected and @p actual differ.
 *
 * Both operands are cast to @c void(*)() before comparison. Pass
 * @c static_cast<void(*)()>(nullptr) to compare against a null function
 * pointer.
 *
 * @param expected  Expected function pointer.
 * @param actual    Actual function pointer.
 *
 * @see FUNCTIONPOINTERS_EQUAL_TEXT
 */
#define FUNCTIONPOINTERS_EQUAL(expected, actual)                               \
  FUNCTIONPOINTERS_EQUAL_LOCATION((expected), (actual), "", __FILE__, __LINE__)

/** @brief FUNCTIONPOINTERS_EQUAL with a custom failure message. @see
 * FUNCTIONPOINTERS_EQUAL */
#define FUNCTIONPOINTERS_EQUAL_TEXT(expected, actual, text)                    \
  FUNCTIONPOINTERS_EQUAL_LOCATION(                                             \
      (expected), (actual), text, __FILE__, __LINE__                           \
  )

/** @brief Location-explicit variant of FUNCTIONPOINTERS_EQUAL. */
#define FUNCTIONPOINTERS_EQUAL_LOCATION(expected, actual, text, file, line)    \
  do {                                                                         \
    mu::tiny::test::Shell::get_current()->assert_function_pointers_equal(      \
        reinterpret_cast<void (*)()>(expected),                                \
        reinterpret_cast<void (*)()>(actual),                                  \
        text,                                                                  \
        file,                                                                  \
        line                                                                   \
    );                                                                         \
  } while (0)

/**
 * @brief Fail if @p expected and @p actual differ by more than @p threshold.
 *
 * Handles NaN correctly: a NaN operand always fails the check.
 *
 * @param expected   Expected double value.
 * @param actual     Actual double value.
 * @param threshold  Maximum allowed absolute difference (must be >= 0).
 *
 * @see DOUBLES_EQUAL_TEXT, doubles_equal
 */
#define DOUBLES_EQUAL(expected, actual, threshold)                             \
  DOUBLES_EQUAL_LOCATION(expected, actual, threshold, "", __FILE__, __LINE__)

/** @brief DOUBLES_EQUAL with a custom failure message. @see DOUBLES_EQUAL */
#define DOUBLES_EQUAL_TEXT(expected, actual, threshold, text)                  \
  DOUBLES_EQUAL_LOCATION(expected, actual, threshold, text, __FILE__, __LINE__)

/** @brief Location-explicit variant of DOUBLES_EQUAL. */
#define DOUBLES_EQUAL_LOCATION(expected, actual, threshold, text, file, line)  \
  do {                                                                         \
    mu::tiny::test::Shell::get_current()->assert_doubles_equal(                \
        expected, actual, threshold, text, file, line                          \
    );                                                                         \
  } while (0)

/**
 * @brief Fail if @p size bytes starting at @p expected and @p actual differ.
 *
 * @param expected  Pointer to the expected memory region.
 * @param actual    Pointer to the actual memory region.
 * @param size      Number of bytes to compare.
 *
 * @see MEMCMP_EQUAL_TEXT
 */
#define MEMCMP_EQUAL(expected, actual, size)                                   \
  MEMCMP_EQUAL_LOCATION(expected, actual, size, "", __FILE__, __LINE__)

/** @brief MEMCMP_EQUAL with a custom failure message. @see MEMCMP_EQUAL */
#define MEMCMP_EQUAL_TEXT(expected, actual, size, text)                        \
  MEMCMP_EQUAL_LOCATION(expected, actual, size, text, __FILE__, __LINE__)

/** @brief Location-explicit variant of MEMCMP_EQUAL. */
#define MEMCMP_EQUAL_LOCATION(expected, actual, size, text, file, line)        \
  do {                                                                         \
    mu::tiny::test::Shell::get_current()->assert_binary_equal(                 \
        expected, actual, size, text, file, line                               \
    );                                                                         \
  } while (0)

/**
 * @brief Fail if two enum values differ, casting both to @c int for display.
 *
 * @param expected  Expected enum value.
 * @param actual    Actual enum value.
 *
 * @see ENUMS_EQUAL_TYPE
 */
#define ENUMS_EQUAL_INT(expected, actual)                                      \
  ENUMS_EQUAL_TYPE(int, expected, actual)

/** @brief ENUMS_EQUAL_INT with a custom failure message. @see ENUMS_EQUAL_INT
 */
#define ENUMS_EQUAL_INT_TEXT(expected, actual, text)                           \
  ENUMS_EQUAL_TYPE_TEXT(int, expected, actual, text)

/**
 * @brief Fail if two enum values differ, casting both to @p underlying_type for
 * display.
 *
 * Use this when the enum's underlying type is not @c int (e.g. @c unsigned
 * or @c uint8_t) to get meaningful output on failure.
 *
 * @param underlying_type  Integer type to cast enum values to for display.
 * @param expected         Expected enum value.
 * @param actual           Actual enum value.
 *
 * @see ENUMS_EQUAL_INT, ENUMS_EQUAL_TYPE_TEXT
 */
#define ENUMS_EQUAL_TYPE(underlying_type, expected, actual)                    \
  ENUMS_EQUAL_TYPE_LOCATION(                                                   \
      underlying_type, expected, actual, "", __FILE__, __LINE__                \
  )

/** @brief ENUMS_EQUAL_TYPE with a custom failure message. @see ENUMS_EQUAL_TYPE
 */
#define ENUMS_EQUAL_TYPE_TEXT(underlying_type, expected, actual, text)         \
  ENUMS_EQUAL_TYPE_LOCATION(                                                   \
      underlying_type, expected, actual, text, __FILE__, __LINE__              \
  )

/** @brief Location-explicit variant of ENUMS_EQUAL_TYPE. */
#define ENUMS_EQUAL_TYPE_LOCATION(                                             \
    underlying_type, expected, actual, text, file, line                        \
)                                                                              \
  mu::tiny::test::check_enum_equal<underlying_type>(                           \
      (expected), (actual), text, file, line                                   \
  )

/**
 * @brief Unconditionally fail the current test with a message.
 *
 * FAIL may already be defined by another library; in that case use FAIL_TEST
 * instead. Both macros behave identically.
 *
 * @param text  Human-readable failure message.
 *
 * @see FAIL_TEST, FAIL_LOCATION
 */
#ifndef FAIL
#define FAIL(text) FAIL_LOCATION(text, __FILE__, __LINE__)

/** @brief Location-explicit variant of FAIL. */
#define FAIL_LOCATION(text, file, line)                                        \
  do {                                                                         \
    mu::tiny::test::Shell::get_current()->fail(text, file, line);              \
  } while (0)
#endif

/** @brief Unconditionally fail the current test. Use when FAIL is already
 * defined. @see FAIL */
#define FAIL_TEST(text) FAIL_TEST_LOCATION(text, __FILE__, __LINE__)

/** @brief Location-explicit variant of FAIL_TEST. */
#define FAIL_TEST_LOCATION(text, file, line)                                   \
  do {                                                                         \
    mu::tiny::test::Shell::get_current()->fail(text, file, line);              \
  } while (0)

/**
 * @brief Exit the current test body immediately without marking it as failed.
 *
 * Useful when a prerequisite check fails and continuing would produce
 * confusing cascading failures. Unlike FAIL, the test is counted as passed.
 */
#define TEST_EXIT                                                              \
  do {                                                                         \
    mu::tiny::test::Shell::get_current()->exit_test();                         \
  } while (0)

#if MUTINY_HAVE_EXCEPTIONS
/**
 * @brief Fail if @p expression does not throw an exception of type @p expected.
 *
 * The test fails if @p expression throws nothing or throws an exception of a
 * different type. Only available when exceptions are enabled
 * (@ref MUTINY_HAVE_EXCEPTIONS is non-zero).
 *
 * @param expected    Exception type that must be thrown (not a string).
 * @param expression  Expression that should throw.
 */
#define CHECK_THROWS(expected, expression)                                     \
  do {                                                                         \
    mu::tiny::String failure_msg(                                              \
        "expected to throw " #expected "\nbut threw nothing"                   \
    );                                                                         \
    bool caught_expected = false;                                              \
    try {                                                                      \
      (expression);                                                            \
    } catch (const expected&) {                                                \
      caught_expected = true;                                                  \
    } catch (...) {                                                            \
      failure_msg =                                                            \
          "expected to throw " #expected "\nbut threw a different type";       \
    }                                                                          \
    if (!caught_expected) {                                                    \
      mu::tiny::test::Shell::get_current()->fail(                              \
          failure_msg.c_str(), __FILE__, __LINE__                              \
      );                                                                       \
    } else {                                                                   \
      mu::tiny::test::Shell::get_current()->count_check();                     \
    }                                                                          \
  } while (0)
#endif /* MUTINY_HAVE_EXCEPTIONS */

#endif
