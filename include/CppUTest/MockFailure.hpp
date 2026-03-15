#ifndef INCLUDED_CPPUTEST_MOCKFAILURE_HPP
#define INCLUDED_CPPUTEST_MOCKFAILURE_HPP

#include "CppUTest/TestFailure.hpp"

namespace cpputest {

class MockExpectedCallsList;
class MockCheckedActualCall;
class MockNamedValue;
class MockFailure;

class MockFailureReporter
{
protected:
  bool crash_on_failure_;

public:
  MockFailureReporter()
    : crash_on_failure_(false)
  {
  }
  virtual ~MockFailureReporter() {}

  virtual void fail_test(MockFailure failure);
  virtual void report_failure(const MockFailure& failure);
  virtual void exit_test();
  virtual cpputest::TestShell* get_test_to_fail();

  virtual void crash_on_failure(bool should_crash)
  {
    crash_on_failure_ = should_crash;
  }
};

class MockFailure : public cpputest::TestFailure
{
public:
  MockFailure(cpputest::TestShell* test);
  MockFailure(MockFailure&&) noexcept = default;
  virtual ~MockFailure() override {}

protected:
  void add_expectations_and_call_history(
      const MockExpectedCallsList& expectations);
  void add_expectations_and_call_history_related_to(
      const cpputest::String& function,
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
      const cpputest::String& function_name,
      MockNamedValue parameter,
      const MockExpectedCallsList& expectations);
};

class MockUnexpectedOutputParameterFailure : public MockFailure
{
public:
  MockUnexpectedOutputParameterFailure(cpputest::TestShell* test,
      const cpputest::String& function_name,
      MockNamedValue parameter,
      const MockExpectedCallsList& expectations);
};

class MockExpectedParameterDidntHappenFailure : public MockFailure
{
public:
  MockExpectedParameterDidntHappenFailure(cpputest::TestShell* test,
      const cpputest::String& function_name,
      const MockExpectedCallsList& all_expectations,
      const MockExpectedCallsList& matching_expectations);
};

class MockNoWayToCompareCustomTypeFailure : public MockFailure
{
public:
  MockNoWayToCompareCustomTypeFailure(cpputest::TestShell* test,
      cpputest::String type_name);
};

class MockNoWayToCopyCustomTypeFailure : public MockFailure
{
public:
  MockNoWayToCopyCustomTypeFailure(cpputest::TestShell* test,
      cpputest::String type_name);
};

class MockUnexpectedObjectFailure : public MockFailure
{
public:
  MockUnexpectedObjectFailure(cpputest::TestShell* test,
      const cpputest::String& function_name,
      const void* expected,
      const MockExpectedCallsList& expectations);
};

class MockExpectedObjectDidntHappenFailure : public MockFailure
{
public:
  MockExpectedObjectDidntHappenFailure(cpputest::TestShell* test,
      const cpputest::String& function_name,
      const MockExpectedCallsList& expectations);
};

} // namespace cpputest

#endif
