#include "mu/tiny/test/TapOutputPlugin.hpp"

#include "mu/tiny/test/TapOutput.hpp"

namespace mu {
namespace tiny {
namespace test {

TapOutputPlugin::TapOutputPlugin()
  : Plugin("TapOutputPlugin")
{
}

bool TapOutputPlugin::parse_arguments(int /*argc*/, const char* const* argv)
{
  String arg = argv[0];
  if (arg != "-ptap") {
    return false;
  }
  active_ = true;
  return true;
}

String TapOutputPlugin::get_help() const
{
  return "  -ptap             - output TAP version 13 to stdout\n";
}

Output* TapOutputPlugin::create_output()
{
  if (!active_) {
    return nullptr;
  }
  return new TapOutput;
}

} // namespace test
} // namespace tiny
} // namespace mu
