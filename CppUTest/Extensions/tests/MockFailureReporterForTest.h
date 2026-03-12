#ifndef D_MockFailureReporterForTest_h
#define D_MockFailureReporterForTest_h

#include "CppUTest/Extensions/MockSupport.hpp"

#define CHECK_EXPECTED_MOCK_FAILURE(expectedFailure)                           \
  CHECK_EXPECTED_MOCK_FAILURE_LOCATION(expectedFailure, __FILE__, __LINE__)
#define CHECK_NO_MOCK_FAILURE()                                                \
  CHECK_NO_MOCK_FAILURE_LOCATION(__FILE__, __LINE__)

class MockFailureReporterForTest
  : public cpputest::extensions::MockFailureReporter
{
public:
  cpputest::String mockFailureString;

  virtual void failTest(cpputest::extensions::MockFailure failure) override;
  virtual void reportFailure(
      const cpputest::extensions::MockFailure& failure) override;
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

cpputest::TestShell*
mockFailureTest();
cpputest::String
mockFailureString();
void
CLEAR_MOCK_FAILURE();
void
CHECK_EXPECTED_MOCK_FAILURE_LOCATION(
    const cpputest::extensions::MockFailure& expectedFailure,
    const char* file,
    size_t line);
void
CHECK_NO_MOCK_FAILURE_LOCATION(const char* file, size_t line);

class MockExpectedCallsListForTest
  : public cpputest::extensions::MockExpectedCallsList
{
public:
  ~MockExpectedCallsListForTest() override;
  cpputest::extensions::MockCheckedExpectedCall* addFunction(
      const cpputest::String& name);
  cpputest::extensions::MockCheckedExpectedCall* addFunction(
      unsigned int numCalls,
      const cpputest::String& name);
  cpputest::extensions::MockCheckedExpectedCall* addFunctionOrdered(
      const cpputest::String& name,
      unsigned int order);
};

#endif
