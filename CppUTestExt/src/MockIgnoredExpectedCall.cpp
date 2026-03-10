#include "CppUTestExt/MockIgnoredExpectedCall.hpp"

namespace cpputest {
namespace extensions {

MockExpectedCall&
MockIgnoredExpectedCall::instance()
{
  static MockIgnoredExpectedCall call;
  return call;
}

} // namespace extensions
} // namespace cpputest
