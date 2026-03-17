#include "CppMu/CompositeTestOutput.hpp"

namespace cppmu {

CompositeTestOutput::CompositeTestOutput()
  : output_one_(nullptr)
  , output_two_(nullptr)
{
}

CompositeTestOutput::~CompositeTestOutput()
{
  delete output_one_;
  delete output_two_;
}

void CompositeTestOutput::set_output_one(TestOutput* output)
{
  delete output_one_;
  output_one_ = output;
}

void CompositeTestOutput::set_output_two(TestOutput* output)
{
  delete output_two_;
  output_two_ = output;
}

void CompositeTestOutput::print_tests_started()
{
  if (output_one_)
    output_one_->print_tests_started();
  if (output_two_)
    output_two_->print_tests_started();
}

void CompositeTestOutput::print_tests_ended(const TestResult& result)
{
  if (output_one_)
    output_one_->print_tests_ended(result);
  if (output_two_)
    output_two_->print_tests_ended(result);
}

void CompositeTestOutput::print_current_test_started(const TestShell& test)
{
  if (output_one_)
    output_one_->print_current_test_started(test);
  if (output_two_)
    output_two_->print_current_test_started(test);
}

void CompositeTestOutput::print_current_test_ended(const TestResult& res)
{
  if (output_one_)
    output_one_->print_current_test_ended(res);
  if (output_two_)
    output_two_->print_current_test_ended(res);
}

void CompositeTestOutput::print_current_group_started(const TestShell& test)
{
  if (output_one_)
    output_one_->print_current_group_started(test);
  if (output_two_)
    output_two_->print_current_group_started(test);
}

void CompositeTestOutput::print_current_group_ended(const TestResult& res)
{
  if (output_one_)
    output_one_->print_current_group_ended(res);
  if (output_two_)
    output_two_->print_current_group_ended(res);
}

void CompositeTestOutput::verbose(VerbosityLevel level)
{
  if (output_one_)
    output_one_->verbose(level);
  if (output_two_)
    output_two_->verbose(level);
}

void CompositeTestOutput::color()
{
  if (output_one_)
    output_one_->color();
  if (output_two_)
    output_two_->color();
}

void CompositeTestOutput::print_buffer(const char* buffer)
{
  if (output_one_)
    output_one_->print_buffer(buffer);
  if (output_two_)
    output_two_->print_buffer(buffer);
}

void CompositeTestOutput::print(const char* buffer)
{
  if (output_one_)
    output_one_->print(buffer);
  if (output_two_)
    output_two_->print(buffer);
}

void CompositeTestOutput::print(long number)
{
  if (output_one_)
    output_one_->print(number);
  if (output_two_)
    output_two_->print(number);
}

void CompositeTestOutput::print(size_t number)
{
  if (output_one_)
    output_one_->print(number);
  if (output_two_)
    output_two_->print(number);
}

void CompositeTestOutput::print_double(double number)
{
  if (output_one_)
    output_one_->print_double(number);
  if (output_two_)
    output_two_->print_double(number);
}

void CompositeTestOutput::print_failure(const TestFailure& failure)
{
  if (output_one_)
    output_one_->print_failure(failure);
  if (output_two_)
    output_two_->print_failure(failure);
}

void CompositeTestOutput::set_progress_indicator(const char* indicator)
{
  if (output_one_)
    output_one_->set_progress_indicator(indicator);
  if (output_two_)
    output_two_->set_progress_indicator(indicator);
}

void CompositeTestOutput::print_very_verbose(const char* str)
{
  if (output_one_)
    output_one_->print_very_verbose(str);
  if (output_two_)
    output_two_->print_very_verbose(str);
}

void CompositeTestOutput::flush()
{
  if (output_one_)
    output_one_->flush();
  if (output_two_)
    output_two_->flush();
}

} // namespace cppmu
