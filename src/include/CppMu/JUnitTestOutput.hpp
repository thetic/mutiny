#ifndef INCLUDED_CPPMU_JUNITTESTOUTPUT_HPP
#define INCLUDED_CPPMU_JUNITTESTOUTPUT_HPP

#include "CppMu/String.hpp"
#include "CppMu/TestOutput.hpp"

namespace cppmu {

struct JUnitTestOutputImpl;
struct JUnitTestCaseResultNode;

class JUnitTestOutput : public TestOutput
{
public:
  JUnitTestOutput();
  ~JUnitTestOutput() override;

  void print_tests_started() override;
  void print_tests_ended(const TestResult& result) override;
  void print_current_test_started(const TestShell& test) override;
  void print_current_test_ended(const TestResult& res) override;
  void print_current_group_started(const TestShell& test) override;
  void print_current_group_ended(const TestResult& res) override;

  void print_buffer(const char*) override;
  void print(const char*) override;
  void print(long) override;
  void print(size_t) override;
  void print_failure(const TestFailure& failure) override;
  void print_test_property(const char* name, const char* value) override;

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
  virtual void write_error(JUnitTestCaseResultNode* node);
  virtual void write_file_ending();
};

} // namespace cppmu

#endif
