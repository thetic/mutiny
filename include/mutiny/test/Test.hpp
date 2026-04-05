/**
 * @file
 * @brief Base class for all test objects.
 *
 * Used by @ref mu::tiny::test::Shell and created indirectly through the @ref
 * TEST_GROUP and @ref TEST macros.
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
 * The @ref TEST() macro generates a subclass of @ref Shell (which extends
 * @ref Test) that overrides @ref test_body() with the test code.
 * @ref setup() and @ref teardown() are called by @ref run() before and after
 * @ref test_body(), respectively.
 */
class MUTINY_EXPORT Test
{
public:
  Test() noexcept = default;
  virtual ~Test() = default;

  /** @brief Execute this test, calling setup(), test_body(), and teardown(). */
  virtual void run();

  /** @brief Called before @ref test_body(); override in a @ref TEST_GROUP to
   * set up fixtures. */
  virtual void setup();

  /** @brief Called after @ref test_body() (even on failure); override in a
   * @ref TEST_GROUP to release fixtures. */
  virtual void teardown();

  /** @brief The test body; overridden by the @ref TEST() macro expansion. */
  virtual void test_body();
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
