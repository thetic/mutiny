#include "mutiny/mock/IgnoredExpectedCall.hpp"

namespace mu {
namespace tiny {
namespace mock {
using namespace mu::tiny::test;

MockExpectedCall& MockIgnoredExpectedCall::instance()
{
  static MockIgnoredExpectedCall call;
  return call;
}

}
}
} // namespace mu::tiny::mock
