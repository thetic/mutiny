#ifndef INCLUDED_MU_TINY_TEST_JUNITOUTPUT_HPP
#define INCLUDED_MU_TINY_TEST_JUNITOUTPUT_HPP

#include "mu/tiny/test/Output.hpp"

#include "mu/tiny/String.hpp"

namespace mu {
namespace tiny {
namespace test {

class JUnitTestOutputImpl;
class JUnitTestCaseResultNode;

class JUnitOutput : public Output
{
public:
  JUnitOutput();
  ~JUnitOutput() override;

  void print_tests_started() override;
  void print_tests_ended(const Result& result) override;
  void print_current_test_started(const Shell& test) override;
  void print_current_test_ended(const Result& res) override;
  void print_current_group_started(const Shell& test) override;
  void print_current_group_ended(const Result& res) override;

  void print_buffer(const char* buffer) override;
  void print_failure(const Failure& failure) override;
  void print_test_property(const char* name, const char* value) override;
  void print_skipped(const char* message) override;

  bool needs_console_companion() const override { return true; }

  String create_file_name();
  void set_package_name(const String& package);

private:
  JUnitTestOutputImpl* impl_;
  void reset_test_group_result();
  void open_file_for_write();
  void write_test_group_to_file();
  void write_to_file(const String& buffer);
  void close_file();

  void write_test_suite_summary();
  void write_test_cases();
  static String encode_xml_text(const String& textbody);
  static String encode_file_name(const String& file_name);
  void write_failure(JUnitTestCaseResultNode* node);
  void write_error(JUnitTestCaseResultNode* node);
  void write_file_ending();
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
