#include "mu/tiny/test/NullPlugin.hpp"

namespace mu {
namespace tiny {
namespace test {

NullPlugin::NullPlugin()
  : Plugin(nullptr)
{
}

NullPlugin* NullPlugin::instance()
{
  static NullPlugin instance;
  return &instance;
}

void NullPlugin::run_all_pre_test_action(Shell&, Result&) {}

void NullPlugin::run_all_post_test_action(Shell&, Result&) {}

} // namespace test
} // namespace tiny
} // namespace mu
