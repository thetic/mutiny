#ifndef INCLUDED_MUTINY_MOCKSUPPORT_HPP
#define INCLUDED_MUTINY_MOCKSUPPORT_HPP

/**
 * @file MockSupport.hpp
 * @brief Core mock support: expectation recording, call verification, and data
 * sharing.
 *
 * The canonical entry point is the free function mock(), which returns a
 * reference to the global (or a named scope) MockSupport. Typical usage:
 *
 * @code
 * // In production code (the mock implementation):
 * int Database::query(const char* sql) {
 *   return mock().actual_call("query")
 *                .with_parameter("sql", sql)
 *                .return_int_value();
 * }
 *
 * // In the test:
 * TEST(DbTest, returns_row_count)
 * {
 *   mock().expect_one_call("query")
 *         .with_parameter("sql", "SELECT 1")
 *         .and_return_value(42);
 *
 *   CHECK_EQUAL(42, db.query("SELECT 1"));
 *   mock().check_expectations();
 *   mock().clear();
 * }
 * @endcode
 *
 * @see mock() for the free function entry point
 * @see MockSupportPlugin for automatic check/clear in teardown
 */

#include "mutiny/mock/ActualCall.hpp"
#include "mutiny/mock/ExpectedCall.hpp"

namespace mu {
namespace tiny {
namespace mock {

class MockCheckedActualCall;
class MockFailure;
class MockFailureReporter;
class MockNamedValueComparatorsAndCopiersRepository;
class MockNamedValueListNode;

/**
 * @brief Central mock-support object for expectation recording and
 * verification.
 *
 * One MockSupport instance represents one named scope (or the global scope when
 * the name is empty). Nested scopes are created transparently by mock("scope").
 *
 * @par Lifecycle within a test
 * 1. In setup (or at the start of the test): optionally call strict_order() or
 *    install_comparator().
 * 2. Register expectations with expect_one_call() / expect_n_calls().
 * 3. Production code calls actual_call() when the mocked function executes.
 * 4. In teardown: call check_expectations() then clear().
 *
 * @see mock(), MockSupportPlugin
 */
class MockSupport
{
public:
  /**
   * @brief Construct a MockSupport with an optional scope name.
   *
   * Prefer the free function mock() over constructing directly.
   *
   * @param mock_name  Scope name; empty string means the global scope.
   */
  MockSupport(const mu::tiny::test::String& mock_name = "");
  virtual ~MockSupport();

  /**
   * @brief Require actual calls to arrive in the same order expectations were
   * set.
   *
   * By default calls may arrive in any order. Call strict_order() before
   * registering expectations to enforce ordering.
   */
  virtual void strict_order();

  /**
   * @brief Expect exactly one call to @p function_name.
   *
   * @param function_name  Name of the function that should be called once.
   * @return A fluent MockExpectedCall for chaining .with_parameter() and
   *         .and_return_value() constraints.
   */
  virtual MockExpectedCall& expect_one_call(
      const mu::tiny::test::String& function_name
  );

  /**
   * @brief Assert that @p function_name is never called.
   *
   * If actual_call() is invoked with this name, the test fails immediately.
   *
   * @param function_name  Function that must not be called.
   */
  virtual void expect_no_call(const mu::tiny::test::String& function_name);

  /**
   * @brief Expect exactly @p amount calls to @p function_name.
   *
   * @param amount         Number of times the function should be called.
   * @param function_name  Name of the function.
   * @return A fluent MockExpectedCall for chaining parameter/return
   * constraints.
   */
  virtual MockExpectedCall& expect_n_calls(
      unsigned int amount,
      const mu::tiny::test::String& function_name
  );

  /**
   * @brief Report that @p function_name was actually called.
   *
   * Call this from inside a mock function implementation, then chain
   * with_parameter() for each argument. Retrieve the configured return value
   * with the appropriate return_*_value() method at the end.
   *
   * @param function_name  Name of the function being called.
   * @return A fluent MockActualCall for supplying parameters and reading the
   *         return value.
   */
  virtual MockActualCall& actual_call(const char* function_name);

  /** @return true if the last actual_call() has an associated return value. */
  virtual bool has_return_value();
  /** @return The return value of the last actual_call() as a MockNamedValue. */
  virtual MockNamedValue return_value();

  /** @return The bool return value configured for the current call. */
  virtual bool bool_return_value();
  /**
   * @return The bool return value if one was configured, otherwise @p
   * default_value.
   * @param default_value  Fallback when no return value was set.
   */
  virtual bool return_bool_value_or_default(bool default_value);

  /** @return The int return value configured for the current call. */
  virtual int int_return_value();
  /**
   * @return The int return value if one was configured, otherwise @p
   * default_value.
   * @param default_value  Fallback when no return value was set.
   */
  virtual int return_int_value_or_default(int default_value);

  /** @return The unsigned int return value configured for the current call. */
  virtual unsigned int unsigned_int_return_value();

  /** @return The long int return value configured for the current call. */
  virtual long int long_int_return_value();
  /**
   * @return The long int return value if configured, otherwise @p
   * default_value.
   * @param default_value  Fallback when no return value was set.
   */
  virtual long int return_long_int_value_or_default(long int default_value);

  /** @return The unsigned long int return value configured for the current
   * call. */
  virtual unsigned long int unsigned_long_int_return_value();
  /**
   * @return The unsigned long int return value if configured, otherwise @p
   * default_value.
   * @param default_value  Fallback when no return value was set.
   */
  virtual unsigned long int return_unsigned_long_int_value_or_default(
      unsigned long int default_value
  );

  /** @return The long long int return value configured for the current call. */
  virtual long long long_long_int_return_value();
  /**
   * @return The long long int return value if configured, otherwise @p
   * default_value.
   * @param default_value  Fallback when no return value was set.
   */
  virtual long long return_long_long_int_value_or_default(
      long long default_value
  );

  /** @return The unsigned long long int return value for the current call. */
  virtual unsigned long long unsigned_long_long_int_return_value();
  /**
   * @return The unsigned long long return value if configured, otherwise @p
   * default_value.
   * @param default_value  Fallback when no return value was set.
   */
  virtual unsigned long long return_unsigned_long_long_int_value_or_default(
      unsigned long long default_value
  );

  /**
   * @return The unsigned int return value if configured, otherwise @p
   * default_value.
   * @param default_value  Fallback when no return value was set.
   */
  virtual unsigned int return_unsigned_int_value_or_default(
      unsigned int default_value
  );

  /** @return The const char* return value configured for the current call. */
  virtual const char* string_return_value();
  /**
   * @return The string return value if configured, otherwise @p default_value.
   * @param default_value  Fallback when no return value was set.
   */
  virtual const char* return_string_value_or_default(const char* default_value);

  /**
   * @return The double return value if configured, otherwise @p default_value.
   * @param default_value  Fallback when no return value was set.
   */
  virtual double return_double_value_or_default(double default_value);
  /** @return The double return value configured for the current call. */
  virtual double double_return_value();

  /** @return The void* return value configured for the current call. */
  virtual void* pointer_return_value();
  /**
   * @return The pointer return value if configured, otherwise @p default_value.
   * @param default_value  Fallback when no return value was set.
   */
  virtual void* return_pointer_value_or_default(void* default_value);
  /**
   * @return The const void* return value if configured, otherwise @p
   * default_value.
   * @param default_value  Fallback when no return value was set.
   */
  virtual const void* return_const_pointer_value_or_default(
      const void* default_value
  );
  /** @return The const void* return value configured for the current call. */
  virtual const void* const_pointer_return_value();

  /**
   * @return The function-pointer return value if configured, otherwise @p
   * default_value.
   * @param default_value  Fallback when no return value was set.
   */
  virtual void (*return_function_pointer_value_or_default(void (*default_value)()))();
  /** @return The function-pointer return value configured for the current call.
   */
  virtual void (*function_pointer_return_value())();

  /**
   * @brief Check whether named data exists in this scope.
   *
   * @param name  Data key.
   * @return true if set_data() was previously called with @p name.
   */
  bool has_data(const mu::tiny::test::String& name);

  /** @brief Store a bool value under @p name for retrieval across mock calls.
   * @param name Key. @param value Value. */
  void set_data(const mu::tiny::test::String& name, bool value);
  /** @brief Store an int value. @param name Key. @param value Value. */
  void set_data(const mu::tiny::test::String& name, int value);
  /** @brief Store an unsigned int value. @param name Key. @param value Value.
   */
  void set_data(const mu::tiny::test::String& name, unsigned int value);
  /** @brief Store a long int value. @param name Key. @param value Value. */
  void set_data(const mu::tiny::test::String& name, long int value);
  /** @brief Store an unsigned long int value. @param name Key. @param value
   * Value. */
  void set_data(const mu::tiny::test::String& name, unsigned long int value);
  /** @brief Store a string value. @param name Key. @param value Value. */
  void set_data(const mu::tiny::test::String& name, const char* value);
  /** @brief Store a double value. @param name Key. @param value Value. */
  void set_data(const mu::tiny::test::String& name, double value);
  /** @brief Store a void* value. @param name Key. @param value Value. */
  void set_data(const mu::tiny::test::String& name, void* value);
  /** @brief Store a const void* value. @param name Key. @param value Value. */
  void set_data(const mu::tiny::test::String& name, const void* value);
  /** @brief Store a function pointer value. @param name Key. @param value
   * Value. */
  void set_data(const mu::tiny::test::String& name, void (*value)());

  /**
   * @brief Store a mutable object pointer with a type name.
   *
   * @param name   Data key.
   * @param type   Type name string (used for display on failure).
   * @param value  Pointer to the object.
   */
  void set_data_object(
      const mu::tiny::test::String& name,
      const mu::tiny::test::String& type,
      void* value
  );

  /**
   * @brief Store a const object pointer with a type name.
   *
   * @param name   Data key.
   * @param type   Type name string.
   * @param value  Const pointer to the object.
   */
  void set_data_const_object(
      const mu::tiny::test::String& name,
      const mu::tiny::test::String& type,
      const void* value
  );

  /**
   * @brief Retrieve a previously stored data value by name.
   *
   * @param name  Data key.
   * @return The stored MockNamedValue; type is undefined if @p name was not
   * set.
   */
  MockNamedValue get_data(const mu::tiny::test::String& name);

  /**
   * @brief Get (or create) a named child scope.
   *
   * Equivalent to calling mock("scope") but from an existing MockSupport
   * reference. Useful when you already hold a reference to the global scope.
   *
   * @param name  Scope name.
   * @return Pointer to the child MockSupport scope.
   */
  MockSupport* get_mock_support_scope(const mu::tiny::test::String& name);

  /**
   * @return A C string containing all traced call output, or empty string if
   *         tracing was not enabled.
   */
  const char* get_trace_output();

  /** @brief Disable this scope; all actual_call() invocations become no-ops. */
  virtual void disable();
  /** @brief Re-enable this scope after disable(). */
  virtual void enable();

  /**
   * @brief Enable or disable call tracing to the test output.
   *
   * @param enabled  true to print each actual call and its parameters.
   */
  virtual void tracing(bool enabled);

  /**
   * @brief Suppress unexpected-call failures.
   *
   * All calls to actual_call() are silently accepted without matching an
   * expectation. Useful when testing code that uses a collaborator you
   * do not want to fully specify.
   */
  virtual void ignore_other_calls();

  /**
   * @brief Verify all expected calls were fulfilled; fail the test if not.
   *
   * Typically called in teardown() after all assertions have run. Reports
   * every unsatisfied expectation as a separate test failure.
   */
  virtual void check_expectations();

  /** @return true if there are registered expectations that have not been
   * called yet. */
  virtual bool expected_calls_left();

  /**
   * @brief Remove all expectations, actual calls, data, and child scopes.
   *
   * Call in teardown() after check_expectations() to leave the mock support
   * in a clean state for the next test.
   */
  virtual void clear();

  /**
   * @brief Choose whether unexpected call failures crash the process.
   *
   * @param should_fail  true to crash on mock failure (useful with a debugger).
   */
  virtual void crash_on_failure(bool should_fail = true);

  /**
   * @brief Replace the standard failure reporter.
   *
   * @param reporter  New standard reporter; pass nullptr to restore the
   * default.
   */
  virtual void set_mock_failure_standard_reporter(
      MockFailureReporter* reporter
  );

  /**
   * @brief Set the active failure reporter for the current call chain.
   *
   * @param active_reporter  Reporter to use; may differ from the standard one.
   */
  virtual void set_active_reporter(MockFailureReporter* active_reporter);

  /** @brief Restore the default comparators-and-copiers repository. */
  virtual void set_default_comparators_and_copiers_repository();

  /**
   * @brief Register a custom comparator for objects of type @p type_name.
   *
   * Required when passing objects via with_parameter_of_type().
   *
   * @param type_name   Type name string, must match the one used in
   * with_parameter_of_type().
   * @param comparator  Comparator whose lifetime must exceed the test.
   */
  virtual void install_comparator(
      const mu::tiny::test::String& type_name,
      MockNamedValueComparator& comparator
  );

  /**
   * @brief Register a custom copier for objects of type @p type_name.
   *
   * Required when using with_output_parameter_of_type_returning().
   *
   * @param type_name  Type name string.
   * @param copier     Copier whose lifetime must exceed the test.
   */
  virtual void install_copier(
      const mu::tiny::test::String& type_name,
      MockNamedValueCopier& copier
  );

  /**
   * @brief Install all comparators and copiers from a repository at once.
   *
   * @param repository  Repository containing the comparators and copiers to
   * install.
   */
  virtual void install_comparators_and_copiers(
      const MockNamedValueComparatorsAndCopiersRepository& repository
  );

  /** @brief Remove all installed comparators and copiers from this scope. */
  virtual void remove_all_comparators_and_copiers();

protected:
  /** @brief Create a child MockSupport scope with the given name. */
  MockSupport* clone(const mu::tiny::test::String& mock_name);
  /** @brief Factory method for the actual-call object (override in tests). */
  virtual MockCheckedActualCall* create_actual_call();
  /** @brief Record a mock failure via the active failure reporter. */
  virtual void fail_test(MockFailure& failure);
  /** @brief Increment the assertion-check count in the active TestResult. */
  void count_check();

private:
  class Impl;
  Impl* impl_;

  unsigned int actual_call_order_{ 0 };
  unsigned int expected_call_order_{ 0 };
  bool strict_ordering_{ false };
  bool ignore_other_calls_{ false };
  bool enabled_{ true };
  const mu::tiny::test::String mock_name_;
  bool tracing_{ false };

  void check_expectations_of_last_actual_call();
  bool was_last_actual_call_fulfilled();
  void fail_test_with_expected_calls_not_fulfilled();
  void fail_test_with_out_of_order_calls();

  MockNamedValue* retrieve_data_from_store(const mu::tiny::test::String& name);

  MockSupport* get_mock_support(MockNamedValueListNode* node);

  bool call_is_ignored(const mu::tiny::test::String& function_name);
  bool has_calls_out_of_order();

  mu::tiny::test::String append_scope_to_name(
      const mu::tiny::test::String& function_name
  );
};

/**
 * @brief Access the global mock support or a named scope.
 *
 * This is the primary entry point for all mock interaction in test code.
 *
 * @code
 * // Global scope:
 * mock().expect_one_call("connect");
 *
 * // Named scope (useful when multiple subsystems share a mock):
 * mock("db").expect_one_call("query");
 * @endcode
 *
 * @param mock_name                     Scope name; empty string is the global
 * scope.
 * @param failure_reporter_for_this_call  Optional reporter override for this
 * call chain.
 * @return Reference to the MockSupport for the requested scope.
 */
MockSupport& mock(
    const mu::tiny::test::String& mock_name = "",
    MockFailureReporter* failure_reporter_for_this_call = nullptr
);

}
}
} // namespace mu::tiny::mock

#endif
