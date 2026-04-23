#include "mu/tiny/test/TapOutput.hpp"

#include "mu/tiny/test/Failure.hpp"
#include "mu/tiny/test/Output.hpp"
#include "mu/tiny/test/Result.hpp"
#include "mu/tiny/test/Shell.hpp"

namespace mu {
namespace tiny {
namespace test {

namespace {

String yaml_escape(const String& text)
{
  String result;
  for (const char* p = text.c_str(); *p != 0; ++p) {
    switch (*p) {
      case '\\':
        result += "\\\\";
        break;
      case '"':
        result += "\\\"";
        break;
      case '\n':
        result += "\\n";
        break;
      case '\r':
        result += "\\r";
        break;
      case '\t':
        result += "\\t";
        break;
      default:
        result += *p;
        break;
    }
  }
  return result;
}

} // namespace

class TapTestResultNode
{
public:
  TapTestResultNode() = default;

  String group;
  String name;
  bool skipped{ false };
  String skip_message;
  Failure* failure{ nullptr };
  bool failure_is_error{ false };
  TapTestResultNode* next{ nullptr };
};

class TapOutputImpl
{
public:
  TapOutputImpl() = default;

  size_t test_count{ 0 };
  TapTestResultNode* head{ nullptr };
  TapTestResultNode* tail{ nullptr };
};

TapOutput::TapOutput()
  : impl_(new TapOutputImpl)
{
}

TapOutput::~TapOutput()
{
  clear_nodes();
  delete impl_;
}

void TapOutput::clear_nodes()
{
  TapTestResultNode* cur = impl_->head;
  while (cur != nullptr) {
    TapTestResultNode* tmp = cur->next;
    delete cur->failure;
    delete cur;
    cur = tmp;
  }
  impl_->head = nullptr;
  impl_->tail = nullptr;
}

void TapOutput::print_tests_started()
{
  impl_->test_count = 0;
  clear_nodes();
}

void TapOutput::print_current_group_started(const Shell& /*test*/) {}
void TapOutput::print_current_group_ended(const Result& /*res*/) {}

void TapOutput::print_current_test_started(const Shell& test)
{
  impl_->test_count++;

  if (impl_->tail == nullptr) {
    impl_->head = impl_->tail = new TapTestResultNode;
  } else {
    impl_->tail->next = new TapTestResultNode;
    impl_->tail = impl_->tail->next;
  }
  impl_->tail->group = test.get_group();
  impl_->tail->name = test.get_name();

  if (!test.will_run()) {
    impl_->tail->skipped = true;
    impl_->tail->skip_message = test.get_macro_name();
  }
}

void TapOutput::print_current_test_ended(const Result& /*res*/) {}

void TapOutput::print_tests_ended(const Result& /*result*/)
{
  String header = string_from_format(
      "TAP version 13\n1..%d\n", static_cast<int>(impl_->test_count)
  );
  fputs_(header.c_str(), stdout_);

  size_t n = 0;
  for (TapTestResultNode* cur = impl_->head; cur != nullptr; cur = cur->next) {
    ++n;
    String test_id = cur->group;
    test_id += ".";
    test_id += cur->name;

    if (cur->failure != nullptr) {
      String line = string_from_format(
          "not ok %d - %s\n", static_cast<int>(n), test_id.c_str()
      );
      line += "  ---\n";
      line += string_from_format(
          "  message: \"%s\"\n",
          yaml_escape(cur->failure->get_message()).c_str()
      );
      line += cur->failure_is_error ? "  severity: comment\n"
                                    : "  severity: fail\n";
      line += "  at:\n";
      line += string_from_format(
          "    file: %s\n", cur->failure->get_file_name().c_str()
      );
      line += string_from_format(
          "    line: %d\n", cur->failure->get_failure_line_number()
      );
      line += "  ...\n";
      fputs_(line.c_str(), stdout_);
    } else if (cur->skipped) {
      String line = string_from_format(
          "ok %d - %s # SKIP %s\n",
          static_cast<int>(n),
          test_id.c_str(),
          cur->skip_message.c_str()
      );
      fputs_(line.c_str(), stdout_);
    } else {
      String line = string_from_format(
          "ok %d - %s\n", static_cast<int>(n), test_id.c_str()
      );
      fputs_(line.c_str(), stdout_);
    }
  }
}

void TapOutput::print_buffer(const char* /*buffer*/) {}

void TapOutput::print_failure(const Failure& failure)
{
  if (impl_->tail != nullptr && impl_->tail->failure == nullptr) {
    impl_->tail->failure_is_error = failure.is_error();
    impl_->tail->failure = new Failure(failure);
  }
}

void TapOutput::print_skipped(const char* message)
{
  if (impl_->tail != nullptr) {
    impl_->tail->skipped = true;
    impl_->tail->skip_message = message;
  }
}

} // namespace test
} // namespace tiny
} // namespace mu
