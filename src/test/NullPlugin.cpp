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

void NullPlugin::run_all_pre_test_action(Shell& /*test*/, Result& /*result*/) {}

void NullPlugin::run_all_post_test_action(Shell& /*test*/, Result& /*result*/)
{
}

} // namespace test
} // namespace tiny
} // namespace mu
