#include "CppMu/CommandLineTestRunner.hpp"

#include "CppMu/CppMu.hpp"
#include "CppMu/JUnitTestOutput.hpp"
#include "CppMu/StringCollection.hpp"
#include "CppMu/TestPlugin.hpp"
#include "CppMu/TestRegistry.hpp"
#include "CppMu/TestTestingFixture.hpp"

namespace {
class DummyPluginWhichCountsThePlugins : public cppmu::TestPlugin
{
public:
  bool return_value;
  int amount_of_plugins;

  DummyPluginWhichCountsThePlugins(const cppmu::String& name,
      cppmu::TestRegistry* registry)
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
  cppmu::TestRegistry* registry_;
};

class CommandLineTestRunnerWithStringBufferOutput
  : public cppmu::CommandLineTestRunner
{
public:
  cppmu::StringBufferTestOutput* fake_j_unit_output_which_is_really_a_buffer;
  cppmu::StringBufferTestOutput* fake_console_output_which_is_really_a_buffer;
  cppmu::StringBufferTestOutput* fake_tc_output_which_is_really_a_buffer;

  CommandLineTestRunnerWithStringBufferOutput(int argc,
      const char* const* argv,
      cppmu::TestRegistry* registry)
    : CommandLineTestRunner(argc, argv, registry)
    , fake_j_unit_output_which_is_really_a_buffer(nullptr)
    , fake_console_output_which_is_really_a_buffer(nullptr)
    , fake_tc_output_which_is_really_a_buffer(nullptr)
  {
  }

  cppmu::TestOutput* create_console_output() override
  {
    fake_console_output_which_is_really_a_buffer =
        new cppmu::StringBufferTestOutput;
    return fake_console_output_which_is_really_a_buffer;
  }

  cppmu::TestOutput* create_j_unit_output(const cppmu::String&) override
  {
    fake_j_unit_output_which_is_really_a_buffer =
        new cppmu::StringBufferTestOutput;
    return fake_j_unit_output_which_is_really_a_buffer;
  }

  cppmu::TestOutput* create_team_city_output() override
  {
    fake_tc_output_which_is_really_a_buffer = new cppmu::StringBufferTestOutput;
    return fake_tc_output_which_is_really_a_buffer;
  }
};

class RunIgnoredTest : public cppmu::Test
{
public:
  static bool checker_;
  void test_body() override { checker_ = true; }
};

bool RunIgnoredTest::checker_ = false;

class RunIgnoredShell : public cppmu::IgnoredTestShell
{
public:
  RunIgnoredShell(const char* group_name,
      const char* test_name,
      const char* file_name,
      size_t line_number)
    : IgnoredTestShell(group_name, test_name, file_name, line_number)
  {
  }
  virtual cppmu::Test* create_test() override { return new RunIgnoredTest; }
};
} // namespace

TEST_GROUP(CommandLineTestRunner)
{
  cppmu::TestRegistry registry;
  cppmu::TestShell* test1;
  cppmu::TestShell* test2;
  DummyPluginWhichCountsThePlugins* plugin_counting_plugin;

  void setup() override
  {
    test1 = new cppmu::TestShell("group1", "test1", "file1", 1);
    test2 = new cppmu::TestShell("group2", "test2", "file2", 2);
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

  cppmu::String run_and_get_output(const int argc, const char* argv[])
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
  STRCMP_CONTAINS("Thanks for using CppMu.",
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

  cppmu::StringCollection string_collection(
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

  cppmu::StringCollection string_collection(
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
  auto* another_test = new cppmu::TestShell("group", "test2", "file", 1);
  registry.add_test(another_test);

  const char* argv[] = { "tests.exe", "-s" };
  cppmu::String text = run_and_get_output(2, argv);
  STRCMP_CONTAINS("shuffling enabled with seed:", text.c_str());

  delete another_test;
}

TEST(CommandLineTestRunner, specificShuffleSeedIsPrintedVerbose)
{
  const char* argv[] = { "tests.exe", "-s2", "-v" };
  cppmu::String text = run_and_get_output(3, argv);
  STRCMP_CONTAINS("shuffling enabled with seed: 2", text.c_str());
}

namespace {

struct FakeOutput
{
  FakeOutput()
    : save_f_open_(cppmu::f_open)
    , save_f_puts_(cppmu::f_puts)
    , save_f_close_(cppmu::f_close)
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
    cppmu::f_open = fopen_fake;
    cppmu::f_puts = fputs_fake;
    cppmu::f_close = fclose_fake;
  }

  void restore_originals()
  {
    cppmu::f_open = save_f_open_;
    cppmu::f_puts = save_f_puts_;
    cppmu::f_close = save_f_close_;
  }

  static cppmu::File fopen_fake(const char*, const char*)
  {
    return static_cast<cppmu::File>(nullptr);
  }

  static void fputs_fake(const char* str, cppmu::File f)
  {
    if (f == cppmu::std_out) {
      current_fake_->console += str;
    } else {
      current_fake_->file += str;
    }
  }

  static void fclose_fake(cppmu::File) {}

  cppmu::String file;
  cppmu::String console;

  static FakeOutput* current_fake_;

private:
  cppmu::FOpenFunc save_f_open_;
  cppmu::FPutsFunc save_f_puts_;
  cppmu::FCloseFunc save_f_close_;
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

  FakeOutput fake_output; /* CPPMU_PTR_SET() is not reentrant */

  cppmu::CommandLineTestRunner command_line_test_runner(4, argv, &registry);
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

  FakeOutput fake_output; /* CPPMU_PTR_SET() is not reentrant */

  cppmu::CommandLineTestRunner command_line_test_runner(4, argv, &registry);
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
  cppmu::TestRegistry ignored_registry;
  RunIgnoredShell run_ignored_test("group", "test", "file", 1);
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
  cppmu::TestRegistry ignored_registry;
  RunIgnoredShell run_ignored_test("group", "test", "file", 1);
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
