#ifndef INCLUDED_CPPUTEST_JUNITTESTOUTPUT_HPP
#define INCLUDED_CPPUTEST_JUNITTESTOUTPUT_HPP

#include "CppUTest/String.hpp"
#include "CppUTest/TestOutput.hpp"

namespace cpputest {

struct JUnitTestOutputImpl;
struct JUnitTestCaseResultNode;

class JUnitTestOutput : public TestOutput
{
public:
  JUnitTestOutput();
  virtual ~JUnitTestOutput() override;

  virtual void print_tests_started() override;
  virtual void print_tests_ended(const TestResult& result) override;
  virtual void print_current_test_started(const TestShell& test) override;
  virtual void print_current_test_ended(const TestResult& res) override;
  virtual void print_current_group_started(const TestShell& test) override;
  virtual void print_current_group_ended(const TestResult& res) override;

  virtual void print_buffer(const char*) override;
  virtual void print(const char*) override;
  virtual void print(long) override;
  virtual void print(size_t) override;
  virtual void print_failure(const TestFailure& failure) override;

  virtual void flush() override;

  virtual String create_file_name(const String& group);
  void set_package_name(const String& package);

protected:
  JUnitTestOutputImpl* impl_;
  void reset_test_group_result();

  virtual void open_file_for_write(const String& file_name);
  virtual void write_test_group_to_file();
  virtual void write_to_file(const String& buffer);
  virtual void close_file();

  virtual void write_xml_header();
  virtual void write_test_suite_summary();
  virtual void write_properties();
  virtual void write_test_cases();
  virtual String encode_xml_text(const String& textbody);
  virtual String encode_file_name(const String& file_name);
  virtual void write_failure(JUnitTestCaseResultNode* node);
  virtual void write_file_ending();
};

} // namespace cpputest

#endif
