#ifndef INCLUDED_MUTINY_TEST_JUNITOUTPUTPLUGIN_HPP
#define INCLUDED_MUTINY_TEST_JUNITOUTPUTPLUGIN_HPP

#include "mutiny/test/Plugin.hpp"

namespace mu {
namespace tiny {
namespace test {

class Output;

class JUnitOutputPlugin : public Plugin
{
public:
  JUnitOutputPlugin();
  bool parse_arguments(int argc, const char* const* argv, int index) override;
  String get_help() const override;

  Output* create_output() override;

  static constexpr const char* name{ "JUnitOutputPlugin" };

private:
  bool active_{ false };
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
