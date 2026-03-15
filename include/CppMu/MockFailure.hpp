#ifndef INCLUDED_CPPMU_MOCKFAILURE_HPP
#define INCLUDED_CPPMU_MOCKFAILURE_HPP

#include "CppMu/TestFailure.hpp"

namespace cppmu {

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
  virtual cppmu::TestShell* get_test_to_fail();

  virtual void crash_on_failure(bool should_crash)
  {
    crash_on_failure_ = should_crash;
  }
};

class MockFailure : public cppmu::TestFailure
{
public:
  MockFailure(cppmu::TestShell* test);
  MockFailure(MockFailure&&) noexcept = default;
  virtual ~MockFailure() override {}

protected:
  void add_expectations_and_call_history(
      const MockExpectedCallsList& expectations);
  void add_expectations_and_call_history_related_to(
      const cppmu::String& function,
      const MockExpectedCallsList& expectations);
};

class MockExpectedCallsDidntHappenFailure : public MockFailure
{
public:
  MockExpectedCallsDidntHappenFailure(cppmu::TestShell* test,
      const MockExpectedCallsList& expectations);
};

class MockUnexpectedCallHappenedFailure : public MockFailure
{
public:
  MockUnexpectedCallHappenedFailure(cppmu::TestShell* test,
      const cppmu::String& name,
      const MockExpectedCallsList& expectations);
};

class MockCallOrderFailure : public MockFailure
{
public:
  MockCallOrderFailure(cppmu::TestShell* test,
      const MockExpectedCallsList& expectations);
};

class MockUnexpectedInputParameterFailure : public MockFailure
{
public:
  MockUnexpectedInputParameterFailure(cppmu::TestShell* test,
      const cppmu::String& function_name,
      MockNamedValue parameter,
      const MockExpectedCallsList& expectations);
};

class MockUnexpectedOutputParameterFailure : public MockFailure
{
public:
  MockUnexpectedOutputParameterFailure(cppmu::TestShell* test,
      const cppmu::String& function_name,
      MockNamedValue parameter,
      const MockExpectedCallsList& expectations);
};

class MockExpectedParameterDidntHappenFailure : public MockFailure
{
public:
  MockExpectedParameterDidntHappenFailure(cppmu::TestShell* test,
      const cppmu::String& function_name,
      const MockExpectedCallsList& all_expectations,
      const MockExpectedCallsList& matching_expectations);
};

class MockNoWayToCompareCustomTypeFailure : public MockFailure
{
public:
  MockNoWayToCompareCustomTypeFailure(cppmu::TestShell* test,
      cppmu::String type_name);
};

class MockNoWayToCopyCustomTypeFailure : public MockFailure
{
public:
  MockNoWayToCopyCustomTypeFailure(cppmu::TestShell* test,
      cppmu::String type_name);
};

class MockUnexpectedObjectFailure : public MockFailure
{
public:
  MockUnexpectedObjectFailure(cppmu::TestShell* test,
      const cppmu::String& function_name,
      const void* expected,
      const MockExpectedCallsList& expectations);
};

class MockExpectedObjectDidntHappenFailure : public MockFailure
{
public:
  MockExpectedObjectDidntHappenFailure(cppmu::TestShell* test,
      const cppmu::String& function_name,
      const MockExpectedCallsList& expectations);
};

} // namespace cppmu

#endif
