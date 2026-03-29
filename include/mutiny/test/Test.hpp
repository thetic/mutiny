/**
 * @file
 * @brief Base class for all test objects.
 *
 * @see Shell, TEST_GROUP, TEST
 */

#ifndef INCLUDED_MUTINY_TEST_TEST_HPP
#define INCLUDED_MUTINY_TEST_TEST_HPP

#include "mutiny/export.h"

namespace mu {
namespace tiny {
namespace test {

/**
 * @brief Abstract base class for a single runnable test.
 *
 * The TEST() macro generates a subclass of Shell (which extends Test) that
 * overrides test_body() with the test code. setup() and teardown() are called
 * by run() before and after test_body(), respectively.
 *
 * @see Shell, TEST_GROUP, TEST
 */
class MUTINY_EXPORT Test
{
public:
  Test() noexcept = default;
  virtual ~Test() = default;

  /** @brief Execute this test, calling setup(), test_body(), and teardown(). */
  virtual void run();

  /** @brief Called before test_body(); override in a TEST_GROUP to set up
   * fixtures. */
  virtual void setup();

  /** @brief Called after test_body() (even on failure); override in a
   * TEST_GROUP to release fixtures. */
  virtual void teardown();

  /** @brief The test body; overridden by the TEST() macro expansion. */
  virtual void test_body();
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
