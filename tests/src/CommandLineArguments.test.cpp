#include "mu/tiny/test/CommandLineArguments.hpp"

#include "mu/tiny/test/Registry.hpp"

#include "mu/tiny/String.hpp"
#include "mu/tiny/test.hpp"

namespace {
class OptionsPlugin : public mu::tiny::test::Plugin
{
public:
  OptionsPlugin(const mu::tiny::String& name)
    : Plugin(name)
  {
  }
  ~OptionsPlugin() override = default;
  bool parse_arguments(int /*argc*/, const char* const* /*argv*/) override
  {
    return true;
  }
};

class HelpPlugin : public mu::tiny::test::Plugin
{
public:
  HelpPlugin()
    : Plugin("help")
  {
  }
  mu::tiny::String get_help() const override
  {
    return "  -phelp             - help text\n";
  }
};
} // namespace

TEST_GROUP(CommandLineArguments)
{
  mu::tiny::test::CommandLineArguments* args;
  OptionsPlugin* plugin;

  void setup() override
  {
    plugin = new OptionsPlugin("options");
    args = nullptr;
  }
  void teardown() override
  {
    delete args;
    delete plugin;
  }

  bool new_argument_parser(int argc, const char* const* argv)
  {
    args = new mu::tiny::test::CommandLineArguments(argc, argv);
    return args->parse(plugin);
  }
};

TEST(CommandLineArguments, Create) {}

TEST(CommandLineArguments, verboseSetMultipleParameters)
{
  const char* argv[] = { "tests.exe", "-v" };
  CHECK(new_argument_parser(2, argv));
  CHECK(args->is_verbose());
}

TEST(CommandLineArguments, veryVerbose)
{
  const char* argv[] = { "tests.exe", "-vv" };
  CHECK(new_argument_parser(2, argv));
  CHECK(args->is_very_verbose());
}

TEST(CommandLineArguments, setColor)
{
  const char* argv[] = { "tests.exe", "-c" };
  CHECK(new_argument_parser(2, argv));
  CHECK(args->is_color());
}

TEST(CommandLineArguments, repeatSet)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-r3" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_EQUAL(3U, args->get_repeat_count());
}

TEST(CommandLineArguments, repeatSetDifferentParameter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-r", "4" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_EQUAL(4U, args->get_repeat_count());
}

TEST(CommandLineArguments, repeatSetDefaultsToTwoAndShuffleDisabled)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-r" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_EQUAL(2U, args->get_repeat_count());
}

TEST(CommandLineArguments, reverseEnabled)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-b" };
  CHECK(new_argument_parser(argc, argv));
  CHECK(args->is_reversing());
}

TEST(CommandLineArguments, shuffleDisabledByDefault)
{
  int argc = 1;
  const char* argv[] = { "tests.exe" };
  CHECK(new_argument_parser(argc, argv));
  CHECK(!args->is_shuffling());
}

TEST(CommandLineArguments, shuffleEnabled)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-s" };
  CHECK(new_argument_parser(argc, argv));
  CHECK(args->is_shuffling());
}

TEST(CommandLineArguments, shuffleWithSeedZeroIsOk)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-s0" };
  CHECK(!new_argument_parser(argc, argv));
  CHECK_EQUAL(0U, args->get_shuffle_seed());
}

TEST(CommandLineArguments, shuffleEnabledSpecificSeedCase1)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-s999" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_EQUAL(999U, args->get_shuffle_seed());
}

TEST(CommandLineArguments, shuffleEnabledSpecificSeedCase2)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-s 888" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_EQUAL(888U, args->get_shuffle_seed());
}

TEST(CommandLineArguments, shuffleEnabledSpecificSeedCase3)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-s", "777" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_EQUAL(777U, args->get_shuffle_seed());
}

TEST(CommandLineArguments, shuffleBeforeDoesNotDisturbOtherSwitch)
{
  int argc = 4;
  const char* argv[] = { "tests.exe", "-s", "--exact-group", "group" };
  CHECK(new_argument_parser(argc, argv));
  mu::tiny::test::Filter group_filter("group");
  group_filter.strict_matching();
  CHECK_EQUAL(group_filter, *args->get_group_filters());
  CHECK(args->is_shuffling());
}

TEST(CommandLineArguments, setGroupFilter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-g", "group" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_EQUAL(mu::tiny::test::Filter("group"), *args->get_group_filters());
}

TEST(CommandLineArguments, setGroupFilterWithNoValueUsesEmptyFilter)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-g" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_EQUAL(mu::tiny::test::Filter(""), *args->get_group_filters());
}

TEST(CommandLineArguments, setCompleteGroupDotNameFilterInvalidArgument)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-t", "groupname" };
  CHECK(!new_argument_parser(argc, argv));
}

TEST(CommandLineArguments, setCompleteGroupDotNameFilter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-t", "group.name" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_EQUAL(mu::tiny::test::Filter("group"), *args->get_group_filters());
  CHECK_EQUAL(mu::tiny::test::Filter("name"), *args->get_name_filters());
}

TEST(CommandLineArguments, setCompleteStrictGroupDotNameFilterInvalidArgument)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "--exact-test", "groupname" };
  CHECK(!new_argument_parser(argc, argv));
}

TEST(CommandLineArguments, setCompleteStrictGroupDotNameFilter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "--exact-test", "group.name" };
  CHECK(new_argument_parser(argc, argv));
  mu::tiny::test::Filter group_filter("group");
  group_filter.strict_matching();
  CHECK_EQUAL(group_filter, *args->get_group_filters());
  mu::tiny::test::Filter name_filter("name");
  name_filter.strict_matching();
  CHECK_EQUAL(name_filter, *args->get_name_filters());
}

TEST(CommandLineArguments, setCompleteExcludeGroupDotNameFilterInvalidArgument)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "--exclude-test", "groupname" };
  CHECK(!new_argument_parser(argc, argv));
}

TEST(CommandLineArguments, setCompleteExcludeGroupDotNameFilter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "--exclude-test", "group.name" };
  CHECK(new_argument_parser(argc, argv));
  mu::tiny::test::Filter group_filter("group");
  group_filter.invert_matching();
  CHECK_EQUAL(group_filter, *args->get_group_filters());
  mu::tiny::test::Filter name_filter("name");
  name_filter.invert_matching();
  CHECK_EQUAL(name_filter, *args->get_name_filters());
}

TEST(CommandLineArguments, setCompleteExcludeStrictGroupDotNameFilterInvalidArgument)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "--exclude-exact-test", "groupname" };
  CHECK(!new_argument_parser(argc, argv));
}

TEST(CommandLineArguments, setCompleteExcludeStrictGroupDotNameFilter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "--exclude-exact-test", "group.name" };
  CHECK(new_argument_parser(argc, argv));
  mu::tiny::test::Filter group_filter("group");
  group_filter.strict_matching();
  group_filter.invert_matching();
  CHECK_EQUAL(group_filter, *args->get_group_filters());
  mu::tiny::test::Filter name_filter("name");
  name_filter.strict_matching();
  name_filter.invert_matching();
  CHECK_EQUAL(name_filter, *args->get_name_filters());
}

TEST(CommandLineArguments, setGroupFilterSameParameter)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-ggroup" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_EQUAL(mu::tiny::test::Filter("group"), *args->get_group_filters());
}

TEST(CommandLineArguments, setStrictGroupFilter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "--exact-group", "group" };
  CHECK(new_argument_parser(argc, argv));
  mu::tiny::test::Filter group_filter("group");
  group_filter.strict_matching();
  CHECK_EQUAL(group_filter, *args->get_group_filters());
}

TEST(CommandLineArguments, setStrictGroupFilterEqualsSyntax)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "--exact-group=group" };
  CHECK(new_argument_parser(argc, argv));
  mu::tiny::test::Filter group_filter("group");
  group_filter.strict_matching();
  CHECK_EQUAL(group_filter, *args->get_group_filters());
}

TEST(CommandLineArguments, setExcludeGroupFilter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "--exclude-group", "group" };
  CHECK(new_argument_parser(argc, argv));
  mu::tiny::test::Filter group_filter("group");
  group_filter.invert_matching();
  CHECK_EQUAL(group_filter, *args->get_group_filters());
}

TEST(CommandLineArguments, setExcludeGroupFilterEqualsSyntax)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "--exclude-group=group" };
  CHECK(new_argument_parser(argc, argv));
  mu::tiny::test::Filter group_filter("group");
  group_filter.invert_matching();
  CHECK_EQUAL(group_filter, *args->get_group_filters());
}

TEST(CommandLineArguments, setExcludeStrictGroupFilter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "--exclude-exact-group", "group" };
  CHECK(new_argument_parser(argc, argv));
  mu::tiny::test::Filter group_filter("group");
  group_filter.invert_matching();
  group_filter.strict_matching();
  CHECK_EQUAL(group_filter, *args->get_group_filters());
}

TEST(CommandLineArguments, setExcludeStrictGroupFilterEqualsSyntax)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "--exclude-exact-group=group" };
  CHECK(new_argument_parser(argc, argv));
  mu::tiny::test::Filter group_filter("group");
  group_filter.invert_matching();
  group_filter.strict_matching();
  CHECK_EQUAL(group_filter, *args->get_group_filters());
}

TEST(CommandLineArguments, setNameFilter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-n", "name" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_EQUAL(mu::tiny::test::Filter("name"), *args->get_name_filters());
}

TEST(CommandLineArguments, setNameFilterSameParameter)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-nname" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_EQUAL(mu::tiny::test::Filter("name"), *args->get_name_filters());
}

TEST(CommandLineArguments, setStrictNameFilter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "--exact-name", "name" };
  CHECK(new_argument_parser(argc, argv));
  mu::tiny::test::Filter name_filter("name");
  name_filter.strict_matching();
  CHECK_EQUAL(name_filter, *args->get_name_filters());
}

TEST(CommandLineArguments, setStrictNameFilterEqualsSyntax)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "--exact-name=name" };
  CHECK(new_argument_parser(argc, argv));
  mu::tiny::test::Filter name_filter("name");
  name_filter.strict_matching();
  CHECK_EQUAL(name_filter, *args->get_name_filters());
}

TEST(CommandLineArguments, setExcludeNameFilter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "--exclude-name", "name" };
  CHECK(new_argument_parser(argc, argv));
  mu::tiny::test::Filter name_filter("name");
  name_filter.invert_matching();
  CHECK_EQUAL(name_filter, *args->get_name_filters());
}

TEST(CommandLineArguments, setExcludeNameFilterEqualsSyntax)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "--exclude-name=name" };
  CHECK(new_argument_parser(argc, argv));
  mu::tiny::test::Filter name_filter("name");
  name_filter.invert_matching();
  CHECK_EQUAL(name_filter, *args->get_name_filters());
}

TEST(CommandLineArguments, setExcludeStrictNameFilter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "--exclude-exact-name", "name" };
  CHECK(new_argument_parser(argc, argv));
  mu::tiny::test::Filter name_filter("name");
  name_filter.invert_matching();
  name_filter.strict_matching();
  CHECK_EQUAL(name_filter, *args->get_name_filters());
}

TEST(CommandLineArguments, setExcludeStrictNameFilterEqualsSyntax)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "--exclude-exact-name=name" };
  CHECK(new_argument_parser(argc, argv));
  mu::tiny::test::Filter name_filter("name");
  name_filter.invert_matching();
  name_filter.strict_matching();
  CHECK_EQUAL(name_filter, *args->get_name_filters());
}

TEST(CommandLineArguments, setTestToRunUsingVerboseOutput)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "TEST(testgroup, testname) - stuff" };
  CHECK(new_argument_parser(argc, argv));

  mu::tiny::test::Filter name_filter("testname");
  mu::tiny::test::Filter group_filter("testgroup");
  name_filter.strict_matching();
  group_filter.strict_matching();
  CHECK_EQUAL(name_filter, *args->get_name_filters());
  CHECK_EQUAL(group_filter, *args->get_group_filters());
}

TEST(CommandLineArguments, setTestToRunUsingVerboseOutputOfIgnoreTest)
{
  int argc = 2;
  const char* argv[] = { "tests.exe",
                         "SKIPPED_TEST(testgroup, testname) - stuff" };
  CHECK(new_argument_parser(argc, argv));

  mu::tiny::test::Filter name_filter("testname");
  mu::tiny::test::Filter group_filter("testgroup");
  name_filter.strict_matching();
  group_filter.strict_matching();
  CHECK_EQUAL(name_filter, *args->get_name_filters());
  CHECK_EQUAL(group_filter, *args->get_group_filters());
}

TEST(CommandLineArguments, setPrintGroups)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "--list-groups" };
  CHECK(new_argument_parser(argc, argv));
  CHECK(args->is_listing_test_group_names());
}

TEST(CommandLineArguments, setPrintGroupsAndNames)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "--list-tests" };
  CHECK(new_argument_parser(argc, argv));
  CHECK(args->is_listing_test_group_and_case_names());
}

TEST(CommandLineArguments, setPrintGroupLocations)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "--list-group-locations" };
  CHECK(new_argument_parser(argc, argv));
  CHECK(args->is_listing_test_group_locations());
}

TEST(CommandLineArguments, weirdParamatersReturnsFalse)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-SomethingWeird" };
  CHECK(!new_argument_parser(argc, argv));
}

TEST(CommandLineArguments, helpPrintsTheHelp)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-h" };
  CHECK(new_argument_parser(argc, argv));
  CHECK(args->need_help());
}

TEST(CommandLineArguments, pluginHelp)
{
  mu::tiny::test::Registry local_reg;
  local_reg.set_current_registry(&local_reg);
  HelpPlugin help_plugin;
  local_reg.install_plugin(&help_plugin);
  mu::tiny::String help_str = mu::tiny::test::CommandLineArguments::help();
  local_reg.set_current_registry(nullptr);
  CHECK(
      help_str.find("Options that are provided by plugins:") !=
      mu::tiny::String::npos
  );
  CHECK(
      help_str.find("-phelp             - help text") != mu::tiny::String::npos
  );
}

TEST(CommandLineArguments, pluginKnowsOption)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-pPluginOption" };
  CHECK(new_argument_parser(argc, argv));
}

TEST(CommandLineArguments, checkDefaultArguments)
{
  int argc = 1;
  const char* argv[] = { "tests.exe" };
  CHECK(new_argument_parser(argc, argv));
  CHECK(!args->is_verbose());
  CHECK_EQUAL(1U, args->get_repeat_count());
  CHECK(nullptr == args->get_group_filters());
  CHECK(nullptr == args->get_name_filters());
  CHECK(!args->is_crashing_on_fail());
  CHECK(args->is_rethrowing_exceptions());
}

TEST(CommandLineArguments, lotsOfGroupsAndTests)
{
  int argc = 10;
  const char* argv[] = { "tests.exe",
                         "--exact-group=group1",
                         "--exclude-name=test1",
                         "--exact-group=group2",
                         "--exact-name=test2",
                         "--exact-name=test3",
                         "--exact-group=group3",
                         "--exact-name=test4",
                         "--exact-group=group4",
                         "--exact-name=test5" };
  CHECK(new_argument_parser(argc, argv));
  mu::tiny::test::Filter name_filter("test1");
  name_filter.invert_matching();
  mu::tiny::test::Filter group_filter("group1");
  group_filter.strict_matching();
  CHECK_EQUAL(
      name_filter,
      *args->get_name_filters()->get_next()->get_next()->get_next()->get_next()
  );
  CHECK_EQUAL(
      group_filter,
      *args->get_group_filters()->get_next()->get_next()->get_next()
  );
}

TEST(CommandLineArguments, setOptRun)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "--run-skipped" };
  CHECK(new_argument_parser(argc, argv));
  CHECK(args->is_run_skipped());
}

TEST(CommandLineArguments, setOptCrashOnFail)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-f" };
  CHECK(new_argument_parser(argc, argv));
  CHECK(args->is_crashing_on_fail());
}

TEST(CommandLineArguments, testVerboseOutputMissingClosingBracketSucceeds)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "TEST(Group, Name" };
  CHECK(new_argument_parser(argc, argv));
}

TEST(CommandLineArguments, setOptRethrowExceptions)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-e" };
  CHECK(new_argument_parser(argc, argv));
  CHECK(!args->is_rethrowing_exceptions());
}
