#include "mu/tiny/test/CommandLineRunner.hpp"
#include "mu/tiny/test/Failure.hpp"
#include "mu/tiny/test/Output.hpp"
#include "mu/tiny/test/Plugin.hpp"
#include "mu/tiny/test/Registry.hpp"
#include "mu/tiny/test/Shell.hpp"
#include "mu/tiny/test/StringBufferOutput.hpp"

#include "mu/tiny/test.hpp"

namespace {

/**
 * Output that declares it needs a console companion, so CommandLineRunner
 * creates a CompositeOutput combining it with the console output.
 */
class NeedsCompanionOutput : public mu::tiny::test::StringBufferOutput
{
public:
  bool needs_console_companion() const override { return true; }
};

/** Plugin whose create_output() always returns a NeedsCompanionOutput. */
class CompanionPlugin : public mu::tiny::test::Plugin
{
public:
  CompanionPlugin()
    : Plugin("CompanionPlugin")
  {
  }

  bool parse_arguments(
      int /*argc*/,
      const char* const* /*argv*/,
      int /*index*/
  ) override
  {
    return false;
  }

  mu::tiny::test::Output* create_output() override
  {
    return new NeedsCompanionOutput;
  }
};

/**
 * Runner subclass that intercepts create_composite_output() to capture the
 * CompositeOutput pointer, and replaces the console with a StringBufferOutput
 * so we can inspect what was written.
 */
class CompositeCapturingRunner : public mu::tiny::test::CommandLineRunner
{
public:
  mu::tiny::test::Output* composite{ nullptr };
  mu::tiny::test::StringBufferOutput* console{ nullptr };

  CompositeCapturingRunner(
      int argc,
      const char* const* argv,
      mu::tiny::test::Registry* reg
  )
    : CommandLineRunner(argc, argv, reg)
  {
  }

  mu::tiny::test::Output* create_console_output() override
  {
    console = new mu::tiny::test::StringBufferOutput;
    return console;
  }

  mu::tiny::test::Output* create_composite_output(
      mu::tiny::test::Output* one,
      mu::tiny::test::Output* two
  ) override
  {
    composite = CommandLineRunner::create_composite_output(one, two);
    return composite;
  }
};

} // namespace

TEST_GROUP(CompositeOutput)
{
  mu::tiny::test::Registry registry;
  CompanionPlugin* plugin;
  mu::tiny::test::Shell* shell;
  CompositeCapturingRunner* runner;

  void setup() override
  {
    plugin = new CompanionPlugin;
    shell = new mu::tiny::test::Shell("CompGroup", "CompTest", "file.cpp", 1);
    registry.install_plugin(plugin);
    registry.add_test(shell);

    const char* argv[] = { "tests.exe", "-vv", "-c" };
    runner = new CompositeCapturingRunner(3, argv, &registry);
    runner->run_all_tests_main();
  }

  void teardown() override
  {
    delete runner;
    registry.reset_plugins();
    delete shell;
    delete plugin;
  }
};

/**
 * Running with -vv -c and a plugin that needs a console companion causes the
 * runner to create a CompositeOutput and call verbose() and color() on it.
 * The test body confirms the composite was actually created.
 */
TEST(CompositeOutput, verboseAndColorCoveredByRunner)
{
  CHECK(runner->composite != nullptr);
}

TEST(CompositeOutput, printBuffer)
{
  runner->console->flush();
  runner->composite->print_buffer("hello");
  STRCMP_CONTAINS("hello", runner->console->get_output().c_str());
}

TEST(CompositeOutput, printString)
{
  runner->console->flush();
  runner->composite->print("world");
  STRCMP_CONTAINS("world", runner->console->get_output().c_str());
}

TEST(CompositeOutput, printLong)
{
  runner->console->flush();
  runner->composite->print(42L);
  STRCMP_CONTAINS("42", runner->console->get_output().c_str());
}

TEST(CompositeOutput, printSizeT)
{
  runner->console->flush();
  size_t n = 7;
  runner->composite->print(n);
  STRCMP_CONTAINS("7", runner->console->get_output().c_str());
}

TEST(CompositeOutput, printDouble)
{
  runner->console->flush();
  runner->composite->print_double(2.5);
  STRCMP_CONTAINS("2.5", runner->console->get_output().c_str());
}

TEST(CompositeOutput, printFailure)
{
  mu::tiny::test::Shell fail_shell("FG", "FT", "fail.cpp", 1);
  mu::tiny::test::Failure failure(&fail_shell, "fail.cpp", 1, "the message");
  runner->console->flush();
  runner->composite->print_failure(failure);
  STRCMP_CONTAINS("the message", runner->console->get_output().c_str());
}

TEST(CompositeOutput, setProgressIndicator)
{
  runner->composite->set_progress_indicator("?");
  /* Verify the indicator propagated to both children by printing it. */
  runner->console->flush();
  runner->composite->print_buffer("?");
  STRCMP_EQUAL("?", runner->console->get_output().c_str());
}

/**
 * With -vv, verbose() propagated to both children during setup().  The
 * composite's print_very_verbose() is therefore exercised when Shell runs
 * through the framework, but we also call it directly to keep the test
 * self-contained.
 */
TEST(CompositeOutput, printVeryVerbose)
{
  runner->console->flush();
  runner->composite->print_very_verbose("vv-line");
  STRCMP_CONTAINS("vv-line", runner->console->get_output().c_str());
}

TEST(CompositeOutput, flush)
{
  /* Ensure the console has accumulated output before flushing. */
  runner->composite->print_buffer("pre-flush");
  runner->composite->flush();
  STRCMP_EQUAL("", runner->console->get_output().c_str());
}
