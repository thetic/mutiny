#include "CppMu/CommandLineArguments.hpp"

#include "CppMu/CppMu.hpp"
#include "CppMu/String.hpp"
#include "CppMu/TestRegistry.hpp"

class OptionsPlugin : public cppmu::TestPlugin
{
public:
  OptionsPlugin(const cppmu::String& name)
    : TestPlugin(name)
  {
  }
  ~OptionsPlugin() override = default;
  bool parse_arguments(int, const char* const*, int) override { return true; }
};

TEST_GROUP(CommandLineArguments)
{
  cppmu::CommandLineArguments* args;
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
    args = new cppmu::CommandLineArguments(argc, argv);
    return args->parse(plugin);
  }
};

TEST(CommandLineArguments, Create)
{
}

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
  LONGS_EQUAL(3, args->get_repeat_count());
}

TEST(CommandLineArguments, repeatSetDifferentParameter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-r", "4" };
  CHECK(new_argument_parser(argc, argv));
  LONGS_EQUAL(4, args->get_repeat_count());
}

TEST(CommandLineArguments, repeatSetDefaultsToTwoAndShuffleDisabled)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-r" };
  CHECK(new_argument_parser(argc, argv));
  LONGS_EQUAL(2, args->get_repeat_count());
}

TEST(CommandLineArguments, reverseEnabled)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-b" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_TRUE(args->is_reversing());
}

TEST(CommandLineArguments, shuffleDisabledByDefault)
{
  int argc = 1;
  const char* argv[] = { "tests.exe" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_FALSE(args->is_shuffling());
}

TEST(CommandLineArguments, shuffleEnabled)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-s" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_TRUE(args->is_shuffling());
}

TEST(CommandLineArguments, shuffleWithSeedZeroIsOk)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-s0" };
  CHECK_FALSE(new_argument_parser(argc, argv));
  CHECK_EQUAL(0, args->get_shuffle_seed());
}

TEST(CommandLineArguments, shuffleEnabledSpecificSeedCase1)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-s999" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_EQUAL(999, args->get_shuffle_seed());
}

TEST(CommandLineArguments, shuffleEnabledSpecificSeedCase2)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-s 888" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_EQUAL(888, args->get_shuffle_seed());
}

TEST(CommandLineArguments, shuffleEnabledSpecificSeedCase3)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-s", "777" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_EQUAL(777, args->get_shuffle_seed());
}

TEST(CommandLineArguments, shuffleBeforeDoesNotDisturbOtherSwitch)
{
  int argc = 4;
  const char* argv[] = { "tests.exe", "-s", "-sg", "group" };
  CHECK(new_argument_parser(argc, argv));
  cppmu::TestFilter group_filter("group");
  group_filter.strict_matching();
  CHECK_EQUAL(group_filter, *args->get_group_filters());
  CHECK_TRUE(args->is_shuffling());
}

TEST(CommandLineArguments, setGroupFilter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-g", "group" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_EQUAL(cppmu::TestFilter("group"), *args->get_group_filters());
}

TEST(CommandLineArguments, setCompleteGroupDotNameFilterInvalidArgument)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-t", "groupname" };
  CHECK_FALSE(new_argument_parser(argc, argv));
}

TEST(CommandLineArguments, setCompleteGroupDotNameFilter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-t", "group.name" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_EQUAL(cppmu::TestFilter("group"), *args->get_group_filters());
  CHECK_EQUAL(cppmu::TestFilter("name"), *args->get_name_filters());
}

TEST(CommandLineArguments, setCompleteStrictGroupDotNameFilterInvalidArgument)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-st", "groupname" };
  CHECK_FALSE(new_argument_parser(argc, argv));
}

TEST(CommandLineArguments, setCompleteStrictGroupDotNameFilter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-st", "group.name" };
  CHECK(new_argument_parser(argc, argv));
  cppmu::TestFilter group_filter("group");
  group_filter.strict_matching();
  CHECK_EQUAL(group_filter, *args->get_group_filters());
  cppmu::TestFilter name_filter("name");
  name_filter.strict_matching();
  CHECK_EQUAL(name_filter, *args->get_name_filters());
}

TEST(CommandLineArguments, setCompleteExcludeGroupDotNameFilterInvalidArgument)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-xt", "groupname" };
  CHECK_FALSE(new_argument_parser(argc, argv));
}

TEST(CommandLineArguments, setCompleteExcludeGroupDotNameFilter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-xt", "group.name" };
  CHECK(new_argument_parser(argc, argv));
  cppmu::TestFilter group_filter("group");
  group_filter.invert_matching();
  CHECK_EQUAL(group_filter, *args->get_group_filters());
  cppmu::TestFilter name_filter("name");
  name_filter.invert_matching();
  CHECK_EQUAL(name_filter, *args->get_name_filters());
}

TEST(CommandLineArguments,
    setCompleteExcludeStrictGroupDotNameFilterInvalidArgument)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-xst", "groupname" };
  CHECK_FALSE(new_argument_parser(argc, argv));
}

TEST(CommandLineArguments, setCompleteExcludeStrictGroupDotNameFilter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-xst", "group.name" };
  CHECK(new_argument_parser(argc, argv));
  cppmu::TestFilter group_filter("group");
  group_filter.strict_matching();
  group_filter.invert_matching();
  CHECK_EQUAL(group_filter, *args->get_group_filters());
  cppmu::TestFilter name_filter("name");
  name_filter.strict_matching();
  name_filter.invert_matching();
  CHECK_EQUAL(name_filter, *args->get_name_filters());
}

TEST(CommandLineArguments, setGroupFilterSameParameter)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-ggroup" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_EQUAL(cppmu::TestFilter("group"), *args->get_group_filters());
}

TEST(CommandLineArguments, setStrictGroupFilter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-sg", "group" };
  CHECK(new_argument_parser(argc, argv));
  cppmu::TestFilter group_filter("group");
  group_filter.strict_matching();
  CHECK_EQUAL(group_filter, *args->get_group_filters());
}

TEST(CommandLineArguments, setStrictGroupFilterSameParameter)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-sggroup" };
  CHECK(new_argument_parser(argc, argv));
  cppmu::TestFilter group_filter("group");
  group_filter.strict_matching();
  CHECK_EQUAL(group_filter, *args->get_group_filters());
}

TEST(CommandLineArguments, setExcludeGroupFilter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-xg", "group" };
  CHECK(new_argument_parser(argc, argv));
  cppmu::TestFilter group_filter("group");
  group_filter.invert_matching();
  CHECK_EQUAL(group_filter, *args->get_group_filters());
}

TEST(CommandLineArguments, setExcludeGroupFilterSameParameter)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-xggroup" };
  CHECK(new_argument_parser(argc, argv));
  cppmu::TestFilter group_filter("group");
  group_filter.invert_matching();
  CHECK_EQUAL(group_filter, *args->get_group_filters());
}

TEST(CommandLineArguments, setExcludeStrictGroupFilter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-xsg", "group" };
  CHECK(new_argument_parser(argc, argv));
  cppmu::TestFilter group_filter("group");
  group_filter.invert_matching();
  group_filter.strict_matching();
  CHECK_EQUAL(group_filter, *args->get_group_filters());
}

TEST(CommandLineArguments, setExcludeStrictGroupFilterSameParameter)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-xsggroup" };
  CHECK(new_argument_parser(argc, argv));
  cppmu::TestFilter group_filter("group");
  group_filter.invert_matching();
  group_filter.strict_matching();
  CHECK_EQUAL(group_filter, *args->get_group_filters());
}

TEST(CommandLineArguments, setNameFilter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-n", "name" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_EQUAL(cppmu::TestFilter("name"), *args->get_name_filters());
}

TEST(CommandLineArguments, setNameFilterSameParameter)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-nname" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_EQUAL(cppmu::TestFilter("name"), *args->get_name_filters());
}

TEST(CommandLineArguments, setStrictNameFilter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-sn", "name" };
  CHECK(new_argument_parser(argc, argv));
  cppmu::TestFilter name_filter("name");
  name_filter.strict_matching();
  CHECK_EQUAL(name_filter, *args->get_name_filters());
}

TEST(CommandLineArguments, setStrictNameFilterSameParameter)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-snname" };
  CHECK(new_argument_parser(argc, argv));
  cppmu::TestFilter name_filter("name");
  name_filter.strict_matching();
  CHECK_EQUAL(name_filter, *args->get_name_filters());
}

TEST(CommandLineArguments, setExcludeNameFilter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-xn", "name" };
  CHECK(new_argument_parser(argc, argv));
  cppmu::TestFilter name_filter("name");
  name_filter.invert_matching();
  CHECK_EQUAL(name_filter, *args->get_name_filters());
}

TEST(CommandLineArguments, setExcludeNameFilterSameParameter)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-xnname" };
  CHECK(new_argument_parser(argc, argv));
  cppmu::TestFilter name_filter("name");
  name_filter.invert_matching();
  CHECK_EQUAL(name_filter, *args->get_name_filters());
}

TEST(CommandLineArguments, setExcludeStrictNameFilter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-xsn", "name" };
  CHECK(new_argument_parser(argc, argv));
  cppmu::TestFilter name_filter("name");
  name_filter.invert_matching();
  name_filter.strict_matching();
  CHECK_EQUAL(name_filter, *args->get_name_filters());
}

TEST(CommandLineArguments, setExcludeStrictNameFilterSameParameter)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-xsnname" };
  CHECK(new_argument_parser(argc, argv));
  cppmu::TestFilter name_filter("name");
  name_filter.invert_matching();
  name_filter.strict_matching();
  CHECK_EQUAL(name_filter, *args->get_name_filters());
}

TEST(CommandLineArguments, setTestToRunUsingVerboseOutput)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "TEST(testgroup, testname) - stuff" };
  CHECK(new_argument_parser(argc, argv));

  cppmu::TestFilter name_filter("testname");
  cppmu::TestFilter group_filter("testgroup");
  name_filter.strict_matching();
  group_filter.strict_matching();
  CHECK_EQUAL(name_filter, *args->get_name_filters());
  CHECK_EQUAL(group_filter, *args->get_group_filters());
}

TEST(CommandLineArguments, setTestToRunUsingVerboseOutputOfIgnoreTest)
{
  int argc = 2;
  const char* argv[] = { "tests.exe",
    "IGNORE_TEST(testgroup, testname) - stuff" };
  CHECK(new_argument_parser(argc, argv));

  cppmu::TestFilter name_filter("testname");
  cppmu::TestFilter group_filter("testgroup");
  name_filter.strict_matching();
  group_filter.strict_matching();
  CHECK_EQUAL(name_filter, *args->get_name_filters());
  CHECK_EQUAL(group_filter, *args->get_group_filters());
}

TEST(CommandLineArguments, setNormalOutput)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-onormal" };
  CHECK(new_argument_parser(argc, argv));
  CHECK(args->is_eclipse_output());
}

TEST(CommandLineArguments, setEclipseOutput)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-oeclipse" };
  CHECK(new_argument_parser(argc, argv));
  CHECK(args->is_eclipse_output());
}

TEST(CommandLineArguments, setNormalOutputDifferentParameter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-o", "normal" };
  CHECK(new_argument_parser(argc, argv));
  CHECK(args->is_eclipse_output());
}

TEST(CommandLineArguments, setJUnitOutputDifferentParameter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-o", "junit" };
  CHECK(new_argument_parser(argc, argv));
  CHECK(args->is_j_unit_output());
}

TEST(CommandLineArguments, setTeamCityOutputDifferentParameter)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-o", "teamcity" };
  CHECK(new_argument_parser(argc, argv));
  CHECK(args->is_team_city_output());
}

TEST(CommandLineArguments, setOutputToGarbage)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-o", "garbage" };
  CHECK(!new_argument_parser(argc, argv));
}

TEST(CommandLineArguments, setPrintGroups)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-lg" };
  CHECK(new_argument_parser(argc, argv));
  CHECK(args->is_listing_test_group_names());
}

TEST(CommandLineArguments, setPrintGroupsAndNames)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-ln" };
  CHECK(new_argument_parser(argc, argv));
  CHECK(args->is_listing_test_group_and_case_names());
}

TEST(CommandLineArguments, weirdParamatersReturnsFalse)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-SomethingWeird" };
  CHECK(!new_argument_parser(argc, argv));
}

TEST(CommandLineArguments, printUsage)
{
  STRCMP_EQUAL(
      "use -h for more extensive help\n"
      "usage [-h] [-v] [-vv] [-c] [-lg] [-ln] [-ll] [-llo] [-ri] [-r[<#>]] "
      "[-f] [-e] [-ci]\n"
      "      [-g|sg|xg|xsg <groupName>]... [-n|sn|xn|xsn <testName>]... "
      "[-t|st|xt|xst <groupName>.<testName>]...\n"
      "      [-b] [-s [<seed>]] [\"[IGNORE_]TEST(<groupName>, "
      "<testName>)\"]...\n"
      "      [-o{normal|eclipse|junit|teamcity}] [-k <packageName>]\n",
      args->usage());
}

TEST(CommandLineArguments, helpPrintsTheHelp)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-h" };
  CHECK(!new_argument_parser(argc, argv));
  CHECK(args->need_help());
}

TEST(CommandLineArguments, pluginKnowsOption)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-pPluginOption" };
  cppmu::TestRegistry::get_current_registry()->install_plugin(plugin);
  CHECK(new_argument_parser(argc, argv));
  cppmu::TestRegistry::get_current_registry()->remove_plugin_by_name("options");
}

TEST(CommandLineArguments, checkDefaultArguments)
{
  int argc = 1;
  const char* argv[] = { "tests.exe" };
  CHECK(new_argument_parser(argc, argv));
  CHECK(!args->is_verbose());
  LONGS_EQUAL(1, args->get_repeat_count());
  CHECK(nullptr == args->get_group_filters());
  CHECK(nullptr == args->get_name_filters());
  CHECK(args->is_eclipse_output());
  CHECK(cppmu::String("") == args->get_package_name());
  CHECK(!args->is_crashing_on_fail());
  CHECK(args->is_rethrowing_exceptions());
}

TEST(CommandLineArguments, checkContinuousIntegrationMode)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-ci" };
  CHECK(new_argument_parser(argc, argv));
  CHECK(!args->is_verbose());
  LONGS_EQUAL(1, args->get_repeat_count());
  CHECK(nullptr == args->get_group_filters());
  CHECK(nullptr == args->get_name_filters());
  CHECK(args->is_eclipse_output());
  CHECK(cppmu::String("") == args->get_package_name());
  CHECK(!args->is_crashing_on_fail());
  CHECK_FALSE(args->is_rethrowing_exceptions());
}

TEST(CommandLineArguments, setPackageName)
{
  int argc = 3;
  const char* argv[] = { "tests.exe", "-k", "package" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_EQUAL(cppmu::String("package"), args->get_package_name());
}

TEST(CommandLineArguments, lotsOfGroupsAndTests)
{
  int argc = 10;
  const char* argv[] = { "tests.exe",
    "-sggroup1",
    "-xntest1",
    "-sggroup2",
    "-sntest2",
    "-sntest3",
    "-sggroup3",
    "-sntest4",
    "-sggroup4",
    "-sntest5" };
  CHECK(new_argument_parser(argc, argv));
  cppmu::TestFilter name_filter("test1");
  name_filter.invert_matching();
  cppmu::TestFilter group_filter("group1");
  group_filter.strict_matching();
  CHECK_EQUAL(name_filter,
      *args->get_name_filters()
          ->get_next()
          ->get_next()
          ->get_next()
          ->get_next());
  CHECK_EQUAL(group_filter,
      *args->get_group_filters()->get_next()->get_next()->get_next());
}

TEST(CommandLineArguments, lastParameterFieldMissing)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-k" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_EQUAL(cppmu::String(""), args->get_package_name());
}

TEST(CommandLineArguments, setOptRun)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-ri" };
  CHECK(new_argument_parser(argc, argv));
  CHECK(args->is_run_ignored());
}

TEST(CommandLineArguments, setOptCrashOnFail)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-f" };
  CHECK(new_argument_parser(argc, argv));
  CHECK(args->is_crashing_on_fail());
}

TEST(CommandLineArguments, setOptRethrowExceptions)
{
  int argc = 2;
  const char* argv[] = { "tests.exe", "-e" };
  CHECK(new_argument_parser(argc, argv));
  CHECK_FALSE(args->is_rethrowing_exceptions());
}
