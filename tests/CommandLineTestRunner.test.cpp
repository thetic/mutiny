#include "CppMu/CommandLineTestRunner.hpp"

#include "CppMu/CppMu.hpp"
#include "CppMu/StringCollection.hpp"
#include "CppMu/TestPlugin.hpp"
#include "CppMu/TestRegistry.hpp"
#include "CppMu/TestTestingFixture.hpp"

namespace {
class DummyPluginWhichCountsThePlugins : public cppmu::TestPlugin
{
public:
  bool return_value{ true };
  int amount_of_plugins{ 0 };

  DummyPluginWhichCountsThePlugins(
      const cppmu::String& name,
      cppmu::TestRegistry* registry
  )
    : TestPlugin(name)
    , registry_(registry)
  {
  }

  bool parse_arguments(int, const char* const*, int) override
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
  cppmu::StringBufferTestOutput* fake_console_output_which_is_really_a_buffer{
    nullptr
  };

  CommandLineTestRunnerWithStringBufferOutput(
      int argc,
      const char* const* argv,
      cppmu::TestRegistry* registry
  )
    : CommandLineTestRunner(argc, argv, registry)

  {
  }

  cppmu::TestOutput* create_console_output() override
  {
    fake_console_output_which_is_really_a_buffer =
        new cppmu::StringBufferTestOutput;
    return fake_console_output_which_is_really_a_buffer;
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
  RunIgnoredShell(
      const char* group_name,
      const char* test_name,
      const char* file_name,
      size_t line_number
  )
    : IgnoredTestShell(group_name, test_name, file_name, line_number)
  {
  }
  cppmu::Test* create_test() override { return new RunIgnoredTest; }
};

class TestExecutionVerifier : public cppmu::Test
{
public:
  static bool was_run_;
  void test_body() override { was_run_ = true; }
};
bool TestExecutionVerifier::was_run_ = false;

class TestExecutionVerifierShell : public cppmu::TestShell
{
public:
  TestExecutionVerifierShell()
    : TestShell("VerifierGroup", "VerifierTest", "file", 1)
  {
  }
  cppmu::Test* create_test() override { return new TestExecutionVerifier; }
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
        argc, argv, &registry
    );
    command_line_test_runner.run_all_tests_main();
    return command_line_test_runner
        .fake_console_output_which_is_really_a_buffer->get_output();
  }
};

TEST(CommandLineTestRunner, TwoBuiltinPluginsAreInstalledDuringTheRunningTheTests)
{
  const char* argv[] = { "tests.exe", "-psomething" };

  registry.install_plugin(plugin_counting_plugin);

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry
  );
  command_line_test_runner.run_all_tests_main();
  registry.remove_plugin_by_name("PluginCountingPlugin");

  LONGS_EQUAL(0, registry.count_plugins());
  LONGS_EQUAL(2, plugin_counting_plugin->amount_of_plugins);
}

TEST(CommandLineTestRunner, NoPluginsAreInstalledAtTheEndOfARunWhenTheArgumentsAreInvalid)
{
  const char* argv[] = { "tests.exe", "-fdskjnfkds" };

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry
  );
  command_line_test_runner.run_all_tests_main();

  LONGS_EQUAL(0, registry.count_plugins());
}

TEST(CommandLineTestRunner, ReturnsOneWhenTheArgumentsAreInvalid)
{
  const char* argv[] = { "tests.exe", "-some-invalid=parameter" };

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry
  );
  int returned = command_line_test_runner.run_all_tests_main();

  LONGS_EQUAL(1, returned);
}

TEST(CommandLineTestRunner, ReturnsZeroPrintsHelpOnHelp)
{
  const char* argv[] = { "tests.exe", "-h" };
  TestExecutionVerifierShell verifier_test;
  registry.add_test(&verifier_test);
  TestExecutionVerifier::was_run_ = false;

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry
  );
  int returned = command_line_test_runner.run_all_tests_main();

  LONGS_EQUAL(0, returned);
  STRCMP_CONTAINS(
      "Options that do not run tests but query:",
      command_line_test_runner.fake_console_output_which_is_really_a_buffer
          ->get_output()
          .c_str()
  );
  CHECK_FALSE(TestExecutionVerifier::was_run_);
}

TEST(CommandLineTestRunner, ReturnsOnePrintsHelpOnHelpWithInvalidArg)
{
  const char* argv[] = { "tests.exe", "-h", "-invalid" };
  TestExecutionVerifierShell verifier_test;
  registry.add_test(&verifier_test);
  TestExecutionVerifier::was_run_ = false;

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      3, argv, &registry
  );
  int returned = command_line_test_runner.run_all_tests_main();

  LONGS_EQUAL(1, returned);
  STRCMP_CONTAINS(
      "Options that do not run tests but query:",
      command_line_test_runner.fake_console_output_which_is_really_a_buffer
          ->get_output()
          .c_str()
  );
  CHECK_FALSE(TestExecutionVerifier::was_run_);
}

TEST(CommandLineTestRunner, ReturnsZeroWhenNoErrors)
{
  const char* argv[] = { "tests.exe" };

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      1, argv, &registry
  );
  int returned = command_line_test_runner.run_all_tests_main();

  LONGS_EQUAL(0, returned);
}

TEST(CommandLineTestRunner, ReturnsOneWhenNoTestsMatchProvidedFilter)
{
  const char* argv[] = { "tests.exe", "-g", "NoSuchGroup" };

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      3, argv, &registry
  );
  int returned = command_line_test_runner.run_all_tests_main();

  LONGS_EQUAL(1, returned);
}

TEST(CommandLineTestRunner, veryVerboseSetOnOutput)
{
  const char* argv[] = { "tests.exe", "-vv" };

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry
  );
  command_line_test_runner.run_all_tests_main();
  STRCMP_CONTAINS(
      "TEST(group1, test1)",
      command_line_test_runner.fake_console_output_which_is_really_a_buffer
          ->get_output()
          .c_str()
  );
  STRCMP_CONTAINS(
      "destroyTest",
      command_line_test_runner.fake_console_output_which_is_really_a_buffer
          ->get_output()
          .c_str()
  );
}

TEST(CommandLineTestRunner, defaultTestsAreRunInOrderTheyAreInRepository)
{
  const char* argv[] = { "tests.exe", "-v" };

  registry.add_test(test2);
  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry
  );
  command_line_test_runner.run_all_tests_main();

  cppmu::StringCollection string_collection(
      command_line_test_runner.fake_console_output_which_is_really_a_buffer
          ->get_output(),
      '\n'
  );
  STRCMP_CONTAINS("test2", string_collection[0].c_str());
  STRCMP_CONTAINS("test1", string_collection[1].c_str());
}

TEST(CommandLineTestRunner, testsCanBeRunInReverseOrder)
{
  const char* argv[] = { "tests.exe", "-v", "-b" };

  registry.add_test(test2);
  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      3, argv, &registry
  );
  command_line_test_runner.run_all_tests_main();

  cppmu::StringCollection string_collection(
      command_line_test_runner.fake_console_output_which_is_really_a_buffer
          ->get_output(),
      '\n'
  );
  STRCMP_CONTAINS("test1", string_collection[0].c_str());
  STRCMP_CONTAINS("test2", string_collection[1].c_str());
}

TEST(CommandLineTestRunner, listTestGroupNamesShouldWorkProperly)
{
  const char* argv[] = { "tests.exe", "-lg" };

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry
  );
  command_line_test_runner.run_all_tests_main();

  STRCMP_CONTAINS(
      "group",
      command_line_test_runner.fake_console_output_which_is_really_a_buffer
          ->get_output()
          .c_str()
  );
}

TEST(CommandLineTestRunner, listTestGroupAndCaseNamesShouldWorkProperly)
{
  const char* argv[] = { "tests.exe", "-ln" };

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry
  );
  command_line_test_runner.run_all_tests_main();

  STRCMP_CONTAINS(
      "group1.test1",
      command_line_test_runner.fake_console_output_which_is_really_a_buffer
          ->get_output()
          .c_str()
  );
}

TEST(CommandLineTestRunner, listTestLocationsShouldWorkProperly)
{
  const char* argv[] = { "tests.exe", "-ll" };

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry
  );
  command_line_test_runner.run_all_tests_main();

  STRCMP_CONTAINS(
      "group1.test1",
      command_line_test_runner.fake_console_output_which_is_really_a_buffer
          ->get_output()
          .c_str()
  );
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
    cppmu::TestOutput::fopen_ = fopen_fake;
    cppmu::TestOutput::fputs_ = fputs_fake;
    cppmu::TestOutput::fclose_ = fclose_fake;
  }

  void restore_originals()
  {
    cppmu::TestOutput::fopen_ = save_f_open_;
    cppmu::TestOutput::fputs_ = save_f_puts_;
    cppmu::TestOutput::fclose_ = save_f_close_;
  }

  static cppmu::TestOutput::File fopen_fake(const char*, const char*)
  {
    return static_cast<cppmu::TestOutput::File>(nullptr);
  }

  static void fputs_fake(const char* str, cppmu::TestOutput::File f)
  {
    if (f == cppmu::TestOutput::stdout_) {
      current_fake_->console += str;
    } else {
      current_fake_->file += str;
    }
  }

  static void fclose_fake(cppmu::TestOutput::File) {}

  cppmu::String file;
  cppmu::String console;

  static FakeOutput* current_fake_;

private:
  cppmu::TestOutput::FOpenFunc save_f_open_{ cppmu::TestOutput::fopen_ };
  cppmu::TestOutput::FPutsFunc save_f_puts_{ cppmu::TestOutput::fputs_ };
  cppmu::TestOutput::FCloseFunc save_f_close_{ cppmu::TestOutput::fclose_ };
};

FakeOutput* FakeOutput::current_fake_ = nullptr;
}

TEST(CommandLineTestRunner, realJunitOutputShouldBeCreatedAndWorkProperly)
{
  const char* argv[] = {
    "tests.exe",
    "-pjunit",
    "-v",
  };

  FakeOutput fake_output; /* CPPMU_PTR_SET() is not reentrant */

  cppmu::CommandLineTestRunner command_line_test_runner(3, argv, &registry);
  command_line_test_runner.run_all_tests_main();

  fake_output.restore_originals();

  STRCMP_CONTAINS(
      "<testcase classname=\"group1\" name=\"test1\"", fake_output.file.c_str()
  );
  STRCMP_CONTAINS("TEST(group1, test1)", fake_output.console.c_str());
}

TEST(CommandLineTestRunner, IgnoreTestWillBeIgnoredIfNoOptionSpecified)
{
  cppmu::TestRegistry ignored_registry;
  RunIgnoredShell run_ignored_test("group", "test", "file", 1);
  ignored_registry.add_test(&run_ignored_test);
  DummyPluginWhichCountsThePlugins ignored_plugin(
      "PluginCountingPlugin", &ignored_registry
  );

  const char* argv[] = { "tests.exe" };
  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      1, argv, &ignored_registry
  );
  command_line_test_runner.run_all_tests_main();

  CHECK_FALSE(RunIgnoredTest::checker_);
  RunIgnoredTest::checker_ = false;
}

TEST(CommandLineTestRunner, listOrderedTestLocations)
{
  const char* argv[] = { "tests.exe", "-llo" };
  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry
  );
  LONGS_EQUAL(0, command_line_test_runner.run_all_tests_main());
}

TEST(CommandLineTestRunner, IgnoreTestWillGetRunIfOptionSpecified)
{
  cppmu::TestRegistry ignored_registry;
  RunIgnoredShell run_ignored_test("group", "test", "file", 1);
  ignored_registry.add_test(&run_ignored_test);
  DummyPluginWhichCountsThePlugins ignored_plugin(
      "PluginCountingPlugin", &ignored_registry
  );

  const char* argv[] = { "tests.exe", "-ri" };
  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &ignored_registry
  );
  command_line_test_runner.run_all_tests_main();

  CHECK_TRUE(RunIgnoredTest::checker_);
  RunIgnoredTest::checker_ = false;
}
