/**
 * @file
 * @brief Parser for the mutiny command-line argument set.
 *
 * Used internally by @ref mu::tiny::test::CommandLineRunner; builds @ref
 * mu::tiny::test::Filter chains from `-g` / `-n` flags and exposes the parsed
 * state via accessors.
 */

#ifndef INCLUDED_MU_TINY_TEST_COMMANDLINEARGUMENTS_HPP
#define INCLUDED_MU_TINY_TEST_COMMANDLINEARGUMENTS_HPP

#include "mu/tiny/test/Filter.hpp"
#include "mu/tiny/test/Output.hpp"

#include "mu/tiny/String.hpp"
#include "mu/tiny/export.h"

namespace mu {
namespace tiny {
namespace test {

class Plugin;

/**
 * @brief Parses and stores the mutiny command-line options.
 *
 * Pass the raw `argc`/`argv` from `main()` to the constructor, then call
 * @ref parse() (optionally passing a @ref Plugin chain for plugin-specific
 * arguments). Query the resulting state with the `is_*()` and `get_*()`
 * accessors. Used internally by @ref CommandLineRunner.
 */
class MUTINY_EXPORT CommandLineArguments
{
public:
  /**
   * @brief Construct from main's argument list.
   *
   * @param argc  Argument count.
   * @param argv  Argument vector.
   */
  explicit CommandLineArguments(int argc, const char* const* argv);
  virtual ~CommandLineArguments();
  CommandLineArguments(const CommandLineArguments&) = delete;
  CommandLineArguments& operator=(const CommandLineArguments&) = delete;

  /**
   * @brief Parse all arguments, delegating unknown ones to @p plugin.
   *
   * @param plugin  Plugin chain to receive unrecognised arguments; may be
   *                nullptr.
   * @return true if parsing succeeded, false if an unrecognised argument was
   *         found.
   */
  bool parse(Plugin* plugin);

  /** @return true if `-h` / `--help` was passed. */
  bool need_help() const;
  /** @return true if verbose output was requested (`-v` / `--verbose`). */
  bool is_verbose() const;
  /**
   * @return true if very-verbose output was requested (`-vv` / `--verbose=2`).
   */
  bool is_very_verbose() const;
  /** @return true if coloured output was requested (`-c` / `--color`). */
  bool is_color() const;
  /** @return true if `--list-groups` was passed. */
  bool is_listing_test_group_names() const;
  /** @return true if `--list-tests` was passed. */
  bool is_listing_test_group_and_case_names() const;
  /** @return true if `--list-locations` was passed. */
  bool is_listing_test_locations() const;
  /** @return true if `--list-ordered-locations` was passed. */
  bool is_listing_ordered_test_locations() const;
  /** @return true if `--list-group-locations` was passed. */
  bool is_listing_test_group_locations() const;
  /** @return true if `--run-skipped` was passed. */
  bool is_run_skipped() const;
  /** @return The number of times to repeat the full test suite (`-r N`). */
  unsigned int get_repeat_count() const;
  /** @return true if `-s` (shuffle) was passed. */
  bool is_shuffling() const;
  /** @return true if tests should run in reverse order (`-rv`). */
  bool is_reversing() const;
  /**
   * @return true if the process should crash on first failure (`-f` /
   * `--crash-on-fail`).
   */
  bool is_crashing_on_fail() const;
  /**
   * @return true if exceptions should be re-thrown after being caught
   * (`--no-rethrow` disables).
   */
  bool is_rethrowing_exceptions() const;
  /** @return The seed used when shuffling (0 means time-seeded). */
  unsigned int get_shuffle_seed() const;
  /** @return The head of the group filter chain, or nullptr. */
  const Filter* get_group_filters() const;
  /** @return The head of the name filter chain, or nullptr. */
  const Filter* get_name_filters() const;
  /** @return A formatted help string describing all supported arguments. */
  static String help();

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
  bool list_test_group_locations_{ false };
  bool run_skipped_{ false };
  bool reversing_{ false };
  bool crash_on_fail_{ false };
  bool rethrow_exceptions_{ true };
  bool shuffling_{ false };
  bool shuffling_pre_seeded_{ false };
  unsigned int repeat_{ 1 };
  unsigned int shuffle_seed_{ 0 };
  Filter* group_filters_{ nullptr };
  Filter* name_filters_{ nullptr };
  int set_repeat_count(
      int argc,
      const char* const* argv,
      const String& flag = "-r"
  );
  int set_shuffle(int argc, const char* const* argv, const String& flag = "-s");
  int add_group_filter(
      int argc,
      const char* const* argv,
      const String& flag = "-g"
  );
  int add_group_dot_name_filter(
      int argc,
      const char* const* argv,
      const String& parameter_name,
      bool strict,
      bool exclude
  );
  int add_strict_group_filter(int argc, const char* const* argv);
  int add_exclude_group_filter(int argc, const char* const* argv);
  int add_exclude_strict_group_filter(int argc, const char* const* argv);
  int add_name_filter(
      int argc,
      const char* const* argv,
      const String& flag = "-n"
  );
  int add_strict_name_filter(int argc, const char* const* argv);
  int add_exclude_name_filter(int argc, const char* const* argv);
  int add_exclude_strict_name_filter(int argc, const char* const* argv);
  int add_test_to_run_based_on_verbose_output(
      int argc,
      const char* const* argv,
      const char* parameter_name
  );
  bool parse_simple_flag(const String& argument);
  int parse_prefix_arg(int argc, const char* const* argv, Plugin* plugin);
  int parse_argument(int argc, const char* const* argv, Plugin* plugin);
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
