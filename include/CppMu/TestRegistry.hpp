///////////////////////////////////////////////////////////////////////////////
//
// TestRegistry is a collection of tests that can be run
//

#ifndef INCLUDED_CPPMU_TESTREGISTRY_HPP
#define INCLUDED_CPPMU_TESTREGISTRY_HPP

#include "CppMu/String.hpp"
#include "CppMu/TestFilter.hpp"

#include <stddef.h>

namespace cppmu {

class TestShell;
class TestResult;
class TestPlugin;

class TestRegistry
{
public:
  TestRegistry();
  virtual ~TestRegistry();

  virtual void add_test(TestShell* test);
  virtual void un_do_last_add_test();
  virtual size_t count_tests();
  virtual void run_all_tests(TestResult& result);
  virtual void shuffle_tests(size_t seed);
  virtual void reverse_tests();
  virtual void list_test_group_names(TestResult& result);
  virtual void list_test_group_and_case_names(TestResult& result);
  virtual void list_test_locations(TestResult& result);
  virtual void list_ordered_test_locations(TestResult& result);
  virtual void set_name_filters(const TestFilter* filters);
  virtual void set_group_filters(const TestFilter* filters);
  virtual void install_plugin(TestPlugin* plugin);
  virtual void reset_plugins();
  virtual TestPlugin* get_first_plugin();
  virtual TestPlugin* get_plugin_by_name(const String& name);
  virtual void remove_plugin_by_name(const String& name);
  virtual int count_plugins();

  virtual TestShell* get_first_test();
  virtual TestShell* get_test_with_next(TestShell* test);

  virtual TestShell* find_test_with_name(const String& name);
  virtual TestShell* find_test_with_group(const String& name);

  static TestRegistry* get_current_registry();
  virtual void set_current_registry(TestRegistry* registry);

  int get_current_repetition();
  void set_run_ignored();

private:
  bool test_should_run(TestShell* test, TestResult& result);
  bool end_of_group(TestShell* test);

  TestShell* tests_{ nullptr };
  const TestFilter* name_filters_{ nullptr };
  const TestFilter* group_filters_{ nullptr };
  TestPlugin* first_plugin_;
  static TestRegistry* current_registry_;
  int current_repetition_{ 0 };
  bool run_ignored_{ false };
};

} // namespace cppmu

#endif
