#include "CppUTest/MockIgnoredExpectedCall.hpp"

namespace cpputest {

MockExpectedCall&
MockIgnoredExpectedCall::instance()
{
  static MockIgnoredExpectedCall call;
  return call;
}

} // namespace cpputest
