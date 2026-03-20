#include "CppMu/TestPlugin.hpp"

#include "CppMu/NullTestPlugin.hpp"
#include "CppMu/TestOutput.hpp"

namespace cppmu {

TestPlugin::TestPlugin(const String& name)
  : next_(NullTestPlugin::instance())
  , name_(name)
  , enabled_(true)
{
}

TestPlugin::TestPlugin(TestPlugin* next)
  : next_(next)
  , name_("null")
  , enabled_(true)
{
}

TestPlugin* TestPlugin::add_plugin(TestPlugin* plugin)
{
  next_ = plugin;
  return this;
}

void TestPlugin::run_all_pre_test_action(TestShell& test, TestResult& result)
{
  if (enabled_)
    pre_test_action(test, result);
  next_->run_all_pre_test_action(test, result);
}

void TestPlugin::run_all_post_test_action(TestShell& test, TestResult& result)
{
  next_->run_all_post_test_action(test, result);
  if (enabled_)
    post_test_action(test, result);
}

bool TestPlugin::parse_all_arguments(int argc, char** argv, int index)
{
  return parse_all_arguments(argc, const_cast<const char* const*>(argv), index);
}

bool TestPlugin::parse_all_arguments(
    int argc,
    const char* const* argv,
    int index
)
{
  if (parse_arguments(argc, argv, index))
    return true;
  if (next_)
    return next_->parse_all_arguments(argc, argv, index);
  return false;
}

String TestPlugin::get_all_help() const
{
  String help = get_help();
  if (next_)
    help += next_->get_all_help();
  return help;
}

TestOutput* TestPlugin::create_all_outputs()
{
  TestOutput* out = create_output();
  if (out)
    return out;
  if (next_)
    return next_->create_all_outputs();
  return nullptr;
}

const String& TestPlugin::get_name()
{
  return name_;
}

TestPlugin* TestPlugin::get_plugin_by_name(const String& name)
{
  if (name == name_)
    return this;
  if (next_)
    return next_->get_plugin_by_name(name);
  return (next_);
}

TestPlugin* TestPlugin::get_next()
{
  return next_;
}
TestPlugin* TestPlugin::remove_plugin_by_name(const String& name)
{
  TestPlugin* removed = nullptr;
  if (next_ && next_->get_name() == name) {
    removed = next_;
    next_ = next_->next_;
  }
  return removed;
}

void TestPlugin::disable()
{
  enabled_ = false;
}

void TestPlugin::enable()
{
  enabled_ = true;
}

bool TestPlugin::is_enabled()
{
  return enabled_;
}

} // namespace cppmu
