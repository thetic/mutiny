#include "mutiny/test/CommandLineRunner.hpp"
#include "mutiny/test/Plugin.hpp"
#include "mutiny/test/Registry.hpp"
#include "mutiny/test/StringBufferOutput.hpp"

#include "mutiny/test.hpp"

namespace {
class DummyPluginWhichCountsThePlugins : public mu::tiny::test::Plugin
{
public:
  bool return_value{ true };
  int amount_of_plugins{ 0 };

  DummyPluginWhichCountsThePlugins(
      const mu::tiny::String& name,
      mu::tiny::test::Registry* registry
  )
    : Plugin(name)
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
  mu::tiny::test::Registry* registry_;
};

class CommandLineTestRunnerWithStringBufferOutput
  : public mu::tiny::test::CommandLineRunner
{
public:
  mu::tiny::test::StringBufferOutput*
      fake_console_output_which_is_really_a_buffer{ nullptr };

  CommandLineTestRunnerWithStringBufferOutput(
      int argc,
      const char* const* argv,
      mu::tiny::test::Registry* registry
  )
    : CommandLineRunner(argc, argv, registry)

  {
  }

  mu::tiny::test::Output* create_console_output() override
  {
    fake_console_output_which_is_really_a_buffer =
        new mu::tiny::test::StringBufferOutput;
    return fake_console_output_which_is_really_a_buffer;
  }
};

class RunIgnoredTest : public mu::tiny::test::Test
{
public:
  static bool checker_;
  void test_body() override { checker_ = true; }
};

bool RunIgnoredTest::checker_ = false;

class RunIgnoredShell : public mu::tiny::test::IgnoredShell
{
public:
  RunIgnoredShell(
      const char* group_name,
      const char* test_name,
      const char* file_name,
      size_t line_number
  )
    : IgnoredShell(group_name, test_name, file_name, line_number)
  {
  }
  mu::tiny::test::Test* create_test() override { return new RunIgnoredTest; }
};

class TestExecutionVerifier : public mu::tiny::test::Test
{
public:
  static bool was_run_;
  void test_body() override { was_run_ = true; }
};
bool TestExecutionVerifier::was_run_ = false;

class TestExecutionVerifierShell : public mu::tiny::test::Shell
{
public:
  TestExecutionVerifierShell()
    : Shell("VerifierGroup", "VerifierTest", "file", 1)
  {
  }
  mu::tiny::test::Test* create_test() override
  {
    return new TestExecutionVerifier;
  }
};
} // namespace

TEST_GROUP(CommandLineRunner)
{
  mu::tiny::test::Registry registry;
  mu::tiny::test::Shell* test1;
  mu::tiny::test::Shell* test2;
  DummyPluginWhichCountsThePlugins* plugin_counting_plugin;

  void setup() override
  {
    test1 = new mu::tiny::test::Shell("group1", "test1", "file1", 1);
    test2 = new mu::tiny::test::Shell("group2", "test2", "file2", 2);
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

  mu::tiny::String run_and_get_output(const int argc, const char* argv[])
  {
    CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
        argc, argv, &registry
    );
    command_line_test_runner.run_all_tests_main();
    return command_line_test_runner
        .fake_console_output_which_is_really_a_buffer->get_output();
  }
};

TEST(CommandLineRunner, TwoBuiltinPluginsAreInstalledDuringTheRunningTheTests)
{
  const char* argv[] = { "tests.exe", "-psomething" };

  registry.install_plugin(plugin_counting_plugin);

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry
  );
  command_line_test_runner.run_all_tests_main();
  registry.remove_plugin_by_name("PluginCountingPlugin");

  CHECK_EQUAL(0, registry.count_plugins());
  CHECK_EQUAL(2, plugin_counting_plugin->amount_of_plugins);
}

TEST(CommandLineRunner, NoPluginsAreInstalledAtTheEndOfARunWhenTheArgumentsAreInvalid)
{
  const char* argv[] = { "tests.exe", "-fdskjnfkds" };

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry
  );
  command_line_test_runner.run_all_tests_main();

  CHECK_EQUAL(0, registry.count_plugins());
}

TEST(CommandLineRunner, ReturnsOneWhenTheArgumentsAreInvalid)
{
  const char* argv[] = { "tests.exe", "-some-invalid=parameter" };

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry
  );
  int returned = command_line_test_runner.run_all_tests_main();

  CHECK_EQUAL(1, returned);
}

TEST(CommandLineRunner, ReturnsZeroPrintsHelpOnHelp)
{
  const char* argv[] = { "tests.exe", "-h" };
  TestExecutionVerifierShell verifier_test;
  registry.add_test(&verifier_test);
  TestExecutionVerifier::was_run_ = false;

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry
  );
  int returned = command_line_test_runner.run_all_tests_main();

  CHECK_EQUAL(0, returned);
  STRCMP_CONTAINS(
      "Options that do not run tests but query:",
      command_line_test_runner.fake_console_output_which_is_really_a_buffer
          ->get_output()
          .c_str()
  );
  CHECK(!TestExecutionVerifier::was_run_);
}

TEST(CommandLineRunner, ReturnsOnePrintsHelpOnHelpWithInvalidArg)
{
  const char* argv[] = { "tests.exe", "-h", "-invalid" };
  TestExecutionVerifierShell verifier_test;
  registry.add_test(&verifier_test);
  TestExecutionVerifier::was_run_ = false;

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      3, argv, &registry
  );
  int returned = command_line_test_runner.run_all_tests_main();

  CHECK_EQUAL(1, returned);
  STRCMP_CONTAINS(
      "Options that do not run tests but query:",
      command_line_test_runner.fake_console_output_which_is_really_a_buffer
          ->get_output()
          .c_str()
  );
  CHECK(!TestExecutionVerifier::was_run_);
}

TEST(CommandLineRunner, ReturnsZeroWhenNoErrors)
{
  const char* argv[] = { "tests.exe" };

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      1, argv, &registry
  );
  int returned = command_line_test_runner.run_all_tests_main();

  CHECK_EQUAL(0, returned);
}

TEST(CommandLineRunner, ReturnsOneWhenNoTestsMatchProvidedFilter)
{
  const char* argv[] = { "tests.exe", "-g", "NoSuchGroup" };

  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      3, argv, &registry
  );
  int returned = command_line_test_runner.run_all_tests_main();

  CHECK_EQUAL(1, returned);
}

TEST(CommandLineRunner, veryVerboseSetOnOutput)
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

TEST(CommandLineRunner, defaultTestsAreRunInOrderTheyAreInRepository)
{
  const char* argv[] = { "tests.exe", "-v" };

  registry.add_test(test2);
  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry
  );
  command_line_test_runner.run_all_tests_main();

  auto output = command_line_test_runner
                    .fake_console_output_which_is_really_a_buffer->get_output();
  STRCMP_CONTAINS("test2", output.c_str());
  STRCMP_CONTAINS("test1", output.c_str());
}

TEST(CommandLineRunner, testsCanBeRunInReverseOrder)
{
  const char* argv[] = { "tests.exe", "-v", "-b" };

  registry.add_test(test2);
  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      3, argv, &registry
  );
  command_line_test_runner.run_all_tests_main();

  auto output = command_line_test_runner
                    .fake_console_output_which_is_really_a_buffer->get_output();
  STRCMP_CONTAINS("test1", output.c_str());
  STRCMP_CONTAINS("test2", output.c_str());
}

TEST(CommandLineRunner, listTestGroupNamesShouldWorkProperly)
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

TEST(CommandLineRunner, listTestGroupAndCaseNamesShouldWorkProperly)
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

TEST(CommandLineRunner, listTestLocationsShouldWorkProperly)
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

TEST(CommandLineRunner, randomShuffleSeedIsPrintedAndRandFuncIsExercised)
{
  // more than 1 item in test list ensures that shuffle algorithm calls rand_()
  auto* another_test = new mu::tiny::test::Shell("group", "test2", "file", 1);
  registry.add_test(another_test);

  const char* argv[] = { "tests.exe", "-s" };
  mu::tiny::String text = run_and_get_output(2, argv);
  STRCMP_CONTAINS("shuffling enabled with seed:", text.c_str());

  delete another_test;
}

TEST(CommandLineRunner, specificShuffleSeedIsPrintedVerbose)
{
  const char* argv[] = { "tests.exe", "-s2", "-v" };
  mu::tiny::String text = run_and_get_output(3, argv);
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
    mu::tiny::test::Output::fopen_ = fopen_fake;
    mu::tiny::test::Output::fputs_ = fputs_fake;
    mu::tiny::test::Output::fclose_ = fclose_fake;
  }

  void restore_originals()
  {
    mu::tiny::test::Output::fopen_ = save_f_open_;
    mu::tiny::test::Output::fputs_ = save_f_puts_;
    mu::tiny::test::Output::fclose_ = save_f_close_;
  }

  static mu::tiny::test::Output::File fopen_fake(const char*, const char*)
  {
    return static_cast<mu::tiny::test::Output::File>(nullptr);
  }

  static void fputs_fake(const char* str, mu::tiny::test::Output::File f)
  {
    if (f == mu::tiny::test::Output::stdout_) {
      current_fake_->console += str;
    } else {
      current_fake_->file += str;
    }
  }

  static void fclose_fake(mu::tiny::test::Output::File) {}

  mu::tiny::String file;
  mu::tiny::String console;

  static FakeOutput* current_fake_;

private:
  mu::tiny::test::Output::FOpenFunc save_f_open_{
    mu::tiny::test::Output::fopen_
  };
  mu::tiny::test::Output::FPutsFunc save_f_puts_{
    mu::tiny::test::Output::fputs_
  };
  mu::tiny::test::Output::FCloseFunc save_f_close_{
    mu::tiny::test::Output::fclose_
  };
};

FakeOutput* FakeOutput::current_fake_ = nullptr;
} // namespace

TEST(CommandLineRunner, realJunitOutputShouldBeCreatedAndWorkProperly)
{
  const char* argv[] = {
    "tests.exe",
    "-pjunit",
    "-v",
  };

  FakeOutput fake_output; /* MUTINY_PTR_SET() is not reentrant */

  mu::tiny::test::CommandLineRunner command_line_test_runner(
      3, argv, &registry
  );
  command_line_test_runner.run_all_tests_main();

  fake_output.restore_originals();

  STRCMP_CONTAINS(
      "<testcase classname=\"group1\" name=\"test1\"", fake_output.file.c_str()
  );
  STRCMP_CONTAINS("TEST(group1, test1)", fake_output.console.c_str());
}

TEST(CommandLineRunner, IgnoreTestWillBeIgnoredIfNoOptionSpecified)
{
  mu::tiny::test::Registry ignored_registry;
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

  CHECK(!RunIgnoredTest::checker_);
  RunIgnoredTest::checker_ = false;
}

TEST(CommandLineRunner, listOrderedTestLocations)
{
  const char* argv[] = { "tests.exe", "-lo" };
  CommandLineTestRunnerWithStringBufferOutput command_line_test_runner(
      2, argv, &registry
  );
  CHECK_EQUAL(0, command_line_test_runner.run_all_tests_main());
}

TEST(CommandLineRunner, IgnoreTestWillGetRunIfOptionSpecified)
{
  mu::tiny::test::Registry ignored_registry;
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

  CHECK(RunIgnoredTest::checker_);
  RunIgnoredTest::checker_ = false;
}
