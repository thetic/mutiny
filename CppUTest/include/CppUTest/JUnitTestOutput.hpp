#ifndef D_JUnitTestOutput_h
#define D_JUnitTestOutput_h

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

  virtual void printTestsStarted() override;
  virtual void printTestsEnded(const TestResult& result) override;
  virtual void printCurrentTestStarted(const TestShell& test) override;
  virtual void printCurrentTestEnded(const TestResult& res) override;
  virtual void printCurrentGroupStarted(const TestShell& test) override;
  virtual void printCurrentGroupEnded(const TestResult& res) override;

  virtual void printBuffer(const char*) override;
  virtual void print(const char*) override;
  virtual void print(long) override;
  virtual void print(size_t) override;
  virtual void printFailure(const TestFailure& failure) override;

  virtual void flush() override;

  virtual String createFileName(const String& group);
  void setPackageName(const String& package);

protected:
  JUnitTestOutputImpl* impl_;
  void resetTestGroupResult();

  virtual void openFileForWrite(const String& fileName);
  virtual void writeTestGroupToFile();
  virtual void writeToFile(const String& buffer);
  virtual void closeFile();

  virtual void writeXmlHeader();
  virtual void writeTestSuiteSummary();
  virtual void writeProperties();
  virtual void writeTestCases();
  virtual String encodeXmlText(const String& textbody);
  virtual String encodeFileName(const String& fileName);
  virtual void writeFailure(JUnitTestCaseResultNode* node);
  virtual void writeFileEnding();
};

} // namespace cpputest

#endif
