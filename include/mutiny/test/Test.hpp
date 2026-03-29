#ifndef INCLUDED_MUTINY_TEST_TEST_HPP
#define INCLUDED_MUTINY_TEST_TEST_HPP

#include "mutiny/export.h"

namespace mu {
namespace tiny {
namespace test {

class MUTINY_EXPORT Test
{
public:
  Test() noexcept = default;
  virtual ~Test() = default;
  virtual void run();

  virtual void setup();
  virtual void teardown();
  virtual void test_body();
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
