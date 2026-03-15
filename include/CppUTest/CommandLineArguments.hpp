#ifndef INCLUDED_CPPUTEST_COMMANDLINEARGUMENTS_HPP
#define INCLUDED_CPPUTEST_COMMANDLINEARGUMENTS_HPP

#include "CppUTest/String.hpp"
#include "CppUTest/TestFilter.hpp"
#include "CppUTest/TestOutput.hpp"

namespace cpputest {

class TestPlugin;

class CommandLineArguments
{
public:
  explicit CommandLineArguments(int argc, const char* const* argv);
  virtual ~CommandLineArguments();

  bool parse(TestPlugin* plugin);
  bool need_help() const;
  bool is_verbose() const;
  bool is_very_verbose() const;
  bool is_color() const;
  bool is_listing_test_group_names() const;
  bool is_listing_test_group_and_case_names() const;
  bool is_listing_test_locations() const;
  bool is_listing_ordered_test_locations() const;
  bool is_run_ignored() const;
  size_t get_repeat_count() const;
  bool is_shuffling() const;
  bool is_reversing() const;
  bool is_crashing_on_fail() const;
  bool is_rethrowing_exceptions() const;
  size_t get_shuffle_seed() const;
  const TestFilter* get_group_filters() const;
  const TestFilter* get_name_filters() const;
  bool is_j_unit_output() const;
  bool is_eclipse_output() const;
  bool is_team_city_output() const;
  const String& get_package_name() const;
  const char* usage() const;
  const char* help() const;

private:
  enum class OutputType
  {
    eclipse,
    junit,
    teamcity
  };

  int ac_;
  const char* const* av_;

  bool need_help_;
  bool verbose_;
  bool very_verbose_;
  bool color_;
  bool list_test_group_names_;
  bool list_test_group_and_case_names_;
  bool list_test_locations_;
  bool list_ordered_test_locations_;
  bool run_ignored_;
  bool reversing_;
  bool crash_on_fail_;
  bool rethrow_exceptions_;
  bool shuffling_;
  bool shuffling_pre_seeded_;
  size_t repeat_;
  size_t shuffle_seed_;
  TestFilter* group_filters_;
  TestFilter* name_filters_;
  OutputType output_type_;
  String package_name_;

  String get_parameter_field(int argc,
      const char* const* argv,
      int& i,
      const String& parameter_name);
  void set_repeat_count(int argc, const char* const* argv, int& index);
  bool set_shuffle(int argc, const char* const* argv, int& index);
  void add_group_filter(int argc, const char* const* argv, int& index);
  bool add_group_dot_name_filter(int argc,
      const char* const* argv,
      int& index,
      const String& parameter_name,
      bool strict,
      bool exclude);
  void add_strict_group_filter(int argc, const char* const* argv, int& index);
  void add_exclude_group_filter(int argc, const char* const* argv, int& index);
  void add_exclude_strict_group_filter(int argc,
      const char* const* argv,
      int& index);
  void add_name_filter(int argc, const char* const* argv, int& index);
  void add_strict_name_filter(int argc, const char* const* argv, int& index);
  void add_exclude_name_filter(int argc, const char* const* argv, int& index);
  void add_exclude_strict_name_filter(int argc,
      const char* const* argv,
      int& index);
  void add_test_to_run_based_on_verbose_output(int argc,
      const char* const* argv,
      int& index,
      const char* parameter_name);
  bool set_output_type(int argc, const char* const* argv, int& index);
  void set_package_name(int argc, const char* const* argv, int& index);

  CommandLineArguments(const CommandLineArguments&);
  CommandLineArguments& operator=(const CommandLineArguments&);
};

} // namespace cpputest

#endif
