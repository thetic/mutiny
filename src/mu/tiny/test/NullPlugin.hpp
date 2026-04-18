#ifndef INCLUDED_MU_TINY_TEST_NULLPLUGIN_HPP
#define INCLUDED_MU_TINY_TEST_NULLPLUGIN_HPP

#include "mu/tiny/test/Plugin.hpp"

namespace mu {
namespace tiny {
namespace test {

class NullPlugin : public Plugin
{
public:
  NullPlugin();

  void run_all_pre_test_action(Shell& test, Result& result) override;
  void run_all_post_test_action(Shell& test, Result& result) override;

  static NullPlugin* instance();
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif // INCLUDED_MU_TINY_TEST_NULLPLUGIN_HPP
