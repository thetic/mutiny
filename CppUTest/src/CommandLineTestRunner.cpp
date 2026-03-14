#include "CppUTest/CommandLineTestRunner.hpp"

#include "CppUTest/CompositeTestOutput.hpp"
#include "CppUTest/JUnitTestOutput.hpp"
#include "CppUTest/SetPointerPlugin.hpp"
#include "CppUTest/TeamCityTestOutput.hpp"
#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestOutput.hpp"
#include "CppUTest/TestRegistry.hpp"

namespace cpputest {

int
CommandLineTestRunner::RunAllTests(int argc, char** argv)
{
  return RunAllTests(argc, reinterpret_cast<const char* const*>(argv));
}

int
CommandLineTestRunner::RunAllTests(int argc, const char* const* argv)
{
  CommandLineTestRunner runner(argc, argv, TestRegistry::getCurrentRegistry());
  return runner.runAllTestsMain();
}

CommandLineTestRunner::CommandLineTestRunner(int argc,
    const char* const* argv,
    TestRegistry* registry)
  : output_(nullptr)
  , arguments_(nullptr)
  , registry_(registry)
{
  arguments_ = new CommandLineArguments(argc, argv);
}

CommandLineTestRunner::~CommandLineTestRunner()
{
  delete arguments_;
  delete output_;
}

int
CommandLineTestRunner::runAllTestsMain()
{
  int testResult = 1;

  SetPointerPlugin pPlugin(DEF_PLUGIN_SET_POINTER);
  registry_->installPlugin(&pPlugin);

  if (parseArguments(registry_->getFirstPlugin()))
    testResult = runAllTests();

  registry_->removePluginByName(DEF_PLUGIN_SET_POINTER);
  return testResult;
}

void
CommandLineTestRunner::initializeTestRun()
{
  registry_->setGroupFilters(arguments_->getGroupFilters());
  registry_->setNameFilters(arguments_->getNameFilters());

  if (arguments_->isVerbose())
    output_->verbose(TestOutput::VerbosityLevel::VERBOSE);
  if (arguments_->isVeryVerbose())
    output_->verbose(TestOutput::VerbosityLevel::VERY_VERBOSE);
  if (arguments_->isColor())
    output_->color();
  if (arguments_->isRunIgnored())
    registry_->setRunIgnored();
  if (arguments_->isCrashingOnFail())
    TestShell::setCrashOnFail();

  TestShell::setRethrowExceptions(arguments_->isRethrowingExceptions());
}

int
CommandLineTestRunner::runAllTests()
{
  initializeTestRun();
  size_t loopCount = 0;
  size_t failedTestCount = 0;
  size_t failedExecutionCount = 0;
  size_t repeatCount = arguments_->getRepeatCount();

  if (arguments_->isListingTestGroupNames()) {
    TestResult tr(*output_);
    registry_->listTestGroupNames(tr);
    return 0;
  }

  if (arguments_->isListingTestGroupAndCaseNames()) {
    TestResult tr(*output_);
    registry_->listTestGroupAndCaseNames(tr);
    return 0;
  }

  if (arguments_->isListingTestLocations()) {
    TestResult tr(*output_);
    registry_->listTestLocations(tr);
    return 0;
  }

  if (arguments_->isReversing())
    registry_->reverseTests();

  if (arguments_->isShuffling()) {
    output_->print("Test order shuffling enabled with seed: ");
    output_->print(arguments_->getShuffleSeed());
    output_->print("\n");
  }
  while (loopCount++ < repeatCount) {

    if (arguments_->isShuffling())
      registry_->shuffleTests(arguments_->getShuffleSeed());

    output_->printTestRun(loopCount, repeatCount);
    TestResult tr(*output_);
    registry_->runAllTests(tr);
    failedTestCount += tr.getFailureCount();
    if (tr.isFailure()) {
      failedExecutionCount++;
    }
  }
  return static_cast<int>(
      failedTestCount != 0 ? failedTestCount : failedExecutionCount);
}

TestOutput*
CommandLineTestRunner::createTeamCityOutput()
{
  return new TeamCityTestOutput;
}

TestOutput*
CommandLineTestRunner::createJUnitOutput(const String& packageName)
{
  JUnitTestOutput* junitOutput = new JUnitTestOutput;
  if (junitOutput != nullptr) {
    junitOutput->setPackageName(packageName);
  }
  return junitOutput;
}

TestOutput*
CommandLineTestRunner::createConsoleOutput()
{
  return new ConsoleTestOutput;
}

TestOutput*
CommandLineTestRunner::createCompositeOutput(TestOutput* outputOne,
    TestOutput* outputTwo)
{
  CompositeTestOutput* composite = new CompositeTestOutput;
  composite->setOutputOne(outputOne);
  composite->setOutputTwo(outputTwo);
  return composite;
}

bool
CommandLineTestRunner::parseArguments(TestPlugin* plugin)
{
  if (!arguments_->parse(plugin)) {
    output_ = createConsoleOutput();
    output_->print(
        (arguments_->needHelp()) ? arguments_->help() : arguments_->usage());
    return false;
  }

  if (arguments_->isJUnitOutput()) {
    output_ = createJUnitOutput(arguments_->getPackageName());
    if (arguments_->isVerbose() || arguments_->isVeryVerbose())
      output_ = createCompositeOutput(output_, createConsoleOutput());
  } else if (arguments_->isTeamCityOutput()) {
    output_ = createTeamCityOutput();
  } else
    output_ = createConsoleOutput();
  return true;
}

} // namespace cpputest
