#include "mu/tiny/test/JUnitOutputPlugin.hpp"

#include "mu/tiny/test/JUnitOutput.hpp"

#include "mu/tiny/String.hpp"

namespace mu {
namespace tiny {
namespace test {

namespace {

String basename_from_path(const char* path)
{
  if ((path == nullptr) || (*path == 0)) {
    return "";
  }
  const char* base = path;
  for (const char* p = path; *p != 0; ++p) {
    if (*p == '/' || *p == '\\') {
      base = p + 1;
    }
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
