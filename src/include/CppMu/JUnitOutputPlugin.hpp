#ifndef INCLUDED_CPPMU_JUNITOUTPUTPLUGIN_HPP
#define INCLUDED_CPPMU_JUNITOUTPUTPLUGIN_HPP

#include "CppMu/TestPlugin.hpp"

namespace cppmu {

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

} // namespace cppmu

#endif
