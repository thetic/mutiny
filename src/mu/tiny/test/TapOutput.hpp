#ifndef INCLUDED_MU_TINY_TEST_TAPOUTPUT_HPP
#define INCLUDED_MU_TINY_TEST_TAPOUTPUT_HPP

#include "mu/tiny/test/Output.hpp"

namespace mu {
namespace tiny {
namespace test {

class TapOutputImpl;

class TapOutput : public Output
{
public:
  TapOutput();
  ~TapOutput() override;

  void print_tests_started() override;
  void print_tests_ended(const Result& result) override;
  void print_current_test_started(const Shell& test) override;
  void print_current_test_ended(const Result& res) override;
  void print_current_group_started(const Shell& test) override;
  void print_current_group_ended(const Result& res) override;

  void print_buffer(const char*) override;
  void print_failure(const Failure& failure) override;
  void print_skipped(const char* message) override;

  bool needs_console_companion() const override { return false; }

private:
  void clear_nodes();

  TapOutputImpl* impl_;
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
