#ifndef D_MockFailure_h
#define D_MockFailure_h

#include "CppUTest/TestFailure.hpp"

namespace cpputest {
namespace extensions {

class MockExpectedCallsList;
class MockCheckedActualCall;
class MockNamedValue;
class MockFailure;

class MockFailureReporter
{
protected:
  bool crashOnFailure_;

public:
  MockFailureReporter()
    : crashOnFailure_(false)
  {
  }
  virtual ~MockFailureReporter() {}

  virtual void failTest(MockFailure failure);
  virtual void reportFailure(const MockFailure& failure);
  virtual void exitTest();
  virtual cpputest::TestShell* getTestToFail();

  virtual void crashOnFailure(bool shouldCrash)
  {
    crashOnFailure_ = shouldCrash;
  }
};

class MockFailure : public cpputest::TestFailure
{
public:
  MockFailure(cpputest::TestShell* test);
  MockFailure(MockFailure&&) noexcept = default;
  virtual ~MockFailure() override {}

protected:
  void addExpectationsAndCallHistory(const MockExpectedCallsList& expectations);
  void addExpectationsAndCallHistoryRelatedTo(const cpputest::String& function,
      const MockExpectedCallsList& expectations);
};

class MockExpectedCallsDidntHappenFailure : public MockFailure
{
public:
  MockExpectedCallsDidntHappenFailure(cpputest::TestShell* test,
      const MockExpectedCallsList& expectations);
};

class MockUnexpectedCallHappenedFailure : public MockFailure
{
public:
  MockUnexpectedCallHappenedFailure(cpputest::TestShell* test,
      const cpputest::String& name,
      const MockExpectedCallsList& expectations);
};

class MockCallOrderFailure : public MockFailure
{
public:
  MockCallOrderFailure(cpputest::TestShell* test,
      const MockExpectedCallsList& expectations);
};

class MockUnexpectedInputParameterFailure : public MockFailure
{
public:
  MockUnexpectedInputParameterFailure(cpputest::TestShell* test,
      const cpputest::String& functionName,
      MockNamedValue parameter,
      const MockExpectedCallsList& expectations);
};

class MockUnexpectedOutputParameterFailure : public MockFailure
{
public:
  MockUnexpectedOutputParameterFailure(cpputest::TestShell* test,
      const cpputest::String& functionName,
      MockNamedValue parameter,
      const MockExpectedCallsList& expectations);
};

class MockExpectedParameterDidntHappenFailure : public MockFailure
{
public:
  MockExpectedParameterDidntHappenFailure(cpputest::TestShell* test,
      const cpputest::String& functionName,
      const MockExpectedCallsList& allExpectations,
      const MockExpectedCallsList& matchingExpectations);
};

class MockNoWayToCompareCustomTypeFailure : public MockFailure
{
public:
  MockNoWayToCompareCustomTypeFailure(cpputest::TestShell* test,
      cpputest::String typeName);
};

class MockNoWayToCopyCustomTypeFailure : public MockFailure
{
public:
  MockNoWayToCopyCustomTypeFailure(cpputest::TestShell* test,
      cpputest::String typeName);
};

class MockUnexpectedObjectFailure : public MockFailure
{
public:
  MockUnexpectedObjectFailure(cpputest::TestShell* test,
      const cpputest::String& functionName,
      const void* expected,
      const MockExpectedCallsList& expectations);
};

class MockExpectedObjectDidntHappenFailure : public MockFailure
{
public:
  MockExpectedObjectDidntHappenFailure(cpputest::TestShell* test,
      const cpputest::String& functionName,
      const MockExpectedCallsList& expectations);
};

} // namespace extensions
} // namespace cpputest

#endif
