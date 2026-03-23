#include "mutiny/mock/IgnoredExpectedCall.hpp"

namespace mu {
namespace tiny {
namespace mock {

ExpectedCall& IgnoredExpectedCall::instance()
{
  static IgnoredExpectedCall call;
  return call;
}

}
}
} // namespace mu::tiny::mock
