#ifndef INCLUDED_MUTINY_TEST_EXECFUNCTION_HPP
#define INCLUDED_MUTINY_TEST_EXECFUNCTION_HPP

#include "mutiny/export.h"

namespace mu {
namespace tiny {
namespace test {

class MUTINY_EXPORT ExecFunction
{
public:
  ExecFunction() = default;
  virtual ~ExecFunction() = default;

  virtual void exec() = 0;
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
