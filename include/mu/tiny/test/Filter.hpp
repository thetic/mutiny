/**
 * @file
 * @brief Test name/group filter used by the command-line runner.
 *
 * Built from `-g` / `-n` arguments by @ref
 * mu::tiny::test::CommandLineArguments and applied via @ref
 * mu::tiny::test::Registry::set_name_filters() and @ref
 * mu::tiny::test::Registry::set_group_filters().
 */

#ifndef INCLUDED_MU_TINY_TEST_FILTER_HPP
#define INCLUDED_MU_TINY_TEST_FILTER_HPP

#include "mu/tiny/String.hpp"
#include "mu/tiny/export.h"

namespace mu {
namespace tiny {
namespace test {

/**
 * @brief A singly-linked list node representing one test name or group filter.
 *
 * Filters are built from the `-g` / `-n` command-line flags by
 * @ref CommandLineArguments and applied by @ref Registry before running each
 * test. By default matching is case-insensitive substring; use
 * @ref strict_matching() for exact equality or @ref invert_matching() to
 * exclude matching tests.
 */
class MUTINY_EXPORT Filter
{
public:
  /** @brief Construct an empty (match-all) filter. */
  Filter();
  /**
   * @brief Construct a filter from a C string pattern.
   * @param filter  Pattern to match against test names or group names.
   */
  Filter(const char* filter);
  /**
   * @brief Construct a filter from a String pattern.
   * @param filter  Pattern to match against test names or group names.
   */
  Filter(const String& filter);

  /**
   * @brief Append @p filter to the end of this filter chain.
   * @param filter  Filter to add.
   * @return @p filter.
   */
  Filter* add(Filter* filter);

  /** @return The next filter in the chain, or nullptr. */
  Filter* get_next() const;

  /**
   * @brief Test whether @p name satisfies this filter.
   * @param name  Test or group name to test.
   * @return true if the name matches (or the filter is empty).
   */
  bool match(const String& name) const;

  /** @brief Enable strict (exact) matching for this filter. */
  void strict_matching();
  /** @brief Invert this filter so matching names are excluded. */
  void invert_matching();

  /** @return true if both filters have the same pattern and mode. */
  bool operator==(const Filter& filter) const;
  /** @return true if the filters differ. */
  bool operator!=(const Filter& filter) const;

  /** @return The filter pattern as a String. */
  String as_string() const;

private:
  String filter_;
  bool strict_matching_;
  bool invert_matching_;
  Filter* next_;
};

} // namespace test

/**
 * @brief Convert a Filter to its string representation.
 * @param filter  Filter to convert.
 * @return The filter pattern string.
 */
MUTINY_EXPORT String string_from(const test::Filter& filter);

} // namespace tiny
} // namespace mu

#endif
