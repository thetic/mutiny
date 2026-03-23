#ifndef INCLUDED_MUTINY_TEST_NULLPLUGIN_HPP
#define INCLUDED_MUTINY_TEST_NULLPLUGIN_HPP

#include "mutiny/test/Plugin.hpp"

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

}
}
} // namespace mu::tiny::test

#endif // INCLUDED_MUTINY_TEST_NULLPLUGIN_HPP
