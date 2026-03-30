/**
 * @file
 * @brief Callable interface used by @ref mu::tiny::test::TestingFixture.
 */

#ifndef INCLUDED_MUTINY_TEST_EXECFUNCTION_HPP
#define INCLUDED_MUTINY_TEST_EXECFUNCTION_HPP

#include "mutiny/export.h"

namespace mu {
namespace tiny {
namespace test {

/**
 * @brief Abstract callable invoked by @ref TestingFixture as the test body.
 *
 * Implement @ref exec() to wrap a test function for use with
 * @ref TestingFixture::set_test_function().
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
