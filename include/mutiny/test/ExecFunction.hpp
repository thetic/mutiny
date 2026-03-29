/**
 * @file
 * @brief Callable interface used by TestingFixture.
 *
 * @see TestingFixture
 */

#ifndef INCLUDED_MUTINY_TEST_EXECFUNCTION_HPP
#define INCLUDED_MUTINY_TEST_EXECFUNCTION_HPP

#include "mutiny/export.h"

namespace mu {
namespace tiny {
namespace test {

/**
 * @brief Abstract callable invoked by TestingFixture as the test body.
 *
 * Implement exec() to wrap a test function for use with
 * TestingFixture::set_test_function().
 *
 * @see TestingFixture
 */
class MUTINY_EXPORT ExecFunction
{
public:
  ExecFunction() = default;
  virtual ~ExecFunction() = default;

  /** @brief Execute the wrapped function. */
  virtual void exec() = 0;
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
