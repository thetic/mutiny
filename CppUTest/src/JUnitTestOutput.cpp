#include "CppUTest/JUnitTestOutput.hpp"

#include "CppUTest/PlatformSpecificFunctions.h"
#include "CppUTest/TestFailure.hpp"
#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestResult.hpp"

namespace cpputest {

struct JUnitTestCaseResultNode
{
  JUnitTestCaseResultNode()
    : execTime_(0)
    , failure_(nullptr)
    , ignored_(false)
    , lineNumber_(0)
    , checkCount_(0)
    , next_(nullptr)
  {
  }

  String name_;
  size_t execTime_;
  TestFailure* failure_;
  bool ignored_;
  String file_;
  size_t lineNumber_;
  size_t checkCount_;
  JUnitTestCaseResultNode* next_;
};

struct JUnitTestGroupResult
{
  JUnitTestGroupResult()
    : testCount_(0)
    , failureCount_(0)
    , totalCheckCount_(0)
    , startTime_(0)
    , groupExecTime_(0)
    , head_(nullptr)
    , tail_(nullptr)
  {
  }

  size_t testCount_;
  size_t failureCount_;
  size_t totalCheckCount_;
  size_t startTime_;
  size_t groupExecTime_;
  String group_;
  JUnitTestCaseResultNode* head_;
  JUnitTestCaseResultNode* tail_;
};

struct JUnitTestOutputImpl
{
  JUnitTestGroupResult results_;
  PlatformSpecificFile file_;
  String package_;
  String stdOutput_;
};

JUnitTestOutput::JUnitTestOutput()
  : impl_(new JUnitTestOutputImpl)
{
}

JUnitTestOutput::~JUnitTestOutput()
{
  resetTestGroupResult();
  delete impl_;
}

void
JUnitTestOutput::resetTestGroupResult()
{
  impl_->results_.testCount_ = 0;
  impl_->results_.failureCount_ = 0;
  impl_->results_.group_ = "";
  JUnitTestCaseResultNode* cur = impl_->results_.head_;
  while (cur) {
    JUnitTestCaseResultNode* tmp = cur->next_;
    delete cur->failure_;
    delete cur;
    cur = tmp;
  }
  impl_->results_.head_ = nullptr;
  impl_->results_.tail_ = nullptr;
}

void
JUnitTestOutput::printTestsStarted()
{
}

void
JUnitTestOutput::printCurrentGroupStarted(const TestShell& /*test*/)
{
}

void
JUnitTestOutput::printCurrentTestEnded(const TestResult& result)
{
  impl_->results_.tail_->execTime_ = result.getCurrentTestTotalExecutionTime();
  impl_->results_.tail_->checkCount_ = result.getCheckCount();
}

void
JUnitTestOutput::printTestsEnded(const TestResult& /*result*/)
{
}

void
JUnitTestOutput::printCurrentGroupEnded(const TestResult& result)
{
  impl_->results_.groupExecTime_ = result.getCurrentGroupTotalExecutionTime();
  writeTestGroupToFile();
  resetTestGroupResult();
}

void
JUnitTestOutput::printCurrentTestStarted(const TestShell& test)
{
  impl_->results_.testCount_++;
  impl_->results_.group_ = test.getGroup();
  impl_->results_.startTime_ =
      static_cast<size_t>(GetPlatformSpecificTimeInMillis());

  if (impl_->results_.tail_ == nullptr) {
    impl_->results_.head_ = impl_->results_.tail_ = new JUnitTestCaseResultNode;
  } else {
    impl_->results_.tail_->next_ = new JUnitTestCaseResultNode;
    impl_->results_.tail_ = impl_->results_.tail_->next_;
  }
  impl_->results_.tail_->name_ = test.getName();
  impl_->results_.tail_->file_ = test.getFile();
  impl_->results_.tail_->lineNumber_ = test.getLineNumber();
  if (!test.willRun()) {
    impl_->results_.tail_->ignored_ = true;
  }
}

String
JUnitTestOutput::createFileName(const String& group)
{
  String fileName = "cpputest_";
  if (!impl_->package_.empty()) {
    fileName += impl_->package_;
    fileName += "_";
  }
  fileName += group;
  return encodeFileName(fileName) + ".xml";
}

String
JUnitTestOutput::encodeFileName(const String& fileName)
{
  // special character list based on: https://en.wikipedia.org/wiki/Filename
  static const char* const forbiddenCharacters = "/\\?%*:|\"<>";

  String result = fileName;
  for (const char* sym = forbiddenCharacters; *sym; ++sym) {
    result.replace(*sym, '_');
  }
  return result;
}

void
JUnitTestOutput::setPackageName(const String& package)
{
  if (impl_ != nullptr) {
    impl_->package_ = package;
  }
}

void
JUnitTestOutput::writeXmlHeader()
{
  writeToFile("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
}

void
JUnitTestOutput::writeTestSuiteSummary()
{
  String buf = StringFromFormat(
      "<testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" "
      "name=\"%s\" tests=\"%d\" time=\"%d.%03d\" timestamp=\"%s\">\n",
      static_cast<int>(impl_->results_.failureCount_),
      impl_->results_.group_.c_str(),
      static_cast<int>(impl_->results_.testCount_),
      static_cast<int>(impl_->results_.groupExecTime_ / 1000),
      static_cast<int>(impl_->results_.groupExecTime_ % 1000),
      GetPlatformSpecificTimeString());
  writeToFile(buf.c_str());
}

void
JUnitTestOutput::writeProperties()
{
  writeToFile("<properties>\n");
  writeToFile("</properties>\n");
}

String
JUnitTestOutput::encodeXmlText(const String& textbody)
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
JUnitTestOutput::writeTestCases()
{
  JUnitTestCaseResultNode* cur = impl_->results_.head_;

  while (cur) {
    String buf = StringFromFormat(
        "<testcase classname=\"%s%s%s\" name=\"%s\" assertions=\"%d\" "
        "time=\"%d.%03d\" file=\"%s\" line=\"%d\">\n",
        impl_->package_.c_str(),
        impl_->package_.empty() ? "" : ".",
        impl_->results_.group_.c_str(),
        cur->name_.c_str(),
        static_cast<int>(cur->checkCount_ - impl_->results_.totalCheckCount_),
        static_cast<int>(cur->execTime_ / 1000),
        static_cast<int>(cur->execTime_ % 1000),
        cur->file_.c_str(),
        static_cast<int>(cur->lineNumber_));
    writeToFile(buf.c_str());

    impl_->results_.totalCheckCount_ = cur->checkCount_;

    if (cur->failure_) {
      writeFailure(cur);
    } else if (cur->ignored_) {
      writeToFile("<skipped />\n");
    }
    writeToFile("</testcase>\n");
    cur = cur->next_;
  }
}

void
JUnitTestOutput::writeFailure(JUnitTestCaseResultNode* node)
{
  String buf = StringFromFormat(
      "<failure message=\"%s:%d: %s\" type=\"AssertionFailedError\">\n",
      node->failure_->getFileName().c_str(),
      static_cast<int>(node->failure_->getFailureLineNumber()),
      encodeXmlText(node->failure_->getMessage()).c_str());
  writeToFile(buf.c_str());
  writeToFile("</failure>\n");
}

void
JUnitTestOutput::writeFileEnding()
{
  writeToFile("<system-out>");
  writeToFile(encodeXmlText(impl_->stdOutput_));
  writeToFile("</system-out>\n");
  writeToFile("<system-err></system-err>\n");
  writeToFile("</testsuite>\n");
}

void
JUnitTestOutput::writeTestGroupToFile()
{
  openFileForWrite(createFileName(impl_->results_.group_));
  writeXmlHeader();
  writeTestSuiteSummary();
  writeProperties();
  writeTestCases();
  writeFileEnding();
  closeFile();
}

void
JUnitTestOutput::printBuffer(const char*)
{
}

void
JUnitTestOutput::print(const char* output)
{
  impl_->stdOutput_ += output;
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
JUnitTestOutput::printFailure(const TestFailure& failure)
{
  if (impl_->results_.tail_->failure_ == nullptr) {
    impl_->results_.failureCount_++;
    impl_->results_.tail_->failure_ = new TestFailure(failure);
  }
}

void
JUnitTestOutput::openFileForWrite(const String& fileName)
{
  impl_->file_ = PlatformSpecificFOpen(fileName.c_str(), "w");
}

void
JUnitTestOutput::writeToFile(const String& buffer)
{
  PlatformSpecificFPuts(buffer.c_str(), impl_->file_);
}

void
JUnitTestOutput::closeFile()
{
  PlatformSpecificFClose(impl_->file_);
}

} // namespace cpputest
