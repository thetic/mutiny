#include "CppMu/JUnitOutputPlugin.hpp"

#include "CppMu/JUnitTestOutput.hpp"
#include "CppMu/String.hpp"

namespace cppmu {

JUnitOutputPlugin::JUnitOutputPlugin()
  : TestPlugin("JUnitOutputPlugin")
{
}

bool JUnitOutputPlugin::parse_arguments(
    int /*argc*/,
    const char* const* argv,
    int index
)
{
  String arg = argv[index];
  if (arg.size() > 2 && String(arg.c_str() + 2) == "junit") {
    active_ = true;
    return true;
  }
  return false;
}

String JUnitOutputPlugin::get_help() const
{
  return "  -pjunit           - output JUnit XML output\n";
}

TestOutput* JUnitOutputPlugin::create_output()
{
  if (active_)
    return new JUnitTestOutput;
  return nullptr;
}

} // namespace cppmu
