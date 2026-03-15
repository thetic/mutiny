#ifndef INCLUDED_CPPUTEST_COMPOSITETESTOUTPUT_HPP
#define INCLUDED_CPPUTEST_COMPOSITETESTOUTPUT_HPP

#include "CppUTest/TestOutput.hpp"

namespace cpputest {

class CompositeTestOutput : public TestOutput
{
public:
  virtual void set_output_one(TestOutput* output);
  virtual void set_output_two(TestOutput* output);

  CompositeTestOutput();
  virtual ~CompositeTestOutput() override;

  virtual void print_tests_started() override;
  virtual void print_tests_ended(const TestResult& result) override;

  virtual void print_current_test_started(const TestShell& test) override;
  virtual void print_current_test_ended(const TestResult& res) override;
  virtual void print_current_group_started(const TestShell& test) override;
  virtual void print_current_group_ended(const TestResult& res) override;

  virtual void verbose(VerbosityLevel level) override;
  virtual void color() override;
  virtual void print_buffer(const char*) override;
  virtual void print(const char*) override;
  virtual void print(long) override;
  virtual void print(size_t) override;
  virtual void print_double(double) override;
  virtual void print_failure(const TestFailure& failure) override;
  virtual void set_progress_indicator(const char*) override;

  virtual void print_very_verbose(const char*) override;

  virtual void flush() override;

protected:
  CompositeTestOutput(const TestOutput&);
  CompositeTestOutput& operator=(const TestOutput&);

private:
  TestOutput* output_one_;
  TestOutput* output_two_;
};

} // namespace cpputest

#endif
