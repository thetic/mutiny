#ifndef INCLUDED_MUTINY_TEST_JUNITOUTPUT_HPP
#define INCLUDED_MUTINY_TEST_JUNITOUTPUT_HPP

#include "mutiny/test/Output.hpp"

#include "mutiny/String.hpp"

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

  void print_buffer(const char*) override;
  void print_failure(const Failure& failure) override;
  void print_test_property(const char* name, const char* value) override;
  void print_skipped(const char* message) override;

  bool needs_console_companion() const override { return true; }

  virtual String create_file_name();
  void set_package_name(const String& package);

protected:
  JUnitTestOutputImpl* impl_;
  void reset_test_group_result();

  virtual void open_file_for_write(const String& file_name);
  virtual void write_test_group_to_file();
  virtual void write_to_file(const String& buffer);
  virtual void close_file();

  virtual void write_test_suite_summary();
  virtual void write_test_cases();
  virtual String encode_xml_text(const String& textbody);
  virtual String encode_file_name(const String& file_name);
  virtual void write_failure(JUnitTestCaseResultNode* node);
  virtual void write_error(JUnitTestCaseResultNode* node);
  virtual void write_file_ending();
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
