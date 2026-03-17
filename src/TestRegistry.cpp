#include "CppMu/TestRegistry.hpp"

#include "CppMu/NullTestPlugin.hpp"
#include "CppMu/TestResult.hpp"
#include "CppMu/TestShell.hpp"
#include "CppMu/TestShellPointerArray.hpp"

namespace cppmu {

TestRegistry::TestRegistry()
  : first_plugin_(NullTestPlugin::instance())

{
}

void TestRegistry::add_test(TestShell* test)
{
  tests_ = test->add_test(tests_);
}

void TestRegistry::run_all_tests(TestResult& result)
{
  bool group_start = true;

  result.tests_started();
  for (TestShell* test = tests_; test != nullptr; test = test->get_next()) {
    if (run_ignored_)
      test->set_run_ignored();

    if (group_start) {
      result.current_group_started(test);
      group_start = false;
    }

    result.count_test();
    if (test_should_run(test, result)) {
      result.current_test_started(test);
      test->run_one_test(first_plugin_, result);
      result.current_test_ended(test);
    }

    if (end_of_group(test)) {
      group_start = true;
      result.current_group_ended(test);
    }
  }
  result.tests_ended();
  current_repetition_++;
}

void TestRegistry::list_test_group_names(TestResult& result)
{
  String group_list;

  for (TestShell* test = tests_; test != nullptr; test = test->get_next()) {
    String gname;
    gname += "#";
    gname += test->get_group();
    gname += "#";

    if (!string_contains(group_list, gname)) {
      group_list += gname;
      group_list += " ";
    }
  }

  string_replace(group_list, "#", "");

  if (string_ends_with(group_list, " "))
    group_list = group_list.substr(0, group_list.size() - 1);
  result.print(group_list.c_str());
}

void TestRegistry::list_test_group_and_case_names(TestResult& result)
{
  String group_and_name_list;

  for (TestShell* test = tests_; test != nullptr; test = test->get_next()) {
    if (test_should_run(test, result)) {
      String group_and_name;
      group_and_name += "#";
      group_and_name += test->get_group();
      group_and_name += ".";
      group_and_name += test->get_name();
      group_and_name += "#";

      if (!string_contains(group_and_name_list, group_and_name)) {
        group_and_name_list += group_and_name;
        group_and_name_list += " ";
      }
    }
  }

  string_replace(group_and_name_list, "#", "");

  if (string_ends_with(group_and_name_list, " "))
    group_and_name_list =
        group_and_name_list.substr(0, group_and_name_list.size() - 1);
  result.print(group_and_name_list.c_str());
}

void TestRegistry::list_test_locations(TestResult& result)
{
  String test_locations;

  for (TestShell* test = tests_; test != nullptr; test = test->get_next()) {
    String test_location;
    test_location += test->get_group();
    test_location += ".";
    test_location += test->get_name();
    test_location += ".";
    test_location += test->get_file();
    test_location += ".";
    test_location +=
        string_from_format("%d\n", static_cast<int>(test->get_line_number()));

    test_locations += test_location;
  }

  result.print(test_locations.c_str());
}

void TestRegistry::list_ordered_test_locations(TestResult& result)
{
  String test_locations;

  for (TestShell* test = tests_; test != nullptr; test = test->get_next()) {
    if (!test->is_ordered())
      continue;
    String test_location;
    test_location += test->get_group();
    test_location += ".";
    test_location += test->get_name();
    test_location += ".";
    test_location += test->get_file();
    test_location += ".";
    test_location +=
        string_from_format("%d\n", static_cast<int>(test->get_line_number()));

    test_locations += test_location;
  }

  result.print(test_locations.c_str());
}

bool TestRegistry::end_of_group(TestShell* test)
{
  return (
      !test || !test->get_next() ||
      test->get_group() != test->get_next()->get_group()
  );
}

size_t TestRegistry::count_tests()
{
  return tests_ ? tests_->count_tests() : 0;
}

TestRegistry* TestRegistry::current_registry_ = nullptr;

TestRegistry* TestRegistry::get_current_registry()
{
  static TestRegistry registry;
  return (current_registry_ == nullptr) ? &registry : current_registry_;
}

void TestRegistry::set_current_registry(TestRegistry* registry)
{
  current_registry_ = registry;
}

void TestRegistry::un_do_last_add_test()
{
  tests_ = tests_ ? tests_->get_next() : nullptr;
}

void TestRegistry::set_name_filters(const TestFilter* filters)
{
  name_filters_ = filters;
}

void TestRegistry::set_group_filters(const TestFilter* filters)
{
  group_filters_ = filters;
}

void TestRegistry::set_run_ignored()
{
  run_ignored_ = true;
}

int TestRegistry::get_current_repetition()
{
  return current_repetition_;
}

bool TestRegistry::test_should_run(TestShell* test, TestResult& result)
{
  if (test->should_run(group_filters_, name_filters_))
    return true;
  else {
    result.count_filtered_out();
    return false;
  }
}

void TestRegistry::reset_plugins()
{
  first_plugin_ = NullTestPlugin::instance();
}

void TestRegistry::install_plugin(TestPlugin* plugin)
{
  first_plugin_ = plugin->add_plugin(first_plugin_);
}

TestPlugin* TestRegistry::get_first_plugin()
{
  return first_plugin_;
}

TestPlugin* TestRegistry::get_plugin_by_name(const String& name)
{
  return first_plugin_->get_plugin_by_name(name);
}

void TestRegistry::remove_plugin_by_name(const String& name)
{
  first_plugin_->remove_plugin_by_name(name);
  if (first_plugin_->get_name() == name)
    first_plugin_ = first_plugin_->get_next();
  first_plugin_->remove_plugin_by_name(name);
}

int TestRegistry::count_plugins()
{
  int count = 0;
  for (TestPlugin* plugin = first_plugin_; plugin != NullTestPlugin::instance();
       plugin = plugin->get_next())
    count++;
  return count;
}

TestShell* TestRegistry::get_first_test()
{
  return tests_;
}

void TestRegistry::shuffle_tests(size_t seed)
{
  TestShellPointerArray array(get_first_test());
  array.shuffle(seed);
  tests_ = array.get_first_test();
}

void TestRegistry::reverse_tests()
{
  TestShellPointerArray array(get_first_test());
  array.reverse();
  tests_ = array.get_first_test();
}

TestShell* TestRegistry::get_test_with_next(TestShell* test)
{
  TestShell* current = tests_;
  while (current && current->get_next() != test)
    current = current->get_next();
  return current;
}

TestShell* TestRegistry::find_test_with_name(const String& name)
{
  TestShell* current = tests_;
  while (current) {
    if (current->get_name() == name)
      return current;
    current = current->get_next();
  }
  return nullptr;
}

TestShell* TestRegistry::find_test_with_group(const String& group)
{
  TestShell* current = tests_;
  while (current) {
    if (current->get_group() == group)
      return current;
    current = current->get_next();
  }
  return nullptr;
}

} // namespace cppmu
