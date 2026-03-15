#include "CppUTest/CommandLineTestRunner.hpp"

#include "CppUTest/JUnitTestOutput.hpp"
#include "CppUTest/PlatformSpecificFunctions.h"
#include "CppUTest/StringCollection.hpp"
#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestPlugin.hpp"
#include "CppUTest/TestRegistry.hpp"
#include "CppUTest/TestTestingFixture.hpp"

namespace {
class DummyPluginWhichCountsThePlugins : public cpputest::TestPlugin
{
public:
  bool returnValue;
  int amountOfPlugins;

  DummyPluginWhichCountsThePlugins(const cpputest::String& name,
      cpputest::TestRegistry* registry)
    : TestPlugin(name)
    , returnValue(true)
    , amountOfPlugins(0)
    , registry_(registry)
  {
  }

  virtual bool parseArguments(int, const char* const*, int) override
  {
    /* Remove ourselves from the count */
    amountOfPlugins = registry_->countPlugins() - 1;
    return returnValue;
  }

private:
  cpputest::TestRegistry* registry_;
};

class CommandLineTestRunnerWithStringBufferOutput
  : public cpputest::CommandLineTestRunner
{
public:
  cpputest::StringBufferTestOutput* fakeJUnitOutputWhichIsReallyABuffer_;
  cpputest::StringBufferTestOutput* fakeConsoleOutputWhichIsReallyABuffer;
  cpputest::StringBufferTestOutput* fakeTCOutputWhichIsReallyABuffer;

  CommandLineTestRunnerWithStringBufferOutput(int argc,
      const char* const* argv,
      cpputest::TestRegistry* registry)
    : CommandLineTestRunner(argc, argv, registry)
    , fakeJUnitOutputWhichIsReallyABuffer_(nullptr)
    , fakeConsoleOutputWhichIsReallyABuffer(nullptr)
    , fakeTCOutputWhichIsReallyABuffer(nullptr)
  {
  }

  cpputest::TestOutput* createConsoleOutput() override
  {
    fakeConsoleOutputWhichIsReallyABuffer =
        new cpputest::StringBufferTestOutput;
    return fakeConsoleOutputWhichIsReallyABuffer;
  }

  cpputest::TestOutput* createJUnitOutput(const cpputest::String&) override
  {
    fakeJUnitOutputWhichIsReallyABuffer_ = new cpputest::StringBufferTestOutput;
    return fakeJUnitOutputWhichIsReallyABuffer_;
  }

  cpputest::TestOutput* createTeamCityOutput() override
  {
    fakeTCOutputWhichIsReallyABuffer = new cpputest::StringBufferTestOutput;
    return fakeTCOutputWhichIsReallyABuffer;
  }
};

class RunIgnoredTest : public cpputest::Test
{
public:
  static bool Checker;
  void testBody() override { Checker = true; }
};

bool RunIgnoredTest::Checker = false;

class RunIgnoredUtestShell : public cpputest::IgnoredTestShell
{
public:
  RunIgnoredUtestShell(const char* groupName,
      const char* testName,
      const char* fileName,
      size_t lineNumber)
    : IgnoredTestShell(groupName, testName, fileName, lineNumber)
  {
  }
  virtual cpputest::Test* createTest() override { return new RunIgnoredTest; }
};
} // namespace

TEST_GROUP(CommandLineTestRunner)
{
  cpputest::TestRegistry registry;
  cpputest::TestShell* test1;
  cpputest::TestShell* test2;
  DummyPluginWhichCountsThePlugins* pluginCountingPlugin;

  void setup() override
  {
    test1 = new cpputest::TestShell("group1", "test1", "file1", 1);
    test2 = new cpputest::TestShell("group2", "test2", "file2", 2);
    registry.addTest(test1);
    pluginCountingPlugin =
        new DummyPluginWhichCountsThePlugins("PluginCountingPlugin", &registry);
  }
  void teardown() override
  {
    delete pluginCountingPlugin;
    delete test2;
    delete test1;
  }

  cpputest::String runAndGetOutput(const int argc, const char* argv[])
  {
    CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
        argc, argv, &registry);
    commandLineTestRunner.runAllTestsMain();
    return commandLineTestRunner.fakeConsoleOutputWhichIsReallyABuffer
        ->getOutput();
  }
};

TEST(CommandLineTestRunner, OnePluginGetsInstalledDuringTheRunningTheTests)
{
  const char* argv[] = { "tests.exe", "-psomething" };

  registry.installPlugin(pluginCountingPlugin);

  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
      2, argv, &registry);
  commandLineTestRunner.runAllTestsMain();
  registry.removePluginByName("PluginCountingPlugin");

  LONGS_EQUAL(0, registry.countPlugins());
  LONGS_EQUAL(1, pluginCountingPlugin->amountOfPlugins);
}

TEST(CommandLineTestRunner,
    NoPluginsAreInstalledAtTheEndOfARunWhenTheArgumentsAreInvalid)
{
  const char* argv[] = { "tests.exe", "-fdskjnfkds" };

  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
      2, argv, &registry);
  commandLineTestRunner.runAllTestsMain();

  LONGS_EQUAL(0, registry.countPlugins());
}

TEST(CommandLineTestRunner, ReturnsOneWhenTheArgumentsAreInvalid)
{
  const char* argv[] = { "tests.exe", "-some-invalid=parameter" };

  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
      2, argv, &registry);
  int returned = commandLineTestRunner.runAllTestsMain();

  LONGS_EQUAL(1, returned);
}

TEST(CommandLineTestRunner, ReturnsOnePrintsHelpOnHelp)
{
  const char* argv[] = { "tests.exe", "-h" };

  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
      2, argv, &registry);
  int returned = commandLineTestRunner.runAllTestsMain();

  LONGS_EQUAL(1, returned);
  STRCMP_CONTAINS("Thanks for using CppUTest.",
      commandLineTestRunner.fakeConsoleOutputWhichIsReallyABuffer->getOutput()
          .c_str());
}

TEST(CommandLineTestRunner, ReturnsZeroWhenNoErrors)
{
  const char* argv[] = { "tests.exe" };

  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
      1, argv, &registry);
  int returned = commandLineTestRunner.runAllTestsMain();

  LONGS_EQUAL(0, returned);
}

TEST(CommandLineTestRunner, ReturnsOneWhenNoTestsMatchProvidedFilter)
{
  const char* argv[] = { "tests.exe", "-g", "NoSuchGroup" };

  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
      3, argv, &registry);
  int returned = commandLineTestRunner.runAllTestsMain();

  LONGS_EQUAL(1, returned);
}

TEST(CommandLineTestRunner, TeamcityOutputEnabled)
{
  const char* argv[] = { "tests.exe", "-oteamcity" };
  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
      2, argv, &registry);
  commandLineTestRunner.runAllTestsMain();
  CHECK(commandLineTestRunner.fakeTCOutputWhichIsReallyABuffer != nullptr);
}

TEST(CommandLineTestRunner, JunitOutputEnabled)
{
  const char* argv[] = { "tests.exe", "-ojunit" };

  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
      2, argv, &registry);
  commandLineTestRunner.runAllTestsMain();
  CHECK(commandLineTestRunner.fakeJUnitOutputWhichIsReallyABuffer_ != nullptr);
}

TEST(CommandLineTestRunner, JunitOutputAndVerboseEnabled)
{
  const char* argv[] = { "tests.exe", "-ojunit", "-v" };

  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
      3, argv, &registry);
  commandLineTestRunner.runAllTestsMain();
  STRCMP_CONTAINS("TEST(group1, test1)",
      commandLineTestRunner.fakeJUnitOutputWhichIsReallyABuffer_->getOutput()
          .c_str());
  STRCMP_CONTAINS("TEST(group1, test1)",
      commandLineTestRunner.fakeConsoleOutputWhichIsReallyABuffer->getOutput()
          .c_str());
}

TEST(CommandLineTestRunner, veryVerboseSetOnOutput)
{
  const char* argv[] = { "tests.exe", "-vv" };

  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
      2, argv, &registry);
  commandLineTestRunner.runAllTestsMain();
  STRCMP_CONTAINS("TEST(group1, test1)",
      commandLineTestRunner.fakeConsoleOutputWhichIsReallyABuffer->getOutput()
          .c_str());
  STRCMP_CONTAINS("destroyTest",
      commandLineTestRunner.fakeConsoleOutputWhichIsReallyABuffer->getOutput()
          .c_str());
}

TEST(CommandLineTestRunner, defaultTestsAreRunInOrderTheyAreInRepository)
{
  const char* argv[] = { "tests.exe", "-v" };

  registry.addTest(test2);
  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
      2, argv, &registry);
  commandLineTestRunner.runAllTestsMain();

  cpputest::StringCollection stringCollection(
      commandLineTestRunner.fakeConsoleOutputWhichIsReallyABuffer->getOutput(),
      '\n');
  STRCMP_CONTAINS("test2", stringCollection[0].c_str());
  STRCMP_CONTAINS("test1", stringCollection[1].c_str());
}

TEST(CommandLineTestRunner, testsCanBeRunInReverseOrder)
{
  const char* argv[] = { "tests.exe", "-v", "-b" };

  registry.addTest(test2);
  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
      3, argv, &registry);
  commandLineTestRunner.runAllTestsMain();

  cpputest::StringCollection stringCollection(
      commandLineTestRunner.fakeConsoleOutputWhichIsReallyABuffer->getOutput(),
      '\n');
  STRCMP_CONTAINS("test1", stringCollection[0].c_str());
  STRCMP_CONTAINS("test2", stringCollection[1].c_str());
}

TEST(CommandLineTestRunner, listTestGroupNamesShouldWorkProperly)
{
  const char* argv[] = { "tests.exe", "-lg" };

  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
      2, argv, &registry);
  commandLineTestRunner.runAllTestsMain();

  STRCMP_CONTAINS("group",
      commandLineTestRunner.fakeConsoleOutputWhichIsReallyABuffer->getOutput()
          .c_str());
}

TEST(CommandLineTestRunner, listTestGroupAndCaseNamesShouldWorkProperly)
{
  const char* argv[] = { "tests.exe", "-ln" };

  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
      2, argv, &registry);
  commandLineTestRunner.runAllTestsMain();

  STRCMP_CONTAINS("group1.test1",
      commandLineTestRunner.fakeConsoleOutputWhichIsReallyABuffer->getOutput()
          .c_str());
}

TEST(CommandLineTestRunner, listTestLocationsShouldWorkProperly)
{
  const char* argv[] = { "tests.exe", "-ll" };

  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
      2, argv, &registry);
  commandLineTestRunner.runAllTestsMain();

  STRCMP_CONTAINS("group1.test1",
      commandLineTestRunner.fakeConsoleOutputWhichIsReallyABuffer->getOutput()
          .c_str());
}

TEST(CommandLineTestRunner, randomShuffleSeedIsPrintedAndRandFuncIsExercised)
{
  // more than 1 item in test list ensures that shuffle algorithm calls rand_()
  auto* anotherTest = new cpputest::TestShell("group", "test2", "file", 1);
  registry.addTest(anotherTest);

  const char* argv[] = { "tests.exe", "-s" };
  cpputest::String text = runAndGetOutput(2, argv);
  STRCMP_CONTAINS("shuffling enabled with seed:", text.c_str());

  delete anotherTest;
}

TEST(CommandLineTestRunner, specificShuffleSeedIsPrintedVerbose)
{
  const char* argv[] = { "tests.exe", "-s2", "-v" };
  cpputest::String text = runAndGetOutput(3, argv);
  STRCMP_CONTAINS("shuffling enabled with seed: 2", text.c_str());
}

namespace {
using FOpenFunc = PlatformSpecificFile (*)(const char*, const char*);
using FPutsFunc = void (*)(const char*, PlatformSpecificFile);
using FCloseFunc = void (*)(PlatformSpecificFile);

struct FakeOutput
{
  FakeOutput()
    : SaveFOpen(PlatformSpecificFOpen)
    , SaveFPuts(PlatformSpecificFPuts)
    , SaveFClose(PlatformSpecificFClose)
  {
    installFakes();
    currentFake = this;
  }

  ~FakeOutput()
  {
    currentFake = nullptr;
    restoreOriginals();
  }

  void installFakes()
  {
    PlatformSpecificFOpen = reinterpret_cast<FOpenFunc>(fopen_fake);
    PlatformSpecificFPuts = reinterpret_cast<FPutsFunc>(fputs_fake);
    PlatformSpecificFClose = reinterpret_cast<FCloseFunc>(fclose_fake);
  }

  void restoreOriginals()
  {
    PlatformSpecificFOpen = SaveFOpen;
    PlatformSpecificFPuts = SaveFPuts;
    PlatformSpecificFClose = SaveFClose;
  }

  static PlatformSpecificFile fopen_fake(const char*, const char*)
  {
    return static_cast<PlatformSpecificFile>(nullptr);
  }

  static void fputs_fake(const char* str, PlatformSpecificFile f)
  {
    if (f == PlatformSpecificStdOut) {
      currentFake->console += str;
    } else {
      currentFake->file += str;
    }
  }

  static void fclose_fake(PlatformSpecificFile) {}

  cpputest::String file;
  cpputest::String console;

  static FakeOutput* currentFake;

private:
  FOpenFunc SaveFOpen;
  FPutsFunc SaveFPuts;
  FCloseFunc SaveFClose;
};

FakeOutput* FakeOutput::currentFake = nullptr;
}

TEST(CommandLineTestRunner, realJunitOutputShouldBeCreatedAndWorkProperly)
{
  const char* argv[] = {
    "tests.exe",
    "-ojunit",
    "-v",
    "-kpackage",
  };

  FakeOutput fakeOutput; /* UT_PTR_SET() is not reentrant */

  cpputest::CommandLineTestRunner commandLineTestRunner(4, argv, &registry);
  commandLineTestRunner.runAllTestsMain();

  fakeOutput.restoreOriginals();

  STRCMP_CONTAINS("<testcase classname=\"package.group1\" name=\"test1\"",
      fakeOutput.file.c_str());
  STRCMP_CONTAINS("TEST(group1, test1)", fakeOutput.console.c_str());
}

TEST(CommandLineTestRunner, realTeamCityOutputShouldBeCreatedAndWorkProperly)
{
  const char* argv[] = {
    "tests.exe",
    "-oteamcity",
    "-v",
    "-kpackage",
  };

  FakeOutput fakeOutput; /* UT_PTR_SET() is not reentrant */

  cpputest::CommandLineTestRunner commandLineTestRunner(4, argv, &registry);
  commandLineTestRunner.runAllTestsMain();

  fakeOutput.restoreOriginals();

  STRCMP_CONTAINS(
      "##teamcity[testSuiteStarted name='group1'", fakeOutput.console.c_str());
  STRCMP_CONTAINS(
      "##teamcity[testStarted name='test1'", fakeOutput.console.c_str());
  STRCMP_CONTAINS(
      "##teamcity[testFinished name='test1'", fakeOutput.console.c_str());
  STRCMP_CONTAINS(
      "##teamcity[testSuiteFinished name='group1'", fakeOutput.console.c_str());
}

TEST(CommandLineTestRunner, IgnoreTestWillBeIgnoredIfNoOptionSpecified)
{
  cpputest::TestRegistry ignoredRegistry;
  RunIgnoredUtestShell runIgnoredTest("group", "test", "file", 1);
  ignoredRegistry.addTest(&runIgnoredTest);
  DummyPluginWhichCountsThePlugins ignoredPlugin(
      "PluginCountingPlugin", &ignoredRegistry);

  const char* argv[] = { "tests.exe" };
  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
      1, argv, &ignoredRegistry);
  commandLineTestRunner.runAllTestsMain();

  CHECK_FALSE(RunIgnoredTest::Checker);
  RunIgnoredTest::Checker = false;
}

TEST(CommandLineTestRunner, IgnoreTestWillGetRunIfOptionSpecified)
{
  cpputest::TestRegistry ignoredRegistry;
  RunIgnoredUtestShell runIgnoredTest("group", "test", "file", 1);
  ignoredRegistry.addTest(&runIgnoredTest);
  DummyPluginWhichCountsThePlugins ignoredPlugin(
      "PluginCountingPlugin", &ignoredRegistry);

  const char* argv[] = { "tests.exe", "-ri" };
  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
      2, argv, &ignoredRegistry);
  commandLineTestRunner.runAllTestsMain();

  CHECK_TRUE(RunIgnoredTest::Checker);
  RunIgnoredTest::Checker = false;
}
