#ifndef INCLUDED_MU_TINY_TEST_COMPOSITEOUTPUT_HPP
#define INCLUDED_MU_TINY_TEST_COMPOSITEOUTPUT_HPP

#include "mu/tiny/test/Output.hpp"

namespace mu {
namespace tiny {
namespace test {

class CompositeOutput : public Output
{
public:
  virtual void set_output_one(Output* output);
  virtual void set_output_two(Output* output);

  CompositeOutput() = default;
  ~CompositeOutput() override;

  void print_tests_started() override;
  void print_tests_ended(const Result& result) override;

  void print_current_test_started(const Shell& test) override;
  void print_current_test_ended(const Result& res) override;
  void print_current_group_started(const Shell& test) override;
  void print_current_group_ended(const Result& res) override;

  void verbose(MutinyVerbosityLevel level) override;
  void color() override;
  void print_buffer(const char*) override;
  void print(const char*) override;
  void print(long) override;
  void print(size_t) override;
  void print_double(double) override;
  void print_failure(const Failure& failure) override;
  void set_progress_indicator(const char*) override;

  void print_very_verbose(const char*) override;

  void flush() override;

  CompositeOutput(const CompositeOutput&) = delete;
  CompositeOutput& operator=(const CompositeOutput&) = delete;

private:
  Output* output_one_{ nullptr };
  Output* output_two_{ nullptr };
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
