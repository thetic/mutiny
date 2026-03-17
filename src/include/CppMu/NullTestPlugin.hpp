#ifndef INCLUDED_CPPMU_NULLTESTPLUGIN_HPP
#define INCLUDED_CPPMU_NULLTESTPLUGIN_HPP

#include "CppMu/TestPlugin.hpp"

namespace cppmu {

class NullTestPlugin : public TestPlugin
{
public:
  NullTestPlugin();

  void run_all_pre_test_action(TestShell& test, TestResult& result) override;
  void run_all_post_test_action(TestShell& test, TestResult& result) override;

  static NullTestPlugin* instance();
};

} // namespace cppmu

#endif // INCLUDED_CPPMU_NULLTESTPLUGIN_HPP
