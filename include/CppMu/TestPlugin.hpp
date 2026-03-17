#ifndef INCLUDED_CPPMU_TESTPLUGIN_HPP
#define INCLUDED_CPPMU_TESTPLUGIN_HPP

#include "CppMu/String.hpp"

namespace cppmu {

class TestOutput;
class TestShell;
class TestResult;

class TestPlugin
{
public:
  TestPlugin(const String& name);
  virtual ~TestPlugin() = default;

  virtual void pre_test_action(TestShell&, TestResult&) {}

  virtual void post_test_action(TestShell&, TestResult&) {}

  virtual bool parse_arguments(int, const char* const*, int) { return false; }

  virtual TestOutput* create_output() { return nullptr; }
  virtual TestOutput* create_all_outputs();

  virtual void run_all_pre_test_action(TestShell&, TestResult&);
  virtual void run_all_post_test_action(TestShell&, TestResult&);
  virtual bool parse_all_arguments(
      int argc,
      const char* const* argv,
      int index
  );
  virtual bool parse_all_arguments(int argc, char** argv, int index);

  virtual TestPlugin* add_plugin(TestPlugin*);
  virtual TestPlugin* remove_plugin_by_name(const String& name);
  virtual TestPlugin* get_next();

  virtual void disable();
  virtual void enable();
  virtual bool is_enabled();

  const String& get_name();
  TestPlugin* get_plugin_by_name(const String& name);

protected:
  TestPlugin(TestPlugin* next);

private:
  TestPlugin* next_;
  String name_;
  bool enabled_;
};

} // namespace cppmu

#endif
