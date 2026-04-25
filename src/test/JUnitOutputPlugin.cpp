#include "mu/tiny/test/JUnitOutputPlugin.hpp"

#include "mu/tiny/test/JUnitOutput.hpp"

#include "mu/tiny/String.hpp"

namespace mu {
namespace tiny {
namespace test {


JUnitOutputPlugin::JUnitOutputPlugin()
  : Plugin("JUnitOutputPlugin")
{
}

bool JUnitOutputPlugin::parse_arguments(int /*argc*/, const char* const* argv)
{
  String arg = argv[0];
  constexpr char flag[] = "-pjunit";
  constexpr size_t flag_len = sizeof(flag) / sizeof(flag[0]);

  if (!string_starts_with(arg, flag)) {
    return false;
  }
  if (arg.size() > (flag_len - 1)) {
    if (arg[flag_len - 1] != '=') {
      return false;
    }
    package_name_ = arg.substr(flag_len);
  }
  active_ = true;
  return true;
}

String JUnitOutputPlugin::get_help() const
{
  return "  -pjunit[=<name>]  - output JUnit XML as <name>.xml\n"
         "                      (defaults to the executable basename)\n";
}

Output* JUnitOutputPlugin::create_output()
{
  if (!active_) {
    return nullptr;
  }
  auto* output = new JUnitOutput;
  if (!package_name_.empty()) {
    output->set_package_name(package_name_);
  }
  return output;
}

} // namespace test
} // namespace tiny
} // namespace mu
