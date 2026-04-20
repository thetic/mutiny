#include "mu/tiny/test/CompositeOutput.hpp"

namespace mu {
namespace tiny {
namespace test {

CompositeOutput::~CompositeOutput()
{
  delete output_one_;
  delete output_two_;
}

void CompositeOutput::set_output_one(Output* output)
{
  delete output_one_;
  output_one_ = output;
}

void CompositeOutput::set_output_two(Output* output)
{
  delete output_two_;
  output_two_ = output;
}

void CompositeOutput::print_tests_started()
{
  if (output_one_ != nullptr) {
    output_one_->print_tests_started();
  }
  if (output_two_ != nullptr) {
    output_two_->print_tests_started();
  }
}

void CompositeOutput::print_tests_ended(const Result& result)
{
  if (output_one_ != nullptr) {
    output_one_->print_tests_ended(result);
  }
  if (output_two_ != nullptr) {
    output_two_->print_tests_ended(result);
  }
}

void CompositeOutput::print_current_test_started(const Shell& test)
{
  if (output_one_ != nullptr) {
    output_one_->print_current_test_started(test);
  }
  if (output_two_ != nullptr) {
    output_two_->print_current_test_started(test);
  }
}

void CompositeOutput::print_current_test_ended(const Result& res)
{
  if (output_one_ != nullptr) {
    output_one_->print_current_test_ended(res);
  }
  if (output_two_ != nullptr) {
    output_two_->print_current_test_ended(res);
  }
}

void CompositeOutput::print_current_group_started(const Shell& test)
{
  if (output_one_ != nullptr) {
    output_one_->print_current_group_started(test);
  }
  if (output_two_ != nullptr) {
    output_two_->print_current_group_started(test);
  }
}

void CompositeOutput::print_current_group_ended(const Result& res)
{
  if (output_one_ != nullptr) {
    output_one_->print_current_group_ended(res);
  }
  if (output_two_ != nullptr) {
    output_two_->print_current_group_ended(res);
  }
}

void CompositeOutput::verbose(MutinyVerbosityLevel level)
{
  if (output_one_ != nullptr) {
    output_one_->verbose(level);
  }
  if (output_two_ != nullptr) {
    output_two_->verbose(level);
  }
}

void CompositeOutput::color()
{
  if (output_one_ != nullptr) {
    output_one_->color();
  }
  if (output_two_ != nullptr) {
    output_two_->color();
  }
}

void CompositeOutput::print_buffer(const char* buffer)
{
  if (output_one_ != nullptr) {
    output_one_->print_buffer(buffer);
  }
  if (output_two_ != nullptr) {
    output_two_->print_buffer(buffer);
  }
}

void CompositeOutput::print(const char* buffer)
{
  if (output_one_ != nullptr) {
    output_one_->print(buffer);
  }
  if (output_two_ != nullptr) {
    output_two_->print(buffer);
  }
}

void CompositeOutput::print(long number)
{
  if (output_one_ != nullptr) {
    output_one_->print(number);
  }
  if (output_two_ != nullptr) {
    output_two_->print(number);
  }
}

void CompositeOutput::print(size_t number)
{
  if (output_one_ != nullptr) {
    output_one_->print(number);
  }
  if (output_two_ != nullptr) {
    output_two_->print(number);
  }
}

void CompositeOutput::print_double(double number)
{
  if (output_one_ != nullptr) {
    output_one_->print_double(number);
  }
  if (output_two_ != nullptr) {
    output_two_->print_double(number);
  }
}

void CompositeOutput::print_failure(const Failure& failure)
{
  if (output_one_ != nullptr) {
    output_one_->print_failure(failure);
  }
  if (output_two_ != nullptr) {
    output_two_->print_failure(failure);
  }
}

void CompositeOutput::set_progress_indicator(const char* indicator)
{
  if (output_one_ != nullptr) {
    output_one_->set_progress_indicator(indicator);
  }
  if (output_two_ != nullptr) {
    output_two_->set_progress_indicator(indicator);
  }
}

void CompositeOutput::print_very_verbose(const char* str)
{
  if (output_one_ != nullptr) {
    output_one_->print_very_verbose(str);
  }
  if (output_two_ != nullptr) {
    output_two_->print_very_verbose(str);
  }
}

void CompositeOutput::flush()
{
  if (output_one_ != nullptr) {
    output_one_->flush();
  }
  if (output_two_ != nullptr) {
    output_two_->flush();
  }
}

} // namespace test
} // namespace tiny
} // namespace mu
