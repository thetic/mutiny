#include "mutiny/test/NullPlugin.hpp"

namespace mu {
namespace tiny {
namespace test {

NullTestPlugin::NullTestPlugin()
  : TestPlugin(nullptr)
{
}

NullTestPlugin* NullTestPlugin::instance()
{
  static NullTestPlugin instance;
  return &instance;
}

void NullTestPlugin::run_all_pre_test_action(TestShell&, TestResult&) {}

void NullTestPlugin::run_all_post_test_action(TestShell&, TestResult&) {}

}
}
} // namespace mu::tiny::test
