#include "mu/tiny/test/JUnitOutputPlugin.hpp"

#include "mu/tiny/test/JUnitOutput.hpp"

#include "mu/tiny/String.hpp"

namespace mu {
namespace tiny {
namespace test {

namespace {

String basename_from_path(const char* path)
{
  if (!path || !*path)
    return "";
  const char* base = path;
  for (const char* p = path; *p; ++p) {
    if (*p == '/' || *p == '\\')
      base = p + 1;
  }
  return base;
}

} // namespace

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
  if (!string_starts_with(arg, "-pjunit"))
    return false;
  if (arg.size() > 7) {
    if (arg[7] != '=')
      return false;
    package_name_ = arg.substr(8);
  } else {
    package_name_ = basename_from_path(argv[0]);
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
  if (!active_)
    return nullptr;
  auto* output = new JUnitOutput;
  if (!package_name_.empty())
    output->set_package_name(package_name_);
  return output;
}

} // namespace test
} // namespace tiny
} // namespace mu
