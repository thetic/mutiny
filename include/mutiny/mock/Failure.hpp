#ifndef INCLUDED_MUTINY_MOCK_FAILURE_HPP
#define INCLUDED_MUTINY_MOCK_FAILURE_HPP

#include "mutiny/test/Failure.hpp"

namespace mu {
namespace tiny {
namespace mock {

class ExpectedCallsList;
class CheckedActualCall;
class NamedValue;
class Failure;

class FailureReporter
{
protected:
  bool crash_on_failure_{ false };

public:
  FailureReporter() = default;
  virtual ~FailureReporter() = default;

  virtual void fail_test(Failure failure);
  virtual void report_failure(const Failure& failure);
  virtual void exit_test();
  virtual test::Shell* get_test_to_fail();

  virtual void crash_on_failure(bool should_crash)
  {
    crash_on_failure_ = should_crash;
  }
};

class Failure : public test::Failure
{
public:
  Failure(test::Shell* test);
  Failure(Failure&&) noexcept = default;
  ~Failure() override = default;

protected:
  void add_expectations_and_call_history(const ExpectedCallsList& expectations);
  void add_expectations_and_call_history_related_to(
      const String& function,
      const ExpectedCallsList& expectations
  );
};

class ExpectedCallsDidntHappenFailure : public Failure
{
public:
  ExpectedCallsDidntHappenFailure(
      test::Shell* test,
      const ExpectedCallsList& expectations
  );
};

class UnexpectedCallHappenedFailure : public Failure
{
public:
  UnexpectedCallHappenedFailure(
      test::Shell* test,
      const String& name,
      const ExpectedCallsList& expectations
  );
};

class CallOrderFailure : public Failure
{
public:
  CallOrderFailure(test::Shell* test, const ExpectedCallsList& expectations);
};

class UnexpectedInputParameterFailure : public Failure
{
public:
  UnexpectedInputParameterFailure(
      test::Shell* test,
      const String& function_name,
      NamedValue parameter,
      const ExpectedCallsList& expectations
  );
};

class UnexpectedOutputParameterFailure : public Failure
{
public:
  UnexpectedOutputParameterFailure(
      test::Shell* test,
      const String& function_name,
      NamedValue parameter,
      const ExpectedCallsList& expectations
  );
};

class ExpectedParameterDidntHappenFailure : public Failure
{
public:
  ExpectedParameterDidntHappenFailure(
      test::Shell* test,
      const String& function_name,
      const ExpectedCallsList& all_expectations,
      const ExpectedCallsList& matching_expectations
  );
};

class NoWayToCompareCustomTypeFailure : public Failure
{
public:
  NoWayToCompareCustomTypeFailure(test::Shell* test, String type_name);
};

class NoWayToCopyCustomTypeFailure : public Failure
{
public:
  NoWayToCopyCustomTypeFailure(test::Shell* test, String type_name);
};

class UnexpectedObjectFailure : public Failure
{
public:
  UnexpectedObjectFailure(
      test::Shell* test,
      const String& function_name,
      const void* expected,
      const ExpectedCallsList& expectations
  );
};

class ExpectedObjectDidntHappenFailure : public Failure
{
public:
  ExpectedObjectDidntHappenFailure(
      test::Shell* test,
      const String& function_name,
      const ExpectedCallsList& expectations
  );
};

} // namespace mock
} // namespace tiny
} // namespace mu

#endif
