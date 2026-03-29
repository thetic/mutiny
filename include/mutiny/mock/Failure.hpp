/**
 * @file
 * @brief Mock failure reporter and failure types.
 *
 * @see mu::tiny::mock::Support, mu::tiny::mock::Failure
 */

#ifndef INCLUDED_MUTINY_MOCK_FAILURE_HPP
#define INCLUDED_MUTINY_MOCK_FAILURE_HPP

#include "mutiny/test/Failure.hpp"

#include "mutiny/export.h"

namespace mu {
namespace tiny {
namespace mock {

class ExpectedCallsList;
class CheckedActualCall;
class NamedValue;
class Failure;

/**
 * @brief Sink for mock failures; reports them through the test framework.
 *
 * The default implementation forwards failures to the current Shell. Replace
 * the reporter on a Support object to intercept or silence mock failures in
 * tests that verify the mock layer itself.
 *
 * @see Support::set_mock_failure_reporter()
 */
class MUTINY_EXPORT FailureReporter
{
protected:
  bool crash_on_failure_{ false };

public:
  FailureReporter() = default;
  virtual ~FailureReporter() = default;

  /**
   * @brief Report @p failure and exit the current test.
   * @param failure  The failure to report.
   */
  virtual void fail_test(Failure failure);

  /**
   * @brief Report @p failure without exiting the test.
   * @param failure  The failure to report.
   */
  virtual void report_failure(const Failure& failure);

  /** @brief Exit the currently running test. */
  virtual void exit_test();

  /**
   * @return The Shell that will receive failures, or nullptr if none.
   */
  virtual test::Shell* get_test_to_fail();

  /**
   * @brief Control whether failures trigger a hard crash.
   * @param should_crash  If true, crash the process on the next failure.
   */
  virtual void crash_on_failure(bool should_crash)
  {
    crash_on_failure_ = should_crash;
  }
};

/**
 * @brief Base class for all mock-layer failure messages.
 *
 * Extends test::Failure to carry expectation history in the failure message.
 * Concrete subclasses describe specific kinds of mock violations.
 *
 * @see FailureReporter, Support
 */
class MUTINY_EXPORT Failure : public test::Failure
{
public:
  /**
   * @brief Construct an empty mock failure attached to @p test.
   * @param test  The test shell that triggered the failure.
   */
  Failure(test::Shell* test);
  Failure(Failure&&) noexcept = default;
  ~Failure() override = default;

protected:
  /**
   * @brief Append the full expectations/call history to the failure message.
   * @param expectations  The expected-calls list at the time of failure.
   */
  void add_expectations_and_call_history(const ExpectedCallsList& expectations);

  /**
   * @brief Append history filtered to a specific function name.
   *
   * @param function      Name of the function whose history to include.
   * @param expectations  The full expected-calls list.
   */
  void add_expectations_and_call_history_related_to(
      const String& function,
      const ExpectedCallsList& expectations
  );
};

/**
 * @brief Failure reported when expected calls were never made.
 *
 * Triggered by Support::check_expectations() when one or more expected calls
 * did not occur.
 */
class MUTINY_EXPORT ExpectedCallsDidntHappenFailure : public Failure
{
public:
  /**
   * @param test          The failing test.
   * @param expectations  The unsatisfied expected-calls list.
   */
  ExpectedCallsDidntHappenFailure(
      test::Shell* test,
      const ExpectedCallsList& expectations
  );
};

/**
 * @brief Failure reported when an unexpected call is made.
 *
 * Triggered when a mock function is called but no matching expectation exists.
 */
class MUTINY_EXPORT UnexpectedCallHappenedFailure : public Failure
{
public:
  /**
   * @param test          The failing test.
   * @param name          Name of the unexpectedly called function.
   * @param expectations  The current expected-calls list.
   */
  UnexpectedCallHappenedFailure(
      test::Shell* test,
      const String& name,
      const ExpectedCallsList& expectations
  );
};

/**
 * @brief Failure reported when calls arrive in the wrong order.
 *
 * Triggered when ordered expectations (set via with_call_order()) are
 * satisfied out of sequence.
 */
class MUTINY_EXPORT CallOrderFailure : public Failure
{
public:
  /**
   * @param test          The failing test.
   * @param expectations  The expected-calls list at time of failure.
   */
  CallOrderFailure(test::Shell* test, const ExpectedCallsList& expectations);
};

/**
 * @brief Failure reported when an input parameter value does not match any
 * expectation.
 */
class MUTINY_EXPORT UnexpectedInputParameterFailure : public Failure
{
public:
  /**
   * @param test          The failing test.
   * @param function_name Name of the mock function called.
   * @param parameter     The unexpected parameter.
   * @param expectations  The current expected-calls list.
   */
  UnexpectedInputParameterFailure(
      test::Shell* test,
      const String& function_name,
      NamedValue parameter,
      const ExpectedCallsList& expectations
  );
};

/**
 * @brief Failure reported when an output parameter does not match any
 * expectation.
 */
class MUTINY_EXPORT UnexpectedOutputParameterFailure : public Failure
{
public:
  /**
   * @param test          The failing test.
   * @param function_name Name of the mock function called.
   * @param parameter     The unexpected output parameter.
   * @param expectations  The current expected-calls list.
   */
  UnexpectedOutputParameterFailure(
      test::Shell* test,
      const String& function_name,
      NamedValue parameter,
      const ExpectedCallsList& expectations
  );
};

/**
 * @brief Failure reported when an expected parameter was never passed.
 */
class MUTINY_EXPORT ExpectedParameterDidntHappenFailure : public Failure
{
public:
  /**
   * @param test                  The failing test.
   * @param function_name         Name of the mock function.
   * @param all_expectations      All current expectations.
   * @param matching_expectations Expectations that matched the call name.
   */
  ExpectedParameterDidntHappenFailure(
      test::Shell* test,
      const String& function_name,
      const ExpectedCallsList& all_expectations,
      const ExpectedCallsList& matching_expectations
  );
};

/**
 * @brief Failure reported when no comparator is registered for a custom type.
 *
 * @see NamedValueComparator, Support::install_comparator()
 */
class MUTINY_EXPORT NoWayToCompareCustomTypeFailure : public Failure
{
public:
  /**
   * @param test       The failing test.
   * @param type_name  Name of the type lacking a comparator.
   */
  NoWayToCompareCustomTypeFailure(test::Shell* test, String type_name);
};

/**
 * @brief Failure reported when no copier is registered for a custom type.
 *
 * @see NamedValueCopier, Support::install_copier()
 */
class MUTINY_EXPORT NoWayToCopyCustomTypeFailure : public Failure
{
public:
  /**
   * @param test       The failing test.
   * @param type_name  Name of the type lacking a copier.
   */
  NoWayToCopyCustomTypeFailure(test::Shell* test, String type_name);
};

/**
 * @brief Failure reported when a call arrives on an unexpected object.
 *
 * Triggered when on_object() was set in the expectation but the actual call
 * used a different object pointer.
 */
class MUTINY_EXPORT UnexpectedObjectFailure : public Failure
{
public:
  /**
   * @param test          The failing test.
   * @param function_name Name of the mock function.
   * @param expected      The object pointer that was expected.
   * @param expectations  The current expected-calls list.
   */
  UnexpectedObjectFailure(
      test::Shell* test,
      const String& function_name,
      const void* expected,
      const ExpectedCallsList& expectations
  );
};

/**
 * @brief Failure reported when an expected object constraint was never
 * satisfied.
 *
 * Triggered by check_expectations() when an expectation with on_object() was
 * never matched.
 */
class MUTINY_EXPORT ExpectedObjectDidntHappenFailure : public Failure
{
public:
  /**
   * @param test          The failing test.
   * @param function_name Name of the mock function.
   * @param expectations  The unsatisfied expected-calls list.
   */
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
