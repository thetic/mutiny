#include "CppUTest/CompositeTestOutput.hpp"

namespace cpputest {

CompositeTestOutput::CompositeTestOutput()
  : outputOne_(nullptr)
  , outputTwo_(nullptr)
{
}

CompositeTestOutput::~CompositeTestOutput()
{
  delete outputOne_;
  delete outputTwo_;
}

void
CompositeTestOutput::setOutputOne(TestOutput* output)
{
  delete outputOne_;
  outputOne_ = output;
}

void
CompositeTestOutput::setOutputTwo(TestOutput* output)
{
  delete outputTwo_;
  outputTwo_ = output;
}

void
CompositeTestOutput::printTestsStarted()
{
  if (outputOne_)
    outputOne_->printTestsStarted();
  if (outputTwo_)
    outputTwo_->printTestsStarted();
}

void
CompositeTestOutput::printTestsEnded(const TestResult& result)
{
  if (outputOne_)
    outputOne_->printTestsEnded(result);
  if (outputTwo_)
    outputTwo_->printTestsEnded(result);
}

void
CompositeTestOutput::printCurrentTestStarted(const TestShell& test)
{
  if (outputOne_)
    outputOne_->printCurrentTestStarted(test);
  if (outputTwo_)
    outputTwo_->printCurrentTestStarted(test);
}

void
CompositeTestOutput::printCurrentTestEnded(const TestResult& res)
{
  if (outputOne_)
    outputOne_->printCurrentTestEnded(res);
  if (outputTwo_)
    outputTwo_->printCurrentTestEnded(res);
}

void
CompositeTestOutput::printCurrentGroupStarted(const TestShell& test)
{
  if (outputOne_)
    outputOne_->printCurrentGroupStarted(test);
  if (outputTwo_)
    outputTwo_->printCurrentGroupStarted(test);
}

void
CompositeTestOutput::printCurrentGroupEnded(const TestResult& res)
{
  if (outputOne_)
    outputOne_->printCurrentGroupEnded(res);
  if (outputTwo_)
    outputTwo_->printCurrentGroupEnded(res);
}

void
CompositeTestOutput::verbose(VerbosityLevel level)
{
  if (outputOne_)
    outputOne_->verbose(level);
  if (outputTwo_)
    outputTwo_->verbose(level);
}

void
CompositeTestOutput::color()
{
  if (outputOne_)
    outputOne_->color();
  if (outputTwo_)
    outputTwo_->color();
}

void
CompositeTestOutput::printBuffer(const char* buffer)
{
  if (outputOne_)
    outputOne_->printBuffer(buffer);
  if (outputTwo_)
    outputTwo_->printBuffer(buffer);
}

void
CompositeTestOutput::print(const char* buffer)
{
  if (outputOne_)
    outputOne_->print(buffer);
  if (outputTwo_)
    outputTwo_->print(buffer);
}

void
CompositeTestOutput::print(long number)
{
  if (outputOne_)
    outputOne_->print(number);
  if (outputTwo_)
    outputTwo_->print(number);
}

void
CompositeTestOutput::print(size_t number)
{
  if (outputOne_)
    outputOne_->print(number);
  if (outputTwo_)
    outputTwo_->print(number);
}

void
CompositeTestOutput::printDouble(double number)
{
  if (outputOne_)
    outputOne_->printDouble(number);
  if (outputTwo_)
    outputTwo_->printDouble(number);
}

void
CompositeTestOutput::printFailure(const TestFailure& failure)
{
  if (outputOne_)
    outputOne_->printFailure(failure);
  if (outputTwo_)
    outputTwo_->printFailure(failure);
}

void
CompositeTestOutput::setProgressIndicator(const char* indicator)
{
  if (outputOne_)
    outputOne_->setProgressIndicator(indicator);
  if (outputTwo_)
    outputTwo_->setProgressIndicator(indicator);
}

void
CompositeTestOutput::printVeryVerbose(const char* str)
{
  if (outputOne_)
    outputOne_->printVeryVerbose(str);
  if (outputTwo_)
    outputTwo_->printVeryVerbose(str);
}

void
CompositeTestOutput::flush()
{
  if (outputOne_)
    outputOne_->flush();
  if (outputTwo_)
    outputTwo_->flush();
}

} // namespace cpputest
