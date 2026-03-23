///////////////////////////////////////////////////////////////////////////////
//
// Registry is a collection of tests that can be run
//

#ifndef INCLUDED_MUTINY_TEST_REGISTRY_HPP
#define INCLUDED_MUTINY_TEST_REGISTRY_HPP

#include "mutiny/test/Filter.hpp"
#include "mutiny/test/String.hpp"

#include <stddef.h>

namespace mu {
namespace tiny {
namespace test {

class Shell;
class Result;
class Plugin;

class Registry
{
public:
  Registry();
  virtual ~Registry() = default;

  virtual void add_test(Shell* test);
  virtual void un_do_last_add_test();
  virtual size_t count_tests();
  virtual void run_all_tests(Result& result);
  virtual void shuffle_tests(size_t seed);
  virtual void reverse_tests();
  virtual void list_test_group_names(Result& result);
  virtual void list_test_group_and_case_names(Result& result);
  virtual void list_test_locations(Result& result);
  virtual void list_ordered_test_locations(Result& result);
  virtual void set_name_filters(const Filter* filters);
  virtual void set_group_filters(const Filter* filters);
  virtual void install_plugin(Plugin* plugin);
  virtual void reset_plugins();
  virtual Plugin* get_first_plugin();
  virtual Plugin* get_plugin_by_name(const String& name);
  virtual void remove_plugin_by_name(const String& name);
  virtual int count_plugins();

  virtual Shell* get_first_test();
  virtual Shell* get_test_with_next(Shell* test);

  virtual Shell* find_test_with_name(const String& name);
  virtual Shell* find_test_with_group(const String& name);

  static Registry* get_current_registry();
  virtual void set_current_registry(Registry* registry);

  int get_current_repetition();
  void set_run_ignored();

private:
  bool test_should_run(Shell* test, Result& result);
  bool end_of_group(Shell* test);

  Shell* tests_{ nullptr };
  const Filter* name_filters_{ nullptr };
  const Filter* group_filters_{ nullptr };
  Plugin* first_plugin_;
  static Registry* current_registry_;
  int current_repetition_{ 0 };
  bool run_ignored_{ false };
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
