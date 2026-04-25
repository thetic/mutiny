#include "mu/tiny/test/Plugin.hpp"

#include "mu/tiny/test/NullPlugin.hpp"
#include "mu/tiny/test/Output.hpp"

namespace mu {
namespace tiny {
namespace test {

Plugin::Plugin(const String& name)
  : next_(NullPlugin::instance())
  , name_(name)
  , enabled_(true)
{
}

Plugin::Plugin(Plugin* next)
  : next_(next)
  , name_("null")
  , enabled_(true)
{
}

Plugin* Plugin::add_plugin(Plugin* plugin)
{
  next_ = plugin;
  return this;
}

void Plugin::pre_test_action(Shell& /*test*/, Result& /*result*/) {}

void Plugin::run_all_pre_test_action(Shell& test, Result& result)
{
  if (enabled_) {
    pre_test_action(test, result);
  }
  next_->run_all_pre_test_action(test, result);
}

void Plugin::post_test_action(Shell& /*test*/, Result& /*result*/) {}

void Plugin::run_all_post_test_action(Shell& test, Result& result)
{
  next_->run_all_post_test_action(test, result);
  if (enabled_) {
    post_test_action(test, result);
  }
}

bool Plugin::parse_arguments(int /*argc*/, const char* const* /*argv*/)
{
  return false;
}

bool Plugin::parse_all_arguments(int argc, char** argv)
{
  return parse_all_arguments(argc, const_cast<const char* const*>(argv));
}

bool Plugin::parse_all_arguments(int argc, const char* const* argv)
{
  if (parse_arguments(argc, argv)) {
    return true;
  }
  if (next_ != nullptr) {
    return next_->parse_all_arguments(argc, argv);
  }
  return false;
}

String Plugin::get_all_help() const
{
  String help = get_help();
  if (next_ != nullptr) {
    help += next_->get_all_help();
  }
  return help;
}

Output* Plugin::create_all_outputs()
{
  Output* out = create_output();
  if (out != nullptr) {
    return out;
  }
  if (next_ != nullptr) {
    return next_->create_all_outputs();
  }
  return nullptr;
}

const String& Plugin::get_name()
{
  return name_;
}

Plugin* Plugin::get_next()
{
  return next_;
}

void Plugin::disable()
{
  enabled_ = false;
}

void Plugin::enable()
{
  enabled_ = true;
}

bool Plugin::is_enabled()
{
  return enabled_;
}

} // namespace test
} // namespace tiny
} // namespace mu
