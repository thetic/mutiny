#include "mu/tiny/test/CommandLineArguments.hpp"

#include "mu/tiny/test/Plugin.hpp"
#include "mu/tiny/test/Registry.hpp"

#include "mu/tiny/StringCollection.hpp"
#include "mu/tiny/time.hpp"
#include "mu/tiny/version.h"

namespace mu {
namespace tiny {
namespace test {

namespace {
String get_parameter_field(
    int argc,
    const char* const* argv,
    int& i,
    const String& parameter_name
)
{
  size_t parameter_length = parameter_name.size();
  String parameter(argv[i]);
  if (parameter.size() > parameter_length) {
    return argv[i] + parameter_length;
  }
  if (i + 1 < argc) {
    return argv[++i];
  }
  return "";
}

String sub_string_from_till(
    const String& str,
    char start_char,
    char last_excluded_char
)
{
  size_t begin_pos = str.find(start_char);
  if (begin_pos == String::npos) {
    return "";
  }

  size_t end_pos = str.find(last_excluded_char, begin_pos);
  if (end_pos == String::npos) {
    return str.substr(begin_pos);
  }

  return str.substr(begin_pos, end_pos - begin_pos);
}
} // namespace

CommandLineArguments::CommandLineArguments(int argc, const char* const* argv)
  : ac_(argc)
  , av_(argv)

{
}

CommandLineArguments::~CommandLineArguments()
{
  while (group_filters_ != nullptr) {
    Filter* current = group_filters_;
    group_filters_ = group_filters_->get_next();
    delete current;
  }
  while (name_filters_ != nullptr) {
    Filter* current = name_filters_;
    name_filters_ = name_filters_->get_next();
    delete current;
  }
}

bool CommandLineArguments::parse_simple_flag(const String& argument)
{
  if (argument == "-h") {
    need_help_ = true;
    return true;
  }
  if (argument == "-v") {
    verbose_ = true;
    return true;
  }
  if (argument == "-vv") {
    very_verbose_ = true;
    return true;
  }
  if (argument == "-c") {
    color_ = true;
    return true;
  }
  if (argument == "-b") {
    reversing_ = true;
    return true;
  }
  if (argument == "-lg") {
    list_test_group_names_ = true;
    return true;
  }
  if (argument == "-ln") {
    list_test_group_and_case_names_ = true;
    return true;
  }
  if (argument == "-lo") {
    list_ordered_test_locations_ = true;
    return true;
  }
  if (argument == "-lgl") {
    list_test_group_locations_ = true;
    return true;
  }
  if (argument == "-ll") {
    list_test_locations_ = true;
    return true;
  }
  if (argument == "-rs") {
    run_skipped_ = true;
    return true;
  }
  if (argument == "-f") {
    crash_on_fail_ = true;
    return true;
  }
  if (argument == "-e") {
    rethrow_exceptions_ = false;
    return true;
  }
  return false;
}

bool CommandLineArguments::parse_prefix_arg(
    const String& argument,
    Plugin* plugin,
    int& index
)
{
  if (string_starts_with(argument, "-r")) {
    set_repeat_count(ac_, av_, index);
    return true;
  }
  if (string_starts_with(argument, "-g")) {
    add_group_filter(ac_, av_, index);
    return true;
  }
  if (string_starts_with(argument, "-t")) {
    return add_group_dot_name_filter(ac_, av_, index, "-t", false, false);
  }
  if (string_starts_with(argument, "-st")) {
    return add_group_dot_name_filter(ac_, av_, index, "-st", true, false);
  }
  if (string_starts_with(argument, "-xt")) {
    return add_group_dot_name_filter(ac_, av_, index, "-xt", false, true);
  }
  if (string_starts_with(argument, "-xst")) {
    return add_group_dot_name_filter(ac_, av_, index, "-xst", true, true);
  }
  if (string_starts_with(argument, "-sg")) {
    add_strict_group_filter(ac_, av_, index);
    return true;
  }
  if (string_starts_with(argument, "-xg")) {
    add_exclude_group_filter(ac_, av_, index);
    return true;
  }
  if (string_starts_with(argument, "-xsg")) {
    add_exclude_strict_group_filter(ac_, av_, index);
    return true;
  }
  if (string_starts_with(argument, "-n")) {
    add_name_filter(ac_, av_, index);
    return true;
  }
  if (string_starts_with(argument, "-sn")) {
    add_strict_name_filter(ac_, av_, index);
    return true;
  }
  if (string_starts_with(argument, "-xn")) {
    add_exclude_name_filter(ac_, av_, index);
    return true;
  }
  if (string_starts_with(argument, "-xsn")) {
    add_exclude_strict_name_filter(ac_, av_, index);
    return true;
  }
  if (string_starts_with(argument, "-s")) {
    return set_shuffle(ac_, av_, index);
  }
  if (string_starts_with(argument, "TEST(")) {
    add_test_to_run_based_on_verbose_output(ac_, av_, index, "TEST(");
    return true;
  }
  if (string_starts_with(argument, "SKIPPED_TEST(")) {
    add_test_to_run_based_on_verbose_output(ac_, av_, index, "SKIPPED_TEST(");
    return true;
  }
  if (string_starts_with(argument, "-p")) {
    return plugin->parse_all_arguments(ac_, av_, index);
  }
  return false;
}

bool CommandLineArguments::parse_argument(
    const String& argument,
    Plugin* plugin,
    int& index
)
{
  if (parse_simple_flag(argument)) {
    return true;
  }
  return parse_prefix_arg(argument, plugin, index);
}

bool CommandLineArguments::parse(Plugin* plugin)
{
  for (int i = 1; i < ac_; i++) {
    if (!parse_argument(av_[i], plugin, i)) {
      return false;
    }
  }
  return true;
}

String CommandLineArguments::help()
{
  String help_str =
      "mutiny v" MUTINY_VERSION_STRING "\n\n"
      "Options that do not run tests but query:\n"
      "  -h                - this wonderful help screen. Joy!\n"
      "  -lg               - print a list of group names, separated by "
      "spaces\n"
      "  -ln               - print a list of test names in the form of "
      "group.name, separated by spaces\n"
      "  -ll               - print a list of test names in the form of "
      "group.name.test_file_path.line\n"
      "  -lo               - print a list of ordered test names in the form "
      "of group.name.test_file_path.line\n"
      "  -lgl              - print a list of group locations in the form of "
      "group.file_path.line\n"
      "\n"
      "Options that change the output format:\n"
      "  -c                - colorize output, print green if OK, or red if "
      "failed\n"
      "  -v                - verbose, print each test name as it runs\n"
      "  -vv               - very verbose, print internal information "
      "during test run\n";

  Plugin* plugin = Registry::get_current_registry()->get_first_plugin();
  String plugin_help = plugin->get_all_help();

  if (!plugin_help.empty()) {
    help_str += "\nOptions that are provided by plugins:\n";
    help_str += plugin_help;
  }

  help_str +=
      "\n"
      "Options that control which tests are run:\n"
      "  -g <group>        - only run tests whose group contains <group>\n"
      "  -n <name>         - only run tests whose name contains <name>\n"
      "  -t <group>.<name> - only run tests whose group and name contain "
      "<group> and <name>\n"
      "  -sg <group>       - only run tests whose group exactly matches "
      "<group>\n"
      "  -sn <name>        - only run tests whose name exactly matches "
      "<name>\n"
      "  -st <grp>.<name>  - only run tests whose group and name exactly "
      "match <grp> and <name>\n"
      "  -xg <group>       - exclude tests whose group contains <group>\n"
      "  -xn <name>        - exclude tests whose name contains <name>\n"
      "  -xt <grp>.<name>  - exclude tests whose group and name contain "
      "<grp> and <name>\n"
      "  -xsg <group>      - exclude tests whose group exactly matches "
      "<group>\n"
      "  -xsn <name>       - exclude tests whose name exactly matches "
      "<name>\n"
      "  -xst <grp>.<name> - exclude tests whose group and name exactly "
      "match <grp> and <name>\n"
      "  \"[SKIPPED_]TEST(<group>, <name>)\"\n"
      "                    - only run tests whose group and name exactly "
      "match <group> and <name>\n"
      "                      (this can be used to copy-paste output from "
      "the -v option on the command line)\n"
      "\n"
      "Options that control how the tests are run:\n"
      "  -b                - run the tests backwards, reversing the normal "
      "way\n"
      "  -s [<seed>]       - shuffle tests randomly (randomization seed is "
      "optional, must be greater than 0)\n"
      "  -r[<#>]           - repeat the tests <#> times (or twice if <#> is "
      "not specified)\n"
      "  -rs               - run skipped tests as if they are not skipped\n"
      "  -f                - Cause the tests to crash on failure (to allow "
      "the test to be debugged if necessary)\n"
      "  -e                - do not rethrow unexpected exceptions on "
      "failure\n";

  return help_str;
}

bool CommandLineArguments::need_help() const
{
  return need_help_;
}

bool CommandLineArguments::is_verbose() const
{
  return verbose_;
}

bool CommandLineArguments::is_very_verbose() const
{
  return very_verbose_;
}

bool CommandLineArguments::is_color() const
{
  return color_;
}

bool CommandLineArguments::is_listing_test_group_names() const
{
  return list_test_group_names_;
}

bool CommandLineArguments::is_listing_test_group_and_case_names() const
{
  return list_test_group_and_case_names_;
}

bool CommandLineArguments::is_listing_test_locations() const
{
  return list_test_locations_;
}

bool CommandLineArguments::is_listing_ordered_test_locations() const
{
  return list_ordered_test_locations_;
}

bool CommandLineArguments::is_listing_test_group_locations() const
{
  return list_test_group_locations_;
}

bool CommandLineArguments::is_run_skipped() const
{
  return run_skipped_;
}

unsigned int CommandLineArguments::get_repeat_count() const
{
  return repeat_;
}

bool CommandLineArguments::is_reversing() const
{
  return reversing_;
}

bool CommandLineArguments::is_crashing_on_fail() const
{
  return crash_on_fail_;
}

bool CommandLineArguments::is_rethrowing_exceptions() const
{
  return rethrow_exceptions_;
}

bool CommandLineArguments::is_shuffling() const
{
  return shuffling_;
}

unsigned int CommandLineArguments::get_shuffle_seed() const
{
  return shuffle_seed_;
}

const Filter* CommandLineArguments::get_group_filters() const
{
  return group_filters_;
}

const Filter* CommandLineArguments::get_name_filters() const
{
  return name_filters_;
}

void CommandLineArguments::set_repeat_count(
    int argc,
    const char* const* argv,
    int& i
)
{
  repeat_ = 0;

  String repeat_parameter(argv[i]);
  if (repeat_parameter.size() > 2) {
    repeat_ = static_cast<unsigned int>(strtol(argv[i] + 2));
  } else if (i + 1 < argc) {
    repeat_ = static_cast<unsigned int>(strtol(argv[i + 1]));
    if (repeat_ != 0) {
      i++;
    }
  }

  if (0 == repeat_) {
    repeat_ = 2;
  }
}

bool CommandLineArguments::set_shuffle(
    int argc,
    const char* const* argv,
    int& i
)
{
  shuffling_ = true;
  shuffle_seed_ = static_cast<unsigned int>(get_time_in_millis());
  if (shuffle_seed_ == 0) {
    shuffle_seed_++;
  }

  String shuffle_parameter = argv[i];
  if (shuffle_parameter.size() > 2) {
    shuffling_pre_seeded_ = true;
    shuffle_seed_ = static_cast<unsigned>(strtoul(argv[i] + 2));
  } else if (i + 1 < argc) {
    auto parsed_parameter = static_cast<unsigned>(strtoul(argv[i + 1]));
    if (parsed_parameter != 0) {
      shuffling_pre_seeded_ = true;
      shuffle_seed_ = parsed_parameter;
      i++;
    }
  }
  return (shuffle_seed_ != 0);
}

void CommandLineArguments::add_group_filter(
    int argc,
    const char* const* argv,
    int& i
)
{
  auto* group_filter = new Filter(get_parameter_field(argc, argv, i, "-g"));
  group_filters_ = group_filter->add(group_filters_);
}

bool CommandLineArguments::add_group_dot_name_filter(
    int argc,
    const char* const* argv,
    int& i,
    const String& parameter_name,
    bool strict,
    bool exclude
)
{
  String group_dot_name = get_parameter_field(argc, argv, i, parameter_name);
  StringCollection collection(group_dot_name, '.');

  if (collection.size() != 2) {
    return false;
  }

  auto* group_filter =
      new Filter(collection[0].substr(0, collection[0].size() - 1));
  auto* name_filter = new Filter(collection[1]);
  if (strict) {
    group_filter->strict_matching();
    name_filter->strict_matching();
  }
  if (exclude) {
    group_filter->invert_matching();
    name_filter->invert_matching();
  }
  group_filters_ = group_filter->add(group_filters_);
  name_filters_ = name_filter->add(name_filters_);
  return true;
}

void CommandLineArguments::add_strict_group_filter(
    int argc,
    const char* const* argv,
    int& i
)
{
  auto* group_filter = new Filter(get_parameter_field(argc, argv, i, "-sg"));
  group_filter->strict_matching();
  group_filters_ = group_filter->add(group_filters_);
}

void CommandLineArguments::add_exclude_group_filter(
    int argc,
    const char* const* argv,
    int& i
)
{
  auto* group_filter = new Filter(get_parameter_field(argc, argv, i, "-xg"));
  group_filter->invert_matching();
  group_filters_ = group_filter->add(group_filters_);
}

void CommandLineArguments::add_exclude_strict_group_filter(
    int argc,
    const char* const* argv,
    int& i
)
{
  auto* group_filter = new Filter(get_parameter_field(argc, argv, i, "-xsg"));
  group_filter->strict_matching();
  group_filter->invert_matching();
  group_filters_ = group_filter->add(group_filters_);
}

void CommandLineArguments::add_name_filter(
    int argc,
    const char* const* argv,
    int& i
)
{
  auto* name_filter = new Filter(get_parameter_field(argc, argv, i, "-n"));
  name_filters_ = name_filter->add(name_filters_);
}

void CommandLineArguments::add_strict_name_filter(
    int argc,
    const char* const* argv,
    int& index
)
{
  auto* name_filter = new Filter(get_parameter_field(argc, argv, index, "-sn"));
  name_filter->strict_matching();
  name_filters_ = name_filter->add(name_filters_);
}

void CommandLineArguments::add_exclude_name_filter(
    int argc,
    const char* const* argv,
    int& index
)
{
  auto* name_filter = new Filter(get_parameter_field(argc, argv, index, "-xn"));
  name_filter->invert_matching();
  name_filters_ = name_filter->add(name_filters_);
}

void CommandLineArguments::add_exclude_strict_name_filter(
    int argc,
    const char* const* argv,
    int& index
)
{
  auto* name_filter =
      new Filter(get_parameter_field(argc, argv, index, "-xsn"));
  name_filter->invert_matching();
  name_filter->strict_matching();
  name_filters_ = name_filter->add(name_filters_);
}

void CommandLineArguments::add_test_to_run_based_on_verbose_output(
    int argc,
    const char* const* argv,
    int& index,
    const char* parameter_name
)
{
  String wholename = get_parameter_field(argc, argv, index, parameter_name);
  String testname = sub_string_from_till(wholename, ',', ')');
  testname = testname.substr(2);
  auto* namefilter = new Filter(testname);
  auto* groupfilter =
      new Filter(sub_string_from_till(wholename, wholename[0], ','));
  namefilter->strict_matching();
  groupfilter->strict_matching();
  group_filters_ = groupfilter->add(group_filters_);
  name_filters_ = namefilter->add(name_filters_);
}

} // namespace test
} // namespace tiny
} // namespace mu
