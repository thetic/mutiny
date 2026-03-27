#include "mutiny/test/JUnitOutputPlugin.hpp"

#include "mutiny/test/JUnitOutput.hpp"

#include "mutiny/String.hpp"

namespace mu {
namespace tiny {
namespace test {

JUnitOutputPlugin::JUnitOutputPlugin()
  : Plugin("JUnitOutputPlugin")
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

Output* JUnitOutputPlugin::create_output()
{
  if (active_)
    return new JUnitOutput;
  return nullptr;
}

} // namespace test
} // namespace tiny
} // namespace mu
