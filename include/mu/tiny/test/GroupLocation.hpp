/**
 * @file
 * @brief Source-location record for a TEST_GROUP() declaration.
 *
 * Each @ref TEST_GROUP() macro instantiates one GroupLocation at static
 * initialisation time, capturing the file path and line number of the group
 * declaration. The instances form a singly-linked list accessible through
 * @ref mu::tiny::test::GroupLocation::get_head(). Used by the command-line
 * runner to implement the `-lgl` listing mode, which lets the cmake discovery
 * script supply accurate
 * @c DEF_SOURCE_LINE properties for group-level CTest entries.
 */

#ifndef INCLUDED_MU_TINY_TEST_GROUPLOCATION_HPP
#define INCLUDED_MU_TINY_TEST_GROUPLOCATION_HPP

#include "mu/tiny/export.h"

#include <stddef.h>
#include <stdint.h>

namespace mu {
namespace tiny {
namespace test {

/**
 * @brief Source-location record for a test group.
 *
 * Constructed by the @ref TEST_GROUP() macro; self-registers into the
 * process-wide singly-linked list during static initialisation.
 */
class MUTINY_EXPORT GroupLocation
{
public:
  /**
   * @brief Record and register a group's source location.
   *
   * @param group_name  Name of the test group.
   * @param file_name   Source file where TEST_GROUP() appears.
   * @param line_number Line number of the TEST_GROUP() invocation.
   */
  GroupLocation(
      const char* group_name,
      const char* file_name,
      int_least32_t line_number
  ) noexcept;

  GroupLocation(const GroupLocation&) = delete;
  GroupLocation& operator=(const GroupLocation&) = delete;

  /** @return The group name. */
  const char* get_group() const;
  /** @return The source file path. */
  const char* get_file() const;
  /** @return The source line number. */
  int get_line_number() const;
  /** @return The next entry in the registration list, or nullptr. */
  GroupLocation* get_next() const;

  /** @return The head of the global group-location list. */
  static GroupLocation* get_head();

  /**
   * @brief Replace the head of the global group-location list.
   *
   * Intended for test infrastructure: save the current head before a test and
   * restore it afterwards to isolate the test from global registrations.
   *
   * @param head  New head (may be nullptr to clear the list).
   */
  static void set_head(GroupLocation* head);

private:
  const char* group_;
  const char* file_;
  int line_number_;
  GroupLocation* next_;

  static GroupLocation* head_;
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
