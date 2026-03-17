#include "CppMu/MockIgnoredExpectedCall.hpp"

namespace cppmu {

MockExpectedCall& MockIgnoredExpectedCall::instance()
{
  static MockIgnoredExpectedCall call;
  return call;
}

} // namespace cppmu
