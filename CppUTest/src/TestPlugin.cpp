#include "CppUTest/TestPlugin.hpp"

#include "CppUTest/TestHarness.hpp"

namespace cpputest {

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

TestPlugin::~TestPlugin() {}

TestPlugin*
TestPlugin::addPlugin(TestPlugin* plugin)
{
  next_ = plugin;
  return this;
}

void
TestPlugin::runAllPreTestAction(TestShell& test, TestResult& result)
{
  if (enabled_)
    preTestAction(test, result);
  next_->runAllPreTestAction(test, result);
}

void
TestPlugin::runAllPostTestAction(TestShell& test, TestResult& result)
{
  next_->runAllPostTestAction(test, result);
  if (enabled_)
    postTestAction(test, result);
}

bool
TestPlugin::parseAllArguments(int ac, char** av, int index)
{
  return parseAllArguments(ac, const_cast<const char* const*>(av), index);
}

bool
TestPlugin::parseAllArguments(int ac, const char* const* av, int index)
{
  if (parseArguments(ac, av, index))
    return true;
  if (next_)
    return next_->parseAllArguments(ac, av, index);
  return false;
}

const String&
TestPlugin::getName()
{
  return name_;
}

TestPlugin*
TestPlugin::getPluginByName(const String& name)
{
  if (name == name_)
    return this;
  if (next_)
    return next_->getPluginByName(name);
  return (next_);
}

TestPlugin*
TestPlugin::getNext()
{
  return next_;
}
TestPlugin*
TestPlugin::removePluginByName(const String& name)
{
  TestPlugin* removed = nullptr;
  if (next_ && next_->getName() == name) {
    removed = next_;
    next_ = next_->next_;
  }
  return removed;
}

void
TestPlugin::disable()
{
  enabled_ = false;
}

void
TestPlugin::enable()
{
  enabled_ = true;
}

bool
TestPlugin::isEnabled()
{
  return enabled_;
}

//////// NullPlugin

NullTestPlugin::NullTestPlugin()
  : TestPlugin(nullptr)
{
}

NullTestPlugin*
NullTestPlugin::instance()
{
  static NullTestPlugin _instance;
  return &_instance;
}

void
NullTestPlugin::runAllPreTestAction(TestShell&, TestResult&)
{
}

void
NullTestPlugin::runAllPostTestAction(TestShell&, TestResult&)
{
}

} // namespace cpputest
