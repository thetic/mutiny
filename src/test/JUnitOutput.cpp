#include "mu/tiny/test/JUnitOutput.hpp"

#include "mu/tiny/test/Failure.hpp"
#include "mu/tiny/test/Output.hpp"
#include "mu/tiny/test/Result.hpp"
#include "mu/tiny/test/Shell.hpp"

#include "mu/tiny/time.hpp"

#include <stdint.h>

namespace mu {
namespace tiny {
namespace test {

namespace {
constexpr uint_least64_t ms_per_s{ 1000 };

class TestProperty
{
public:
  String name;
  String value;
  TestProperty* next{ nullptr };
};

} // namespace

class JUnitTestCaseResultNode
{
public:
  JUnitTestCaseResultNode() = default;

  String name;
  uint_least64_t exec_time{ 0 };
  Failure* failure{ nullptr };
  bool failure_is_error{ false };
  bool skipped{ false };
  String skip_message;
  String file;
  int line_number{ 0 };
  size_t check_count{ 0 };
  TestProperty* properties{ nullptr };
  TestProperty* properties_tail{ nullptr };
  JUnitTestCaseResultNode* next{ nullptr };
};

class JUnitTestGroupResult
{
public:
  JUnitTestGroupResult() = default;

  size_t test_count{ 0 };
  size_t failure_count{ 0 };
  size_t error_count{ 0 };
  size_t skip_count{ 0 };
  size_t total_check_count{ 0 };
  uint_least64_t start_time{ 0 };
  uint_least64_t group_exec_time{ 0 };
  String group;
  JUnitTestCaseResultNode* head{ nullptr };
  JUnitTestCaseResultNode* tail{ nullptr };
};

class JUnitTestOutputImpl
{
public:
  JUnitTestGroupResult results;
  String current_group_xml;
  String accumulated_xml;
  String package;
  size_t total_test_count{ 0 };
  size_t total_failure_count{ 0 };
  size_t total_error_count{ 0 };
  size_t total_skip_count{ 0 };
  uint_least64_t total_exec_time{ 0 };
  String start_timestamp;
};

JUnitOutput::JUnitOutput()
  : impl_(new JUnitTestOutputImpl)
{
}

JUnitOutput::~JUnitOutput()
{
  reset_test_group_result();
  delete impl_;
}

void JUnitOutput::reset_test_group_result()
{
  impl_->results.test_count = 0;
  impl_->results.failure_count = 0;
  impl_->results.error_count = 0;
  impl_->results.skip_count = 0;
  impl_->results.group = "";
  JUnitTestCaseResultNode* cur = impl_->results.head;
  while (cur != nullptr) {
    JUnitTestCaseResultNode* tmp = cur->next;
    delete cur->failure;
    TestProperty* prop = cur->properties;
    while (prop != nullptr) {
      TestProperty* prop_tmp = prop->next;
      delete prop;
      prop = prop_tmp;
    }
    delete cur;
    cur = tmp;
  }
  impl_->results.head = nullptr;
  impl_->results.tail = nullptr;
}

void JUnitOutput::print_tests_started()
{
  impl_->accumulated_xml.clear();
  impl_->total_test_count = 0;
  impl_->total_failure_count = 0;
  impl_->total_error_count = 0;
  impl_->total_skip_count = 0;
  impl_->total_exec_time = 0;
  impl_->start_timestamp = get_time_string();
}

void JUnitOutput::print_current_group_started(const Shell& /*test*/) {}

void JUnitOutput::print_current_test_ended(const Result& result)
{
  impl_->results.tail->exec_time =
      result.get_current_test_total_execution_time();
  impl_->results.tail->check_count = result.get_check_count();
}

void JUnitOutput::print_tests_ended(const Result& /*result*/)
{
  Output::File file = fopen_(create_file_name().c_str(), "w");
  String header = string_from_format(
      "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
      "<testsuites tests=\"%d\" failures=\"%d\" errors=\"%d\" "
      "skipped=\"%d\" time=\"%d.%03d\" timestamp=\"%s\">\n",
      static_cast<int>(impl_->total_test_count),
      static_cast<int>(impl_->total_failure_count),
      static_cast<int>(impl_->total_error_count),
      static_cast<int>(impl_->total_skip_count),
      static_cast<int>(impl_->total_exec_time / ms_per_s),
      static_cast<int>(impl_->total_exec_time % ms_per_s),
      impl_->start_timestamp.c_str()
  );
  fputs_(header.c_str(), file);
  fputs_(impl_->accumulated_xml.c_str(), file);
  fputs_("</testsuites>\n", file);
  fclose_(file);
}

void JUnitOutput::print_current_group_ended(const Result& result)
{
  if (impl_->results.test_count == 0) {
    reset_test_group_result();
    return;
  }
  impl_->results.group_exec_time =
      result.get_current_group_total_execution_time();
  impl_->total_test_count += impl_->results.test_count;
  impl_->total_failure_count += impl_->results.failure_count;
  impl_->total_error_count += impl_->results.error_count;
  impl_->total_skip_count += impl_->results.skip_count;
  impl_->total_exec_time += impl_->results.group_exec_time;
  write_test_group_to_file();
  reset_test_group_result();
}

void JUnitOutput::print_current_test_started(const Shell& test)
{
  impl_->results.test_count++;
  impl_->results.group = test.get_group();
  impl_->results.start_time = get_time_in_millis();

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
    impl_->results.tail->skipped = true;
    impl_->results.tail->skip_message = test.get_macro_name();
    impl_->results.skip_count++;
  }
}

String JUnitOutput::create_file_name()
{
  if (!impl_->package.empty()) {
    return encode_file_name(impl_->package) + ".xml";
  }
  return "mutiny.xml";
}

String JUnitOutput::encode_file_name(const String& file_name)
{
  // special character list based on: https://en.wikipedia.org/wiki/Filename
  static const char* const forbidden_characters = "/\\?%*:|\"<>";

  String result = file_name;
  for (const char* sym = forbidden_characters; *sym != 0; ++sym) {
    string_replace(result, *sym, '_');
  }
  return result;
}

void JUnitOutput::set_package_name(const String& package)
{
  if (impl_ != nullptr) {
    impl_->package = package;
  }
}

void JUnitOutput::write_test_suite_summary()
{
  size_t total_assertions = 0;
  for (JUnitTestCaseResultNode* n = impl_->results.head; n != nullptr;
       n = n->next) {
    total_assertions = n->check_count;
  }

  String buf = string_from_format(
      "<testsuite errors=\"%d\" failures=\"%d\" skipped=\"%d\" "
      "assertions=\"%d\" name=\"%s\" tests=\"%d\" "
      "time=\"%d.%03d\" timestamp=\"%s\">\n",
      static_cast<int>(impl_->results.error_count),
      static_cast<int>(impl_->results.failure_count),
      static_cast<int>(impl_->results.skip_count),
      static_cast<int>(total_assertions),
      impl_->results.group.c_str(),
      static_cast<int>(impl_->results.test_count),
      static_cast<int>(impl_->results.group_exec_time / ms_per_s),
      static_cast<int>(impl_->results.group_exec_time % ms_per_s),
      get_time_string()
  );
  write_to_file(buf.c_str());
}

String JUnitOutput::encode_xml_text(const String& textbody)
{
  String buf = textbody.c_str();
  string_replace(buf, "&", "&amp;");
  string_replace(buf, "\"", "&quot;");
  string_replace(buf, "<", "&lt;");
  string_replace(buf, ">", "&gt;");
  string_replace(buf, "\r", "&#13;");
  string_replace(buf, "\n", "&#10;");
  return buf;
}

void JUnitOutput::write_test_cases()
{
  JUnitTestCaseResultNode* cur = impl_->results.head;

  while (cur != nullptr) {
    String buf = string_from_format(
        "<testcase classname=\"%s%s%s\" name=\"%s\" assertions=\"%d\" "
        "time=\"%d.%03d\" file=\"%s\" line=\"%d\">\n",
        impl_->package.c_str(),
        impl_->package.empty() ? "" : ".",
        impl_->results.group.c_str(),
        cur->name.c_str(),
        static_cast<int>(cur->check_count - impl_->results.total_check_count),
        static_cast<int>(cur->exec_time / ms_per_s),
        static_cast<int>(cur->exec_time % ms_per_s),
        cur->file.c_str(),
        cur->line_number
    );
    write_to_file(buf.c_str());

    impl_->results.total_check_count = cur->check_count;

    if (cur->properties != nullptr) {
      write_to_file("<properties>\n");
      for (TestProperty* prop = cur->properties; prop != nullptr;
           prop = prop->next) {
        String prop_buf = string_from_format(
            "<property name=\"%s\" value=\"%s\"/>\n",
            encode_xml_text(prop->name).c_str(),
            encode_xml_text(prop->value).c_str()
        );
        write_to_file(prop_buf.c_str());
      }
      write_to_file("</properties>\n");
    }

    if (cur->failure != nullptr) {
      if (cur->failure_is_error) {
        write_error(cur);
      } else {
        write_failure(cur);
      }
    } else if (cur->skipped) {
      if (cur->skip_message.empty()) {
        write_to_file("<skipped />\n");
      } else {
        write_to_file(string_from_format(
                          "<skipped message=\"%s\" />\n",
                          encode_xml_text(cur->skip_message).c_str()
        )
                          .c_str());
      }
    }

    write_to_file("</testcase>\n");
    cur = cur->next;
  }
}

void JUnitOutput::write_failure(JUnitTestCaseResultNode* node)
{
  String file = encode_xml_text(node->failure->get_file_name());
  String msg = encode_xml_text(node->failure->get_message());
  String buf = string_from_format(
      "<failure message=\"%s:%d: %s\" type=\"AssertionFailedError\">\n"
      "%s:%d: %s\n",
      file.c_str(),
      node->failure->get_failure_line_number(),
      msg.c_str(),
      file.c_str(),
      node->failure->get_failure_line_number(),
      msg.c_str()
  );
  write_to_file(buf.c_str());
  write_to_file("</failure>\n");
}

void JUnitOutput::write_error(JUnitTestCaseResultNode* node)
{
  String msg = encode_xml_text(node->failure->get_message());
  String buf = string_from_format(
      "<error message=\"%s\" type=\"UnexpectedException\">\n"
      "%s\n",
      msg.c_str(),
      msg.c_str()
  );
  write_to_file(buf.c_str());
  write_to_file("</error>\n");
}

void JUnitOutput::write_file_ending()
{
  write_to_file("</testsuite>\n");
}

void JUnitOutput::write_test_group_to_file()
{
  open_file_for_write();
  write_test_suite_summary();
  write_test_cases();
  write_file_ending();
  close_file();
}

void JUnitOutput::print_buffer(const char* /*buffer*/) {}

void JUnitOutput::print_test_property(const char* name, const char* value)
{
  if (impl_->results.tail == nullptr) {
    return;
  }
  auto* prop = new TestProperty;
  prop->name = name;
  prop->value = value;
  if (impl_->results.tail->properties == nullptr) {
    impl_->results.tail->properties = prop;
    impl_->results.tail->properties_tail = prop;
  } else {
    impl_->results.tail->properties_tail->next = prop;
    impl_->results.tail->properties_tail = prop;
  }
}

void JUnitOutput::print_skipped(const char* message)
{
  if (impl_->results.tail == nullptr) {
    return;
  }
  impl_->results.tail->skipped = true;
  impl_->results.tail->skip_message = message;
  impl_->results.skip_count++;
}

void JUnitOutput::print_failure(const Failure& failure)
{
  if (impl_->results.tail->failure == nullptr) {
    if (failure.is_error()) {
      impl_->results.error_count++;
      impl_->results.tail->failure_is_error = true;
    } else {
      impl_->results.failure_count++;
    }
    impl_->results.tail->failure = new Failure(failure);
  }
}

void JUnitOutput::open_file_for_write()
{
  impl_->current_group_xml.clear();
}

void JUnitOutput::write_to_file(const String& buffer)
{
  impl_->current_group_xml += buffer;
}

void JUnitOutput::close_file()
{
  impl_->accumulated_xml += impl_->current_group_xml;
}

} // namespace test
} // namespace tiny
} // namespace mu
