#include "CppMu/JUnitTestOutput.hpp"

#include "CppMu/CppMu.hpp"
#include "CppMu/TestFailure.hpp"
#include "CppMu/TestOutput.hpp"
#include "CppMu/TestResult.hpp"
#include "CppMu/time.hpp"

namespace cppmu {

struct JUnitTestCaseResultNode
{
  JUnitTestCaseResultNode() = default;

  String name;
  size_t exec_time{ 0 };
  TestFailure* failure{ nullptr };
  bool ignored{ false };
  String file;
  size_t line_number{ 0 };
  size_t check_count{ 0 };
  JUnitTestCaseResultNode* next{ nullptr };
};

struct JUnitTestGroupResult
{
  JUnitTestGroupResult() = default;

  size_t test_count{ 0 };
  size_t failure_count{ 0 };
  size_t total_check_count{ 0 };
  size_t start_time{ 0 };
  size_t group_exec_time{ 0 };
  String group;
  JUnitTestCaseResultNode* head{ nullptr };
  JUnitTestCaseResultNode* tail{ nullptr };
};

struct JUnitTestOutputImpl
{
  JUnitTestGroupResult results;
  cppmu::File file;
  String package;
  String std_output;
};

JUnitTestOutput::JUnitTestOutput()
  : impl_(new JUnitTestOutputImpl)
{
}

JUnitTestOutput::~JUnitTestOutput()
{
  reset_test_group_result();
  delete impl_;
}

void
JUnitTestOutput::reset_test_group_result()
{
  impl_->results.test_count = 0;
  impl_->results.failure_count = 0;
  impl_->results.group = "";
  JUnitTestCaseResultNode* cur = impl_->results.head;
  while (cur) {
    JUnitTestCaseResultNode* tmp = cur->next;
    delete cur->failure;
    delete cur;
    cur = tmp;
  }
  impl_->results.head = nullptr;
  impl_->results.tail = nullptr;
}

void
JUnitTestOutput::print_tests_started()
{
}

void
JUnitTestOutput::print_current_group_started(const TestShell& /*test*/)
{
}

void
JUnitTestOutput::print_current_test_ended(const TestResult& result)
{
  impl_->results.tail->exec_time =
      result.get_current_test_total_execution_time();
  impl_->results.tail->check_count = result.get_check_count();
}

void
JUnitTestOutput::print_tests_ended(const TestResult& /*result*/)
{
}

void
JUnitTestOutput::print_current_group_ended(const TestResult& result)
{
  impl_->results.group_exec_time =
      result.get_current_group_total_execution_time();
  write_test_group_to_file();
  reset_test_group_result();
}

void
JUnitTestOutput::print_current_test_started(const TestShell& test)
{
  impl_->results.test_count++;
  impl_->results.group = test.get_group();
  impl_->results.start_time = static_cast<size_t>(cppmu::get_time_in_millis());

  if (impl_->results.tail == nullptr) {
    impl_->results.head = impl_->results.tail = new JUnitTestCaseResultNode;
  } else {
    impl_->results.tail->next = new JUnitTestCaseResultNode;
    impl_->results.tail = impl_->results.tail->next;
  }
  impl_->results.tail->name = test.get_name();
  impl_->results.tail->file = test.get_file();
  impl_->results.tail->line_number = test.get_line_number();
  if (!test.will_run()) {
    impl_->results.tail->ignored = true;
  }
}

String
JUnitTestOutput::create_file_name(const String& group)
{
  String file_name = "cppmu_";
  if (!impl_->package.empty()) {
    file_name += impl_->package;
    file_name += "_";
  }
  file_name += group;
  return encode_file_name(file_name) + ".xml";
}

String
JUnitTestOutput::encode_file_name(const String& file_name)
{
  // special character list based on: https://en.wikipedia.org/wiki/Filename
  static const char* const forbidden_characters = "/\\?%*:|\"<>";

  String result = file_name;
  for (const char* sym = forbidden_characters; *sym; ++sym) {
    result.replace(*sym, '_');
  }
  return result;
}

void
JUnitTestOutput::set_package_name(const String& package)
{
  if (impl_ != nullptr) {
    impl_->package = package;
  }
}

void
JUnitTestOutput::write_xml_header()
{
  write_to_file("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
}

void
JUnitTestOutput::write_test_suite_summary()
{
  String buf = string_from_format(
      "<testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" "
      "name=\"%s\" tests=\"%d\" time=\"%d.%03d\" timestamp=\"%s\">\n",
      static_cast<int>(impl_->results.failure_count),
      impl_->results.group.c_str(),
      static_cast<int>(impl_->results.test_count),
      static_cast<int>(impl_->results.group_exec_time / 1000),
      static_cast<int>(impl_->results.group_exec_time % 1000),
      cppmu::get_time_string());
  write_to_file(buf.c_str());
}

void
JUnitTestOutput::write_properties()
{
  write_to_file("<properties>\n");
  write_to_file("</properties>\n");
}

String
JUnitTestOutput::encode_xml_text(const String& textbody)
{
  String buf = textbody.c_str();
  buf.replace("&", "&amp;");
  buf.replace("\"", "&quot;");
  buf.replace("<", "&lt;");
  buf.replace(">", "&gt;");
  buf.replace("\r", "&#13;");
  buf.replace("\n", "&#10;");
  return buf;
}

void
JUnitTestOutput::write_test_cases()
{
  JUnitTestCaseResultNode* cur = impl_->results.head;

  while (cur) {
    String buf = string_from_format(
        "<testcase classname=\"%s%s%s\" name=\"%s\" assertions=\"%d\" "
        "time=\"%d.%03d\" file=\"%s\" line=\"%d\">\n",
        impl_->package.c_str(),
        impl_->package.empty() ? "" : ".",
        impl_->results.group.c_str(),
        cur->name.c_str(),
        static_cast<int>(cur->check_count - impl_->results.total_check_count),
        static_cast<int>(cur->exec_time / 1000),
        static_cast<int>(cur->exec_time % 1000),
        cur->file.c_str(),
        static_cast<int>(cur->line_number));
    write_to_file(buf.c_str());

    impl_->results.total_check_count = cur->check_count;

    if (cur->failure) {
      write_failure(cur);
    } else if (cur->ignored) {
      write_to_file("<skipped />\n");
    }
    write_to_file("</testcase>\n");
    cur = cur->next;
  }
}

void
JUnitTestOutput::write_failure(JUnitTestCaseResultNode* node)
{
  String buf = string_from_format(
      "<failure message=\"%s:%d: %s\" type=\"AssertionFailedError\">\n",
      node->failure->get_file_name().c_str(),
      static_cast<int>(node->failure->get_failure_line_number()),
      encode_xml_text(node->failure->get_message()).c_str());
  write_to_file(buf.c_str());
  write_to_file("</failure>\n");
}

void
JUnitTestOutput::write_file_ending()
{
  write_to_file("<system-out>");
  write_to_file(encode_xml_text(impl_->std_output));
  write_to_file("</system-out>\n");
  write_to_file("<system-err></system-err>\n");
  write_to_file("</testsuite>\n");
}

void
JUnitTestOutput::write_test_group_to_file()
{
  open_file_for_write(create_file_name(impl_->results.group));
  write_xml_header();
  write_test_suite_summary();
  write_properties();
  write_test_cases();
  write_file_ending();
  close_file();
}

void
JUnitTestOutput::print_buffer(const char*)
{
}

void
JUnitTestOutput::print(const char* output)
{
  impl_->std_output += output;
}

void
JUnitTestOutput::print(long)
{
}

void
JUnitTestOutput::print(size_t)
{
}

void
JUnitTestOutput::flush()
{
}

void
JUnitTestOutput::print_failure(const TestFailure& failure)
{
  if (impl_->results.tail->failure == nullptr) {
    impl_->results.failure_count++;
    impl_->results.tail->failure = new TestFailure(failure);
  }
}

void
JUnitTestOutput::open_file_for_write(const String& file_name)
{
  impl_->file = cppmu::f_open(file_name.c_str(), "w");
}

void
JUnitTestOutput::write_to_file(const String& buffer)
{
  cppmu::f_puts(buffer.c_str(), impl_->file);
}

void
JUnitTestOutput::close_file()
{
  cppmu::f_close(impl_->file);
}

} // namespace cppmu
