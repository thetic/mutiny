#ifndef INCLUDED_CPPMU_COMPOSITETESTOUTPUT_HPP
#define INCLUDED_CPPMU_COMPOSITETESTOUTPUT_HPP

#include "CppMu/TestOutput.hpp"

namespace cppmu {

class CompositeTestOutput : public TestOutput
{
public:
  virtual void set_output_one(TestOutput* output);
  virtual void set_output_two(TestOutput* output);

  CompositeTestOutput();
  ~CompositeTestOutput() override;

  void print_tests_started() override;
  void print_tests_ended(const TestResult& result) override;

  void print_current_test_started(const TestShell& test) override;
  void print_current_test_ended(const TestResult& res) override;
  void print_current_group_started(const TestShell& test) override;
  void print_current_group_ended(const TestResult& res) override;

  void verbose(VerbosityLevel level) override;
  void color() override;
  void print_buffer(const char*) override;
  void print(const char*) override;
  void print(long) override;
  void print(size_t) override;
  void print_double(double) override;
  void print_failure(const TestFailure& failure) override;
  void set_progress_indicator(const char*) override;

  void print_very_verbose(const char*) override;

  void flush() override;

protected:
  CompositeTestOutput(const TestOutput&);
  CompositeTestOutput& operator=(const TestOutput&);

private:
  TestOutput* output_one_;
  TestOutput* output_two_;
};

} // namespace cppmu

#endif
