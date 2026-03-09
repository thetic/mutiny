/*
 * Copyright (c) 2007, Michael Feathers, James Grenning and Bas Vodde
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE EARLIER MENTIONED AUTHORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "CppUTest/CommandLineTestRunner.h"

#include "CppUTest/JUnitTestOutput.h"
#include "CppUTest/PlatformSpecificFunctions.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/TestPlugin.h"
#include "CppUTest/TestRegistry.h"
#include "CppUTest/TestTestingFixture.h"

class DummyPluginWhichCountsThePlugins : public cpputest::TestPlugin
{
public:
  bool returnValue;
  int amountOfPlugins;

  DummyPluginWhichCountsThePlugins(const cpputest::SimpleString& name,
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

  cpputest::TestOutput* createJUnitOutput(
    const cpputest::SimpleString&) override
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

TEST_GROUP(CommandLineTestRunner) {
cpputest::TestRegistry registry;
cpputest::TestShell* test1;
cpputest::TestShell* test2;
DummyPluginWhichCountsThePlugins* pluginCountingPlugin;

void
setup() override
{
  test1 = new cpputest::TestShell("group1", "test1", "file1", 1);
  test2 = new cpputest::TestShell("group2", "test2", "file2", 2);
  registry.addTest(test1);
  pluginCountingPlugin =
    new DummyPluginWhichCountsThePlugins("PluginCountingPlugin", &registry);
}
void
teardown() override
{
  delete pluginCountingPlugin;
  delete test2;
  delete test1;
}

cpputest::SimpleString
runAndGetOutput(const int argc, const char* argv[])
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
  STRCMP_CONTAINS(
    "Thanks for using CppUTest.",
    commandLineTestRunner.fakeConsoleOutputWhichIsReallyABuffer->getOutput()
      .asCharString());
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
  STRCMP_CONTAINS(
    "TEST(group1, test1)",
    commandLineTestRunner.fakeJUnitOutputWhichIsReallyABuffer_->getOutput()
      .asCharString());
  STRCMP_CONTAINS(
    "TEST(group1, test1)",
    commandLineTestRunner.fakeConsoleOutputWhichIsReallyABuffer->getOutput()
      .asCharString());
}

TEST(CommandLineTestRunner, veryVerboseSetOnOutput)
{
  const char* argv[] = { "tests.exe", "-vv" };

  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
    2, argv, &registry);
  commandLineTestRunner.runAllTestsMain();
  STRCMP_CONTAINS(
    "TEST(group1, test1)",
    commandLineTestRunner.fakeConsoleOutputWhichIsReallyABuffer->getOutput()
      .asCharString());
  STRCMP_CONTAINS(
    "destroyTest",
    commandLineTestRunner.fakeConsoleOutputWhichIsReallyABuffer->getOutput()
      .asCharString());
}

TEST(CommandLineTestRunner, defaultTestsAreRunInOrderTheyAreInRepository)
{
  const char* argv[] = { "tests.exe", "-v" };

  registry.addTest(test2);
  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
    2, argv, &registry);
  commandLineTestRunner.runAllTestsMain();

  cpputest::SimpleStringCollection stringCollection;
  commandLineTestRunner.fakeConsoleOutputWhichIsReallyABuffer->getOutput()
    .split("\n", stringCollection);
  STRCMP_CONTAINS("test2", stringCollection[0].asCharString());
  STRCMP_CONTAINS("test1", stringCollection[1].asCharString());
}

TEST(CommandLineTestRunner, testsCanBeRunInReverseOrder)
{
  const char* argv[] = { "tests.exe", "-v", "-b" };

  registry.addTest(test2);
  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
    3, argv, &registry);
  commandLineTestRunner.runAllTestsMain();

  cpputest::SimpleStringCollection stringCollection;
  commandLineTestRunner.fakeConsoleOutputWhichIsReallyABuffer->getOutput()
    .split("\n", stringCollection);
  STRCMP_CONTAINS("test1", stringCollection[0].asCharString());
  STRCMP_CONTAINS("test2", stringCollection[1].asCharString());
}

TEST(CommandLineTestRunner, listTestGroupNamesShouldWorkProperly)
{
  const char* argv[] = { "tests.exe", "-lg" };

  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
    2, argv, &registry);
  commandLineTestRunner.runAllTestsMain();

  STRCMP_CONTAINS(
    "group",
    commandLineTestRunner.fakeConsoleOutputWhichIsReallyABuffer->getOutput()
      .asCharString());
}

TEST(CommandLineTestRunner, listTestGroupAndCaseNamesShouldWorkProperly)
{
  const char* argv[] = { "tests.exe", "-ln" };

  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
    2, argv, &registry);
  commandLineTestRunner.runAllTestsMain();

  STRCMP_CONTAINS(
    "group1.test1",
    commandLineTestRunner.fakeConsoleOutputWhichIsReallyABuffer->getOutput()
      .asCharString());
}

TEST(CommandLineTestRunner, listTestLocationsShouldWorkProperly)
{
  const char* argv[] = { "tests.exe", "-ll" };

  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
    2, argv, &registry);
  commandLineTestRunner.runAllTestsMain();

  STRCMP_CONTAINS(
    "group1.test1",
    commandLineTestRunner.fakeConsoleOutputWhichIsReallyABuffer->getOutput()
      .asCharString());
}

TEST(CommandLineTestRunner, randomShuffleSeedIsPrintedAndRandFuncIsExercised)
{
  // more than 1 item in test list ensures that shuffle algorithm calls rand_()
  cpputest::TestShell* anotherTest =
    new cpputest::TestShell("group", "test2", "file", 1);
  registry.addTest(anotherTest);

  const char* argv[] = { "tests.exe", "-s" };
  cpputest::SimpleString text = runAndGetOutput(2, argv);
  STRCMP_CONTAINS("shuffling enabled with seed:", text.asCharString());

  delete anotherTest;
}

TEST(CommandLineTestRunner, specificShuffleSeedIsPrintedVerbose)
{
  const char* argv[] = { "tests.exe", "-s2", "-v" };
  cpputest::SimpleString text = runAndGetOutput(3, argv);
  STRCMP_CONTAINS("shuffling enabled with seed: 2", text.asCharString());
}

extern "C"
{
  typedef PlatformSpecificFile (*FOpenFunc)(const char*, const char*);
  typedef void (*FPutsFunc)(const char*, PlatformSpecificFile);
  typedef void (*FCloseFunc)(PlatformSpecificFile);
}

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

  cpputest::SimpleString file;
  cpputest::SimpleString console;

  static FakeOutput* currentFake;

private:
  FOpenFunc SaveFOpen;
  FPutsFunc SaveFPuts;
  FCloseFunc SaveFClose;
};

FakeOutput* FakeOutput::currentFake = nullptr;

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
                  fakeOutput.file.asCharString());
  STRCMP_CONTAINS("TEST(group1, test1)", fakeOutput.console.asCharString());
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

  STRCMP_CONTAINS("##teamcity[testSuiteStarted name='group1'",
                  fakeOutput.console.asCharString());
  STRCMP_CONTAINS("##teamcity[testStarted name='test1'",
                  fakeOutput.console.asCharString());
  STRCMP_CONTAINS("##teamcity[testFinished name='test1'",
                  fakeOutput.console.asCharString());
  STRCMP_CONTAINS("##teamcity[testSuiteFinished name='group1'",
                  fakeOutput.console.asCharString());
}

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

TEST_GROUP(RunIgnoredTest) {
cpputest::TestRegistry registry;
RunIgnoredUtestShell* runIgnoredTest;
DummyPluginWhichCountsThePlugins* pluginCountingPlugin;

void
setup() override
{
  runIgnoredTest = new RunIgnoredUtestShell("group", "test", "file", 1);
  registry.addTest(runIgnoredTest);
  pluginCountingPlugin =
    new DummyPluginWhichCountsThePlugins("PluginCountingPlugin", &registry);
}
void
teardown() override
{
  delete pluginCountingPlugin;
  delete runIgnoredTest;
  RunIgnoredTest::Checker = false;
}
};

TEST(RunIgnoredTest, IgnoreTestWillBeIgnoredIfNoOptionSpecified)
{
  const char* argv[] = { "tests.exe" };

  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
    1, argv, &registry);
  commandLineTestRunner.runAllTestsMain();

  CHECK_FALSE(RunIgnoredTest::Checker);
}

TEST(RunIgnoredTest, IgnoreTestWillGetRunIfOptionSpecified)
{
  const char* argv[] = { "tests.exe", "-ri" };

  CommandLineTestRunnerWithStringBufferOutput commandLineTestRunner(
    2, argv, &registry);
  commandLineTestRunner.runAllTestsMain();

  CHECK_TRUE(RunIgnoredTest::Checker);
}
