#ifndef INCLUDED_MUTINY_NULLTESTPLUGIN_HPP
#define INCLUDED_MUTINY_NULLTESTPLUGIN_HPP

#include "mutiny/test/Plugin.hpp"

namespace mu {
namespace tiny {
namespace test {

class NullTestPlugin : public TestPlugin
{
public:
  NullTestPlugin();

  void run_all_pre_test_action(TestShell& test, TestResult& result) override;
  void run_all_post_test_action(TestShell& test, TestResult& result) override;

  static NullTestPlugin* instance();
};

}
}
} // namespace mu::tiny::test

#endif // INCLUDED_MUTINY_NULLTESTPLUGIN_HPP
