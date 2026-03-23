#ifndef INCLUDED_MUTINY_JUNITOUTPUTPLUGIN_HPP
#define INCLUDED_MUTINY_JUNITOUTPUTPLUGIN_HPP

#include "mutiny/test/Plugin.hpp"

namespace mu {
namespace tiny {
namespace test {

class TestOutput;

class JUnitOutputPlugin : public TestPlugin
{
public:
  JUnitOutputPlugin();
  bool parse_arguments(int argc, const char* const* argv, int index) override;
  String get_help() const override;

  TestOutput* create_output() override;

  static constexpr const char* name{ "JUnitOutputPlugin" };

private:
  bool active_{ false };
};

}
}
} // namespace mu::tiny::test

#endif
