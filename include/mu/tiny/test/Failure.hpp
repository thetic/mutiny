/**
 * @file
 * @brief Test failure types carrying location and message information.
 *
 * @ref mu::tiny::test::Failure is the base class; each assertion macro creates
 * an appropriate concrete subclass when the assertion does not hold. Failures
 * are reported via @ref mu::tiny::test::Shell::add_failure() and rendered by
 * @ref mu::tiny::test::Output::print_failure().
 */

#ifndef INCLUDED_MU_TINY_TEST_FAILURE_HPP
#define INCLUDED_MU_TINY_TEST_FAILURE_HPP

#include "mu/tiny/String.hpp"
#include "mu/tiny/export.h"
#include "mu/tiny/features.hpp"

#if MUTINY_USE_STD_CPP_LIB
#include <stdexcept>
#endif

namespace mu {
namespace tiny {
namespace test {

class Shell;
class Output;

/**
 * @brief Holds location and message information for a single test failure.
 *
 * Subclass @ref Failure to provide richer diagnostic messages. The assertion
 * macros construct the appropriate concrete subclass and pass it to @ref
 * Shell::add_failure().
 */
class MUTINY_EXPORT Failure
{

public:
  /**
   * @brief Construct with full location information and a message.
   *
   * @param test         The test shell in which the failure occurred.
   * @param file_name    Source file of the failing assertion.
   * @param line_number  Line number of the failing assertion.
   * @param the_message  Human-readable failure description.
   */
  Failure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      const String& the_message
  );

  /**
   * @brief Construct with a message but no assertion location (test-level
   * error).
   *
   * @param test         The test shell in which the failure occurred.
   * @param the_message  Human-readable failure description.
   */
  Failure(Shell* test, const String& the_message);

  /**
   * @brief Construct with a location but no message (location-only failure).
   *
   * @param test         The test shell in which the failure occurred.
   * @param file_name    Source file of the failing assertion.
   * @param line_number  Line number of the failing assertion.
   */
  Failure(Shell* test, const char* file_name, size_t line_number);
  Failure(const Failure&) = default;
  Failure(Failure&&) noexcept;
  virtual ~Failure() = default;

  /** @return Source file of the failing assertion. */
  virtual const String& get_file_name() const;
  /** @return Fully-qualified test name (`group.case`). */
  virtual const String& get_test_name() const;
  /** @return Test case name without the group prefix. */
  virtual const String& get_test_name_only() const;
  /** @return Line number of the failing assertion. */
  virtual size_t get_failure_line_number() const;
  /** @return Human-readable failure description. */
  virtual const String& get_message() const;
  /** @return Source file in which the test is defined. */
  virtual const String& get_test_file_name() const;
  /** @return Line number of the TEST() macro for the failing test. */
  virtual size_t get_test_line_number() const;

  /**
   * @return true if this failure represents an error (e.g. unexpected
   * exception) rather than a normal assertion failure.
   */
  virtual bool is_error() const { return false; }

  /** @return true if the failure originated outside the test's source file. */
  bool is_outside_test_file() const;
  /** @return true if the failure originated in a helper function. */
  bool is_in_helper_function() const;

protected:
  /**
   * @brief Format a `expected <X> but was <Y>` string.
   *
   * @param expected  Expected value as a string.
   * @param actual    Actual value as a string.
   * @return Formatted comparison string.
   */
  String create_but_was_string(const String& expected, const String& actual);

  /**
   * @brief Format a `difference starts at position N` string.
   *
   * @param actual             Actual string value.
   * @param offset             Byte offset where the difference starts.
   * @param reported_position  1-based position to report to the user.
   * @return Formatted difference-position string.
   */
  String create_difference_at_pos_string(
      const String& actual,
      size_t offset,
      size_t reported_position
  );

  /**
   * @brief Wrap a user-supplied text string for inclusion in a failure
   * message.
   *
   * @param text  User text from the `_TEXT` variant of an assertion.
   * @return Formatted user-text string, or empty if @p text is empty.
   */
  String create_user_text(const String& text);

  String test_name_;
  String test_name_only_;
  String file_name_;
  size_t line_number_;
  String test_file_name_;
  size_t test_line_number_;
  String message_;

  Failure& operator=(const Failure&);
};

/**
 * @brief Failure for @ref CHECK_EQUAL when two values compare unequal.
 */
class MUTINY_EXPORT EqualsFailure : public Failure
{
public:
  /**
   * @param test         The failing test.
   * @param file_name    Source file of the assertion.
   * @param line_number  Line of the assertion.
   * @param expected     Expected value (C string).
   * @param actual       Actual value (C string).
   * @param text         Optional user text from the `_TEXT` variant.
   */
  EqualsFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      const char* expected,
      const char* actual,
      const String& text
  );
  /**
   * @param test         The failing test.
   * @param file_name    Source file of the assertion.
   * @param line_number  Line of the assertion.
   * @param expected     Expected value (String).
   * @param actual       Actual value (String).
   * @param text         Optional user text from the `_TEXT` variant.
   */
  EqualsFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      const String& expected,
      const String& actual,
      const String& text
  );
};

/**
 * @brief Failure for @ref CHECK_APPROX when two values differ by more than
 * the threshold.
 *
 * @tparam T  Numeric type of the values (@c int, @c float, @c double, etc.).
 *            Values are formatted via @ref mu::tiny::string_from() and NaN is
 *            detected with the IEEE 754 self-comparison trick (@c v!=v), the
 *            same approach used by @ref mu::tiny::test::approx_equal.
 */
template<typename T>
class ApproxEqualFailure : public Failure
{
public:
  /**
   * @param test         The failing test.
   * @param file_name    Source file of the assertion.
   * @param line_number  Line of the assertion.
   * @param expected     Expected value.
   * @param actual       Actual value.
   * @param threshold    Allowed tolerance.
   * @param text         Optional user text.
   */
  ApproxEqualFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      T expected,
      T actual,
      T threshold,
      const String& text
  )
    : Failure(test, file_name, line_number)
  {
    message_ = create_user_text(text);
    message_ +=
        create_but_was_string(string_from(expected), string_from(actual));
    message_ += " threshold used was <";
    message_ += string_from(threshold);
    message_ += ">";
    // v != v is true iff v is NaN (IEEE 754); always false for integral T
    if (expected != expected || actual != actual || threshold != threshold)
      message_ += "\n\tCannot make comparisons with Nan";
  }
};

/**
 * @brief Failure for generic @ref CHECK_EQUAL on arbitrary types.
 */
class MUTINY_EXPORT CheckEqualFailure : public Failure
{
public:
  /**
   * @param test         The failing test.
   * @param file_name    Source file of the assertion.
   * @param line_number  Line of the assertion.
   * @param expected     String representation of the expected value.
   * @param actual       String representation of the actual value.
   * @param text         Optional user text.
   */
  CheckEqualFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      const String& expected,
      const String& actual,
      const String& text
  );
};

/**
 * @brief Failure for @ref CHECK_COMPARE when a relational comparison fails.
 */
class MUTINY_EXPORT ComparisonFailure : public Failure
{
public:
  /**
   * @param test              The failing test.
   * @param file_name         Source file of the assertion.
   * @param line_number       Line of the assertion.
   * @param check_string      The CHECK expression string.
   * @param comparison_string The comparison result description.
   * @param text              Optional user text.
   */
  ComparisonFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      const String& check_string,
      const String& comparison_string,
      const String& text
  );
};

/**
 * @brief Failure for @ref STRCMP_CONTAINS when a string does not contain the
 * expected substring.
 */
class MUTINY_EXPORT ContainsFailure : public Failure
{
public:
  /**
   * @param test         The failing test.
   * @param file_name    Source file of the assertion.
   * @param line_number  Line of the assertion.
   * @param expected     Expected substring.
   * @param actual       Actual string that was searched.
   * @param text         Optional user text.
   */
  ContainsFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      const String& expected,
      const String& actual,
      const String& text
  );
};

/**
 * @brief Failure for @ref CHECK.
 */
class MUTINY_EXPORT CheckFailure : public Failure
{
public:
  /**
   * @param test              The failing test.
   * @param file_name         Source file of the assertion.
   * @param line_number       Line of the assertion.
   * @param check_string      The CHECK expression string.
   * @param condition_string  The evaluated condition.
   * @param text_string       Optional user text.
   */
  CheckFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      const String& check_string,
      const String& condition_string,
      const String& text_string = ""
  );
};

/**
 * @brief Failure for @ref FAIL_TEST.
 */
class MUTINY_EXPORT FailFailure : public Failure
{
public:
  /**
   * @param test         The failing test.
   * @param file_name    Source file of the assertion.
   * @param line_number  Line of the assertion.
   * @param message      User-supplied failure message.
   */
  FailFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      const String& message
  );
};

/**
 * @brief Failure for C integer equality checks (signed, any width).
 *
 * Used by the C-interface wrappers for @c bool, @c int, @c long, and @c long
 * @c long; values are widened to @c intmax_t before the comparison so that a
 * single failure class covers all signed widths.
 */
class MUTINY_EXPORT IntMaxEqualFailure : public Failure
{
public:
  /**
   * @param test         The failing test.
   * @param file_name    Source file.
   * @param line_number  Line number.
   * @param expected     Expected value (widened to @c intmax_t).
   * @param actual       Actual value (widened to @c intmax_t).
   * @param text         Optional user text.
   */
  IntMaxEqualFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      long long expected,
      long long actual,
      const String& text
  );
};

/**
 * @brief Failure for C integer equality checks (unsigned, any width).
 *
 * Used by the C-interface wrappers for @c unsigned @c int, @c unsigned @c
 * long, and @c unsigned @c long @c long; values are widened to @c uintmax_t.
 */
class MUTINY_EXPORT UintMaxEqualFailure : public Failure
{
public:
  /**
   * @param test         The failing test.
   * @param file_name    Source file.
   * @param line_number  Line number.
   * @param expected     Expected value (widened to @c uintmax_t).
   * @param actual       Actual value (widened to @c uintmax_t).
   * @param text         Optional user text.
   */
  UintMaxEqualFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      unsigned long long expected,
      unsigned long long actual,
      const String& text
  );
};

/**
 * @brief Failure for @ref STRCMP_EQUAL.
 */
class MUTINY_EXPORT StringEqualFailure : public Failure
{
public:
  /**
   * @param test         The failing test.
   * @param file_name    Source file.
   * @param line_number  Line number.
   * @param expected     Expected C string.
   * @param actual       Actual C string.
   * @param text         Optional user text.
   */
  StringEqualFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      const char* expected,
      const char* actual,
      const String& text
  );
};

/**
 * @brief Failure for case-insensitive string comparison.
 */
class MUTINY_EXPORT StringEqualNoCaseFailure : public Failure
{
public:
  /**
   * @param test         The failing test.
   * @param file_name    Source file.
   * @param line_number  Line number.
   * @param expected     Expected C string.
   * @param actual       Actual C string.
   * @param text         Optional user text.
   */
  StringEqualNoCaseFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      const char* expected,
      const char* actual,
      const String& text
  );
};

/**
 * @brief Failure for @ref MEMCMP_EQUAL.
 */
class MUTINY_EXPORT BinaryEqualFailure : public Failure
{
public:
  /**
   * @param test         The failing test.
   * @param file_name    Source file.
   * @param line_number  Line number.
   * @param expected     Expected byte buffer.
   * @param actual       Actual byte buffer.
   * @param size         Number of bytes compared.
   * @param text         Optional user text.
   */
  BinaryEqualFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      const unsigned char* expected,
      const unsigned char* actual,
      size_t size,
      const String& text
  );
};

/**
 * @brief Failure reported when a required feature is unavailable.
 */
class MUTINY_EXPORT FeatureUnsupportedFailure : public Failure
{
public:
  /**
   * @param test         The failing test.
   * @param file_name    Source file.
   * @param line_number  Line number.
   * @param feature_name Name of the unsupported feature.
   * @param text         Optional user text.
   */
  FeatureUnsupportedFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      const String& feature_name,
      const String& text
  );
};

#if MUTINY_HAVE_EXCEPTIONS
/**
 * @brief Failure reported when a test body throws an unexpected exception.
 *
 * @ref is_error() returns true for this subclass because it represents an
 * unhandled exception rather than a deliberate assertion.
 *
 * Only available when @ref MUTINY_HAVE_EXCEPTIONS is 1.
 */
class MUTINY_EXPORT UnexpectedExceptionFailure : public Failure
{
public:
  /** @return true (this is an error, not a deliberate assertion failure). */
  bool is_error() const override { return true; }
  /**
   * @brief Construct for an exception of unknown type.
   * @param test  The failing test.
   */
  UnexpectedExceptionFailure(Shell* test);
#if MUTINY_USE_STD_CPP_LIB
  /**
   * @brief Construct with the `std::exception` message.
   * @param test  The failing test.
   * @param e     The caught exception.
   */
  UnexpectedExceptionFailure(Shell* test, const std::exception& e);
#endif
};
#endif

} // namespace test
} // namespace tiny
} // namespace mu

#endif
