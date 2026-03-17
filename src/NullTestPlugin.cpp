#include "CppMu/NullTestPlugin.hpp"

namespace cppmu {

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

} // namespace cppmu
