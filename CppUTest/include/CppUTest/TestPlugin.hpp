#ifndef D_TestPlugin_h
#define D_TestPlugin_h

#include "CppUTest/String.hpp"

namespace cpputest {

class TestShell;
class TestResult;

class TestPlugin
{
public:
  TestPlugin(const String& name);
  virtual ~TestPlugin();

  virtual void preTestAction(TestShell&, TestResult&) {}

  virtual void postTestAction(TestShell&, TestResult&) {}

  virtual bool parseArguments(int /* ac */,
      const char* const* /* av */,
      int /* index */)
  {
    return false;
  }

  virtual void runAllPreTestAction(TestShell&, TestResult&);
  virtual void runAllPostTestAction(TestShell&, TestResult&);
  virtual bool parseAllArguments(int ac, const char* const* av, int index);
  virtual bool parseAllArguments(int ac, char** av, int index);

  virtual TestPlugin* addPlugin(TestPlugin*);
  virtual TestPlugin* removePluginByName(const String& name);
  virtual TestPlugin* getNext();

  virtual void disable();
  virtual void enable();
  virtual bool isEnabled();

  const String& getName();
  TestPlugin* getPluginByName(const String& name);

protected:
  TestPlugin(TestPlugin* next_);

private:
  TestPlugin* next_;
  String name_;
  bool enabled_;
};

///////////// Null Plugin

class NullTestPlugin : public TestPlugin
{
public:
  NullTestPlugin();

  virtual void runAllPreTestAction(TestShell& test,
      TestResult& result) override;
  virtual void runAllPostTestAction(TestShell& test,
      TestResult& result) override;

  static NullTestPlugin* instance();
};

} // namespace cpputest

#endif
