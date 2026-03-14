#include "CppUTest/TestTestingFixture.hpp"

#include "CppUTest/ExecFunctionTestShell.hpp"
#include "CppUTest/TestHarness.hpp"

namespace cpputest {

bool TestTestingFixture::lineOfCodeExecutedAfterCheck = false;

TestTestingFixture::TestTestingFixture()
{
  output_ = new StringBufferTestOutput();
  result_ = new TestResult(*output_);
  genTest_ = new ExecFunctionTestShell();
  registry_ = new TestRegistry();
  ownsExecFunction_ = false;

  registry_->setCurrentRegistry(registry_);
  registry_->addTest(genTest_);

  lineOfCodeExecutedAfterCheck = false;
}

void
TestTestingFixture::flushOutputAndResetResult()
{
  output_->flush();
  delete result_;
  result_ = new TestResult(*output_);
}

TestTestingFixture::~TestTestingFixture()
{
  registry_->setCurrentRegistry(nullptr);
  clearExecFunction();
  delete registry_;
  delete result_;
  delete output_;
  delete genTest_;
}

void
TestTestingFixture::clearExecFunction()
{
  if (genTest_->testFunction_ && ownsExecFunction_)
    delete genTest_->testFunction_;
}

void
TestTestingFixture::addTest(TestShell* test)
{
  registry_->addTest(test);
}

void
TestTestingFixture::setTestFunction(void (*testFunction)())
{
  clearExecFunction();

  genTest_->testFunction_ = new ExecFunctionWithoutParameters(testFunction);
  ownsExecFunction_ = true;
}

void
TestTestingFixture::setTestFunction(ExecFunction* testFunction)
{
  clearExecFunction();

  genTest_->testFunction_ = testFunction;

  ownsExecFunction_ = false;
}

void
TestTestingFixture::setSetup(void (*setupFunction)())
{
  genTest_->setup_ = setupFunction;
}

void
TestTestingFixture::setTeardown(void (*teardownFunction)())
{
  genTest_->teardown_ = teardownFunction;
}

void
TestTestingFixture::installPlugin(TestPlugin* plugin)
{
  registry_->installPlugin(plugin);
}

void
TestTestingFixture::setOutputVerbose()
{
  output_->verbose(TestOutput::VerbosityLevel::VERBOSE);
}

void
TestTestingFixture::runTestWithMethod(void (*method)())
{
  setTestFunction(method);
  runAllTests();
}

void
TestTestingFixture::runAllTests()
{
  registry_->runAllTests(*result_);
}

size_t
TestTestingFixture::getFailureCount()
{
  return result_->getFailureCount();
}

size_t
TestTestingFixture::getCheckCount()
{
  return result_->getCheckCount();
}

size_t
TestTestingFixture::getTestCount()
{
  return result_->getTestCount();
}

size_t
TestTestingFixture::getIgnoreCount()
{
  return result_->getIgnoredCount();
}

TestRegistry*
TestTestingFixture::getRegistry()
{
  return registry_;
}

bool
TestTestingFixture::hasTestFailed()
{
  return genTest_->hasFailed();
}

void
TestTestingFixture::assertPrintContains(const String& contains)
{
  STRCMP_CONTAINS(contains.c_str(), getOutput().c_str());
}

void
TestTestingFixture::assertPrintContainsNot(const String& contains)
{
  CHECK(!stringContains(getOutput(), contains));
}

const String&
TestTestingFixture::getOutput()
{
  return output_->getOutput();
}

size_t
TestTestingFixture::getRunCount()
{
  return result_->getRunCount();
}

void
TestTestingFixture::lineExecutedAfterCheck()
{
  lineOfCodeExecutedAfterCheck = true;
}

void
TestTestingFixture::checkTestFailsWithProperTestLocation(const char* text,
    const char* file,
    size_t line)
{
  if (getFailureCount() != 1)
    FAIL_LOCATION(
        StringFromFormat(
            "Expected one test failure, but got %d amount of test failures",
            static_cast<int>(getFailureCount()))
            .c_str(),
        file,
        line);

  STRCMP_CONTAINS_LOCATION(text, output_->getOutput().c_str(), "", file, line);

  if (lineOfCodeExecutedAfterCheck)
    FAIL_LOCATION("The test should jump/throw on failure and not execute the "
                  "next line. However, the next line was executed.",
        file,
        line);
}

} // namespace cpputest
