#ifndef D_MockFailureReporterForTest_h
#define D_MockFailureReporterForTest_h

#include "CppUTest/Extensions/MockSupport.hpp"

using namespace cpputest;
using namespace cpputest::extensions;
using UtestShell = cpputest::TestShell;

#define CHECK_EXPECTED_MOCK_FAILURE(expectedFailure)                           \
  CHECK_EXPECTED_MOCK_FAILURE_LOCATION(expectedFailure, __FILE__, __LINE__)
#define CHECK_NO_MOCK_FAILURE()                                                \
  CHECK_NO_MOCK_FAILURE_LOCATION(__FILE__, __LINE__)

class MockFailureReporterForTest : public MockFailureReporter
{
public:
  String mockFailureString;

  virtual void failTest(MockFailure failure) override;
  virtual void reportFailure(const MockFailure& failure) override;
  virtual void exitTest() override;
  static MockFailureReporterForTest* getReporter();
  static void clearReporter();

private:
  static MockFailureReporterForTest* instance_;
};

class MockFailureReporterInstaller
{
public:
  MockFailureReporterInstaller();
  ~MockFailureReporterInstaller();
};

UtestShell*
mockFailureTest();
String
mockFailureString();
void
CLEAR_MOCK_FAILURE();
void
CHECK_EXPECTED_MOCK_FAILURE_LOCATION(const MockFailure& expectedFailure,
    const char* file,
    size_t line);
void
CHECK_NO_MOCK_FAILURE_LOCATION(const char* file, size_t line);

class MockExpectedCallsListForTest : public MockExpectedCallsList
{
public:
  ~MockExpectedCallsListForTest() override;
  MockCheckedExpectedCall* addFunction(const String& name);
  MockCheckedExpectedCall* addFunction(unsigned int numCalls,
      const String& name);
  MockCheckedExpectedCall* addFunctionOrdered(const String& name,
      unsigned int order);
};

#endif
