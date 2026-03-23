#ifndef INCLUDED_MUTINY_TEST_COMMANDLINEARGUMENTS_HPP
#define INCLUDED_MUTINY_TEST_COMMANDLINEARGUMENTS_HPP

#include "mutiny/test/Filter.hpp"
#include "mutiny/test/Output.hpp"
#include "mutiny/test/String.hpp"

namespace mu {
namespace tiny {
namespace test {

class Plugin;

class CommandLineArguments
{
public:
  explicit CommandLineArguments(int argc, const char* const* argv);
  virtual ~CommandLineArguments();
  CommandLineArguments(const CommandLineArguments&) = delete;
  CommandLineArguments& operator=(const CommandLineArguments&) = delete;

  bool parse(Plugin* plugin);
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
  const Filter* get_group_filters() const;
  const Filter* get_name_filters() const;
  String help() const;

private:
  int ac_;
  const char* const* av_;

  bool need_help_{ false };
  bool verbose_{ false };
  bool very_verbose_{ false };
  bool color_{ false };
  bool list_test_group_names_{ false };
  bool list_test_group_and_case_names_{ false };
  bool list_test_locations_{ false };
  bool list_ordered_test_locations_{ false };
  bool run_ignored_{ false };
  bool reversing_{ false };
  bool crash_on_fail_{ false };
  bool rethrow_exceptions_{ true };
  bool shuffling_{ false };
  bool shuffling_pre_seeded_{ false };
  size_t repeat_{ 1 };
  size_t shuffle_seed_{ 0 };
  Filter* group_filters_{ nullptr };
  Filter* name_filters_{ nullptr };
  String get_parameter_field(
      int argc,
      const char* const* argv,
      int& i,
      const String& parameter_name
  );
  void set_repeat_count(int argc, const char* const* argv, int& index);
  bool set_shuffle(int argc, const char* const* argv, int& index);
  void add_group_filter(int argc, const char* const* argv, int& index);
  bool add_group_dot_name_filter(
      int argc,
      const char* const* argv,
      int& index,
      const String& parameter_name,
      bool strict,
      bool exclude
  );
  void add_strict_group_filter(int argc, const char* const* argv, int& index);
  void add_exclude_group_filter(int argc, const char* const* argv, int& index);
  void add_exclude_strict_group_filter(
      int argc,
      const char* const* argv,
      int& index
  );
  void add_name_filter(int argc, const char* const* argv, int& index);
  void add_strict_name_filter(int argc, const char* const* argv, int& index);
  void add_exclude_name_filter(int argc, const char* const* argv, int& index);
  void add_exclude_strict_name_filter(
      int argc,
      const char* const* argv,
      int& index
  );
  void add_test_to_run_based_on_verbose_output(
      int argc,
      const char* const* argv,
      int& index,
      const char* parameter_name
  );
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
