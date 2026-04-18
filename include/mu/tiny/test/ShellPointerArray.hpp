/**
 * @file
 * @brief Array-of-Shell-pointers helper for shuffling and reversing tests.
 *
 * Used by @ref mu::tiny::test::Registry::shuffle_tests() and @ref
 * mu::tiny::test::Registry::reverse_tests().
 */

#ifndef INCLUDED_MU_TINY_TEST_SHELLPOINTERARRAY_HPP
#define INCLUDED_MU_TINY_TEST_SHELLPOINTERARRAY_HPP

#include "mu/tiny/export.h"

#include <stddef.h>

namespace mu {
namespace tiny {
namespace test {

/**
 * @brief Replaceable pseudo-random number generator used for test shuffling.
 *
 * Defaults to the platform's `rand()`. Replace with a deterministic function
 * to make shuffle order reproducible in tests. Used by
 * @ref ShellPointerArray::shuffle().
 */
MUTINY_EXPORT extern int (*rand)();

class Shell;

/**
 * @brief Flat array of Shell pointers that supports in-place shuffle/reverse.
 *
 * Built from the linked list of tests held by @ref Registry so that O(n)
 * random access is available for shuffling. Call @ref relink_tests_in_order()
 * after sorting to restore the linked-list structure. Used by
 * @ref Registry::shuffle_tests() and @ref Registry::reverse_tests().
 */
class MUTINY_EXPORT ShellPointerArray
{
public:
  /**
   * @brief Build the array by walking the linked list starting at
   * @p first_test.
   * @param first_test  Head of the Shell linked list.
   */
  ShellPointerArray(Shell* first_test);
  ~ShellPointerArray();

  /**
   * @brief Shuffle the array using a seeded Fisher-Yates algorithm.
   * @param seed  Seed for the rand() function pointer.
   */
  void shuffle(size_t seed);

  /** @brief Reverse the order of the array. */
  void reverse();

  /** @brief Rewrite the Shell next-pointers to match the current array order.
   */
  void relink_tests_in_order();

  /** @return The first Shell in the (possibly reordered) array. */
  Shell* get_first_test() const;

  /**
   * @brief Random-access into the array.
   * @param index  Zero-based index.
   * @return The Shell at @p index, or nullptr if out of range.
   */
  Shell* get(size_t index) const;

private:
  void swap(size_t index1, size_t index2);

  Shell** array_of_tests_{ nullptr };
  size_t count_{ 0 };
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
