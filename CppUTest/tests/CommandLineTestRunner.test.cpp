#include "CppUTest/CommandLineTestRunner.hpp"

#include "CppUTest/JUnitTestOutput.hpp"
#include "CppUTest/StringCollection.hpp"
#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestPlugin.hpp"
#include "CppUTest/TestRegistry.hpp"
#include "CppUTest/TestTestingFixture.hpp"

namespace {
class DummyPluginWhichCountsThePlugins : public cpputest::TestPlugin
{
public:
  bool return_value;
  int amount_of_plugins;

  DummyPluginWhichCountsThePlugins(const cpputest::String& name,
      cpputest::TestRegistry* registry)
    : TestPlugin(name)
    , return_value(true)
    , amount_of_plugins(0)
    , registry_(registry)
  {
  }

  virtual bool parse_arguments(int, const char* const*, int) override
  {
    /* Remove ourselves from the count */
    amount_of_plugins = registry_->count_plugins() - 1;
    return return_value;
  }

private:
  cpputest::TestRegistry* registry_;
};

class CommandLineTestRunnerWithStringBufferOutput
  : public cpputest::CommandLineTestRunner
{
public:
  cpputest::StringBufferTestOutput* fake_j_unit_output_which_is_really_a_buffer;
  cpputest::StringBufferTestOutput*
      fake_console_output_which_is_really_a_buffer;
  cpputest::StringBufferTestOutput* fake_tc_output_which_is_really_a_buffer;

  CommandLineTestRunnerWithStringBufferOutput(int argc,
      const char* const* argv,
      cpputest::TestRegistry* registry)
    : CommandLineTestRunner(argc, argv, registry)
    , fake_j_unit_output_which_is_really_a_buffer(nullptr)
    , fake_console_output_which_is_really_a_buffer(nullptr)
    , fake_tc_output_which_is_really_a_buffer(nullptr)
  {
  }

  cpputest::TestOutput* create_console_output() override
  {
    fake_console_output_which_is_really_a_buffer =
        new cpputest::StringBufferTestOutput;
    return fake_console_output_which_is_really_a_buffer;
  }

  cpputest::TestOutput* create_j_unit_output(const cpputest::String&) override
  {
    fake_j_unit_output_which_is_really_a_buffer =
        new cpputest::StringBufferTestOutput;
    return fake_j_unit_output_which_is_really_a_buffer;
  }

  cpputest::TestOutput* create_team_city_output() override
  {
    fake_tc_output_which_is_really_a_buffer =
        new cpputest::StringBufferTestOutput;
    return fake_tc_output_which_is_really_a_buffer;
  }
};

class RunIgnoredTest : public cpputest::Test
{
public:
  static bool checker_;
  void test_body() override { checker_ = true; }
};

bool RunIgnoredTest::checker_ = false;

class RunIgnoredUtestShell : public cpputest::IgnoredTestShell
{
public:
  RunIgnoredUtestShell(const char* group_name,
      const char* test_name,
      const char* file_name,
      size_t line_number)
    : IgnoredTestShell(group_name, test_name, file_name, line_number)
  {
  }
  virtual cpputest::Test* create_test() override { return new RunIgnoredTest; }
};
} // namespace

TEST_GROUP(CommandLineTestRunner)
{
  cpputest::TestRegistry registry;
  cpputest::TestShell* test1;
  cpputest::TestShell* test2;
  DummyPluginWhichCountsThePlugins* plugin_counting_plugin;

  void setup() override
  {
    test1 = new cpputest::TestShell("group1", "test1", "file1", 1);
    test2 = new cpputest::TestShell("group2", "test2", "file2", 2);
    registry.add_test(test1);
    plugin_counting_plugin =
        new DummyPluginWhichCountsThePlugins("PluginCountingPlugin", &registry);
  }
  void teardown() override
  {
    delete plugin_counting_plugin;
    delete test2;
    delete test1;
  }

  cpputest::String run_and_get_output(const int argc, const char* argv[])
  {
    CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
        argc, argv, &registry);
    command_line_test_runner.run_all_tests_main();
    return command_line_test_runner
        .fake_console_output_which_is_really_a_buffer->get_output();
  }
};

TEST(CommandLineTestRunner, OnePluginGetsInstalledDuringTheRunningTheTests)
{
  const char* argv[] = { "tests.exe", "-psomething" };

  registry.install_plugin(plugin_counting_plugin);

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry);
  command_line_test_runner.run_all_tests_main();
  registry.remove_plugin_by_name("PluginCountingPlugin");

  LONGS_EQUAL(0, registry.count_plugins());
  LONGS_EQUAL(1, plugin_counting_plugin->amount_of_plugins);
}

TEST(CommandLineTestRunner,
    NoPluginsAreInstalledAtTheEndOfARunWhenTheArgumentsAreInvalid)
{
  const char* argv[] = { "tests.exe", "-fdskjnfkds" };

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry);
  command_line_test_runner.run_all_tests_main();

  LONGS_EQUAL(0, registry.count_plugins());
}

TEST(CommandLineTestRunner, ReturnsOneWhenTheArgumentsAreInvalid)
{
  const char* argv[] = { "tests.exe", "-some-invalid=parameter" };

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry);
  int returned = command_line_test_runner.run_all_tests_main();

  LONGS_EQUAL(1, returned);
}

TEST(CommandLineTestRunner, ReturnsOnePrintsHelpOnHelp)
{
  const char* argv[] = { "tests.exe", "-h" };

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry);
  int returned = command_line_test_runner.run_all_tests_main();

  LONGS_EQUAL(1, returned);
  STRCMP_CONTAINS("Thanks for using CppUTest.",
      command_line_test_runner.fake_console_output_which_is_really_a_buffer
          ->get_output()
          .c_str());
}

TEST(CommandLineTestRunner, ReturnsZeroWhenNoErrors)
{
  const char* argv[] = { "tests.exe" };

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      1, argv, &registry);
  int returned = command_line_test_runner.run_all_tests_main();

  LONGS_EQUAL(0, returned);
}

TEST(CommandLineTestRunner, ReturnsOneWhenNoTestsMatchProvidedFilter)
{
  const char* argv[] = { "tests.exe", "-g", "NoSuchGroup" };

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      3, argv, &registry);
  int returned = command_line_test_runner.run_all_tests_main();

  LONGS_EQUAL(1, returned);
}

TEST(CommandLineTestRunner, TeamcityOutputEnabled)
{
  const char* argv[] = { "tests.exe", "-oteamcity" };
  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry);
  command_line_test_runner.run_all_tests_main();
  CHECK(command_line_test_runner.fake_tc_output_which_is_really_a_buffer !=
        nullptr);
}

TEST(CommandLineTestRunner, JunitOutputEnabled)
{
  const char* argv[] = { "tests.exe", "-ojunit" };

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry);
  command_line_test_runner.run_all_tests_main();
  CHECK(command_line_test_runner.fake_j_unit_output_which_is_really_a_buffer !=
        nullptr);
}

TEST(CommandLineTestRunner, JunitOutputAndVerboseEnabled)
{
  const char* argv[] = { "tests.exe", "-ojunit", "-v" };

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      3, argv, &registry);
  command_line_test_runner.run_all_tests_main();
  STRCMP_CONTAINS("TEST(group1, test1)",
      command_line_test_runner.fake_j_unit_output_which_is_really_a_buffer
          ->get_output()
          .c_str());
  STRCMP_CONTAINS("TEST(group1, test1)",
      command_line_test_runner.fake_console_output_which_is_really_a_buffer
          ->get_output()
          .c_str());
}

TEST(CommandLineTestRunner, veryVerboseSetOnOutput)
{
  const char* argv[] = { "tests.exe", "-vv" };

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry);
  command_line_test_runner.run_all_tests_main();
  STRCMP_CONTAINS("TEST(group1, test1)",
      command_line_test_runner.fake_console_output_which_is_really_a_buffer
          ->get_output()
          .c_str());
  STRCMP_CONTAINS("destroyTest",
      command_line_test_runner.fake_console_output_which_is_really_a_buffer
          ->get_output()
          .c_str());
}

TEST(CommandLineTestRunner, defaultTestsAreRunInOrderTheyAreInRepository)
{
  const char* argv[] = { "tests.exe", "-v" };

  registry.add_test(test2);
  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry);
  command_line_test_runner.run_all_tests_main();

  cpputest::StringCollection string_collection(
      command_line_test_runner.fake_console_output_which_is_really_a_buffer
          ->get_output(),
      '\n');
  STRCMP_CONTAINS("test2", string_collection[0].c_str());
  STRCMP_CONTAINS("test1", string_collection[1].c_str());
}

TEST(CommandLineTestRunner, testsCanBeRunInReverseOrder)
{
  const char* argv[] = { "tests.exe", "-v", "-b" };

  registry.add_test(test2);
  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      3, argv, &registry);
  command_line_test_runner.run_all_tests_main();

  cpputest::StringCollection string_collection(
      command_line_test_runner.fake_console_output_which_is_really_a_buffer
          ->get_output(),
      '\n');
  STRCMP_CONTAINS("test1", string_collection[0].c_str());
  STRCMP_CONTAINS("test2", string_collection[1].c_str());
}

TEST(CommandLineTestRunner, listTestGroupNamesShouldWorkProperly)
{
  const char* argv[] = { "tests.exe", "-lg" };

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry);
  command_line_test_runner.run_all_tests_main();

  STRCMP_CONTAINS("group",
      command_line_test_runner.fake_console_output_which_is_really_a_buffer
          ->get_output()
          .c_str());
}

TEST(CommandLineTestRunner, listTestGroupAndCaseNamesShouldWorkProperly)
{
  const char* argv[] = { "tests.exe", "-ln" };

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry);
  command_line_test_runner.run_all_tests_main();

  STRCMP_CONTAINS("group1.test1",
      command_line_test_runner.fake_console_output_which_is_really_a_buffer
          ->get_output()
          .c_str());
}

TEST(CommandLineTestRunner, listTestLocationsShouldWorkProperly)
{
  const char* argv[] = { "tests.exe", "-ll" };

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry);
  command_line_test_runner.run_all_tests_main();

  STRCMP_CONTAINS("group1.test1",
      command_line_test_runner.fake_console_output_which_is_really_a_buffer
          ->get_output()
          .c_str());
}

TEST(CommandLineTestRunner, randomShuffleSeedIsPrintedAndRandFuncIsExercised)
{
  // more than 1 item in test list ensures that shuffle algorithm calls rand_()
  auto* another_test = new cpputest::TestShell("group", "test2", "file", 1);
  registry.add_test(another_test);

  const char* argv[] = { "tests.exe", "-s" };
  cpputest::String text = run_and_get_output(2, argv);
  STRCMP_CONTAINS("shuffling enabled with seed:", text.c_str());

  delete another_test;
}

TEST(CommandLineTestRunner, specificShuffleSeedIsPrintedVerbose)
{
  const char* argv[] = { "tests.exe", "-s2", "-v" };
  cpputest::String text = run_and_get_output(3, argv);
  STRCMP_CONTAINS("shuffling enabled with seed: 2", text.c_str());
}

namespace {

struct FakeOutput
{
  FakeOutput()
    : save_f_open_(cpputest::f_open)
    , save_f_puts_(cpputest::f_puts)
    , save_f_close_(cpputest::f_close)
  {
    install_fakes();
    current_fake_ = this;
  }

  ~FakeOutput()
  {
    current_fake_ = nullptr;
    restore_originals();
  }

  void install_fakes()
  {
    cpputest::f_open = fopen_fake;
    cpputest::f_puts = fputs_fake;
    cpputest::f_close = fclose_fake;
  }

  void restore_originals()
  {
    cpputest::f_open = save_f_open_;
    cpputest::f_puts = save_f_puts_;
    cpputest::f_close = save_f_close_;
  }

  static cpputest::File fopen_fake(const char*, const char*)
  {
    return static_cast<cpputest::File>(nullptr);
  }

  static void fputs_fake(const char* str, cpputest::File f)
  {
    if (f == cpputest::std_out) {
      current_fake_->console += str;
    } else {
      current_fake_->file += str;
    }
  }

  static void fclose_fake(cpputest::File) {}

  cpputest::String file;
  cpputest::String console;

  static FakeOutput* current_fake_;

private:
  cpputest::FOpenFunc save_f_open_;
  cpputest::FPutsFunc save_f_puts_;
  cpputest::FCloseFunc save_f_close_;
};

FakeOutput* FakeOutput::current_fake_ = nullptr;
}

TEST(CommandLineTestRunner, realJunitOutputShouldBeCreatedAndWorkProperly)
{
  const char* argv[] = {
    "tests.exe",
    "-ojunit",
    "-v",
    "-kpackage",
  };

  FakeOutput fake_output; /* UT_PTR_SET() is not reentrant */

  cpputest::CommandLineTestRunner command_line_test_runner(4, argv, &registry);
  command_line_test_runner.run_all_tests_main();

  fake_output.restore_originals();

  STRCMP_CONTAINS("<testcase classname=\"package.group1\" name=\"test1\"",
      fake_output.file.c_str());
  STRCMP_CONTAINS("TEST(group1, test1)", fake_output.console.c_str());
}

TEST(CommandLineTestRunner, realTeamCityOutputShouldBeCreatedAndWorkProperly)
{
  const char* argv[] = {
    "tests.exe",
    "-oteamcity",
    "-v",
    "-kpackage",
  };

  FakeOutput fake_output; /* UT_PTR_SET() is not reentrant */

  cpputest::CommandLineTestRunner command_line_test_runner(4, argv, &registry);
  command_line_test_runner.run_all_tests_main();

  fake_output.restore_originals();

  STRCMP_CONTAINS(
      "##teamcity[testSuiteStarted name='group1'", fake_output.console.c_str());
  STRCMP_CONTAINS(
      "##teamcity[testStarted name='test1'", fake_output.console.c_str());
  STRCMP_CONTAINS(
      "##teamcity[testFinished name='test1'", fake_output.console.c_str());
  STRCMP_CONTAINS("##teamcity[testSuiteFinished name='group1'",
      fake_output.console.c_str());
}

TEST(CommandLineTestRunner, IgnoreTestWillBeIgnoredIfNoOptionSpecified)
{
  cpputest::TestRegistry ignored_registry;
  RunIgnoredUtestShell run_ignored_test("group", "test", "file", 1);
  ignored_registry.add_test(&run_ignored_test);
  DummyPluginWhichCountsThePlugins ignored_plugin(
      "PluginCountingPlugin", &ignored_registry);

  const char* argv[] = { "tests.exe" };
  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      1, argv, &ignored_registry);
  command_line_test_runner.run_all_tests_main();

  CHECK_FALSE(RunIgnoredTest::checker_);
  RunIgnoredTest::checker_ = false;
}

TEST(CommandLineTestRunner, IgnoreTestWillGetRunIfOptionSpecified)
{
  cpputest::TestRegistry ignored_registry;
  RunIgnoredUtestShell run_ignored_test("group", "test", "file", 1);
  ignored_registry.add_test(&run_ignored_test);
  DummyPluginWhichCountsThePlugins ignored_plugin(
      "PluginCountingPlugin", &ignored_registry);

  const char* argv[] = { "tests.exe", "-ri" };
  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &ignored_registry);
  command_line_test_runner.run_all_tests_main();

  CHECK_TRUE(RunIgnoredTest::checker_);
  RunIgnoredTest::checker_ = false;
}
