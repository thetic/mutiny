#ifndef INCLUDED_MUTINY_MOCKFAILURE_HPP
#define INCLUDED_MUTINY_MOCKFAILURE_HPP

#include "mutiny/test/Failure.hpp"

namespace mu {
namespace tiny {
namespace mock {

class MockExpectedCallsList;
class MockCheckedActualCall;
class MockNamedValue;
class MockFailure;

class MockFailureReporter
{
protected:
  bool crash_on_failure_{ false };

public:
  MockFailureReporter() = default;
  virtual ~MockFailureReporter() = default;

  virtual void fail_test(MockFailure failure);
  virtual void report_failure(const MockFailure& failure);
  virtual void exit_test();
  virtual mu::tiny::test::TestShell* get_test_to_fail();

  virtual void crash_on_failure(bool should_crash)
  {
    crash_on_failure_ = should_crash;
  }
};

class MockFailure : public mu::tiny::test::TestFailure
{
public:
  MockFailure(mu::tiny::test::TestShell* test);
  MockFailure(MockFailure&&) noexcept = default;
  ~MockFailure() override = default;

protected:
  void add_expectations_and_call_history(
      const MockExpectedCallsList& expectations
  );
  void add_expectations_and_call_history_related_to(
      const mu::tiny::test::String& function,
      const MockExpectedCallsList& expectations
  );
};

class MockExpectedCallsDidntHappenFailure : public MockFailure
{
public:
  MockExpectedCallsDidntHappenFailure(
      mu::tiny::test::TestShell* test,
      const MockExpectedCallsList& expectations
  );
};

class MockUnexpectedCallHappenedFailure : public MockFailure
{
public:
  MockUnexpectedCallHappenedFailure(
      mu::tiny::test::TestShell* test,
      const mu::tiny::test::String& name,
      const MockExpectedCallsList& expectations
  );
};

class MockCallOrderFailure : public MockFailure
{
public:
  MockCallOrderFailure(
      mu::tiny::test::TestShell* test,
      const MockExpectedCallsList& expectations
  );
};

class MockUnexpectedInputParameterFailure : public MockFailure
{
public:
  MockUnexpectedInputParameterFailure(
      mu::tiny::test::TestShell* test,
      const mu::tiny::test::String& function_name,
      MockNamedValue parameter,
      const MockExpectedCallsList& expectations
  );
};

class MockUnexpectedOutputParameterFailure : public MockFailure
{
public:
  MockUnexpectedOutputParameterFailure(
      mu::tiny::test::TestShell* test,
      const mu::tiny::test::String& function_name,
      MockNamedValue parameter,
      const MockExpectedCallsList& expectations
  );
};

class MockExpectedParameterDidntHappenFailure : public MockFailure
{
public:
  MockExpectedParameterDidntHappenFailure(
      mu::tiny::test::TestShell* test,
      const mu::tiny::test::String& function_name,
      const MockExpectedCallsList& all_expectations,
      const MockExpectedCallsList& matching_expectations
  );
};

class MockNoWayToCompareCustomTypeFailure : public MockFailure
{
public:
  MockNoWayToCompareCustomTypeFailure(
      mu::tiny::test::TestShell* test,
      mu::tiny::test::String type_name
  );
};

class MockNoWayToCopyCustomTypeFailure : public MockFailure
{
public:
  MockNoWayToCopyCustomTypeFailure(
      mu::tiny::test::TestShell* test,
      mu::tiny::test::String type_name
  );
};

class MockUnexpectedObjectFailure : public MockFailure
{
public:
  MockUnexpectedObjectFailure(
      mu::tiny::test::TestShell* test,
      const mu::tiny::test::String& function_name,
      const void* expected,
      const MockExpectedCallsList& expectations
  );
};

class MockExpectedObjectDidntHappenFailure : public MockFailure
{
public:
  MockExpectedObjectDidntHappenFailure(
      mu::tiny::test::TestShell* test,
      const mu::tiny::test::String& function_name,
      const MockExpectedCallsList& expectations
  );
};

}
}
} // namespace mu::tiny::mock

#endif
