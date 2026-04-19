/**
 * @file
 * @brief Core mock support: expectation recording, call verification, and data
 * sharing.
 *
 * The canonical entry point is the free function @ref mu::tiny::mock::mock(),
 * which returns a reference to the global (or a named scope)
 * @ref mu::tiny::mock::Support.
 */

#ifndef INCLUDED_MU_TINY_MOCK_HPP
#define INCLUDED_MU_TINY_MOCK_HPP

#include "mu/tiny/mock/ActualCall.hpp"
#include "mu/tiny/mock/ExpectedCall.hpp"

#include "mu/tiny/export.h"

namespace mu {
namespace tiny {
namespace mock {

class CheckedActualCall;
class Failure;
class FailureReporter;
class NamedValueComparatorsAndCopiersRepository;
class NamedValueListNode;

/**
 * @brief Central mock-support object for expectation recording and
 * verification.
 *
 * One Support instance represents one named scope (or the global scope when
 * the name is empty). Nested scopes are created transparently by @c
 * mock("scope").
 *
 * @par Lifecycle within a test
 * 1. In setup (or at the start of the test): optionally call @ref
 * strict_order() or
 *    @ref install_comparator().
 * 2. Register expectations with @ref expect_one_call() / @ref expect_n_calls().
 * 3. Production code calls @ref actual_call() when the mocked function
 * executes.
 * 4. In teardown: call @ref check_expectations() then @ref clear().
 */
class MUTINY_EXPORT Support
{
public:
  /** @brief Type alias for a pointer to a function with signature `void()`. */
  using FunctionPointerValue = void (*)();

  /**
   * @brief Construct a Support with an optional scope name.
   *
   * Prefer the free function @ref mock() over constructing directly.
   *
   * @param mock_name  Scope name; empty string means the global scope.
   */
  Support(StringView mock_name = "");
  virtual ~Support();

  /**
   * @brief Require actual calls to arrive in the same order expectations were
   * set.
   *
   * By default calls may arrive in any order. Call @ref strict_order() before
   * registering expectations to enforce ordering.
   */
  virtual void strict_order();

  /**
   * @brief Expect exactly one call to @p function_name.
   *
   * @param function_name  Name of the function that should be called once.
   * @return A fluent @ref ExpectedCall for chaining @ref
   *         mu::tiny::mock::ExpectedCall::with_parameter() and @ref
   *         mu::tiny::mock::ExpectedCall::and_return_value() constraints.
   */
  virtual ExpectedCall& expect_one_call(StringView function_name);

  /**
   * @brief Assert that @p function_name is never called.
   *
   * If @ref actual_call() is invoked with this name, the test fails
   * immediately.
   *
   * @param function_name  Function that must not be called.
   */
  virtual void expect_no_call(StringView function_name);

  /**
   * @brief Expect exactly @p amount calls to @p function_name.
   *
   * @param amount         Number of times the function should be called.
   * @param function_name  Name of the function.
   * @return A fluent @ref ExpectedCall for chaining parameter/return
   * constraints.
   */
  virtual ExpectedCall& expect_n_calls(
      unsigned int amount,
      StringView function_name
  );

  /**
   * @brief Report that @p function_name was actually called.
   *
   * Call this from inside a mock function implementation, then chain
   * with_parameter() for each argument. Retrieve the configured return value
   * with the appropriate @c return_*_value() method at the end.
   *
   * @param function_name  Name of the function being called.
   * @return A fluent @ref ActualCall for supplying parameters and reading the
   *         return value.
   */
  virtual ActualCall& actual_call(const char* function_name);

  /** @return true if the last @ref actual_call() has an associated return
   * value. */
  virtual bool has_return_value();

  /** @return The return value of the last @ref actual_call() as a NamedValue.
   */
  virtual NamedValue return_value();

  /**
   * @brief Type-safe return-value accessor template.
   *
   * @tparam T  The type to retrieve.
   * @return The stored return value converted to @p T.
   */
  template<typename T>
  T return_value()
  {
    return return_value().get_value<T>();
  }

  /**
   * @brief Type-safe return-value accessor with a fallback default.
   *
   * @tparam T  The type to retrieve.
   * @param default_value  Value returned when no return value was configured.
   * @return The stored return value or @p default_value.
   */
  template<typename T>
  T return_value_or_default(T default_value)
  {
    if (has_return_value())
      return return_value().get_value<T>();
    return default_value;
  }

  /**
   * @brief Check whether named data exists in this scope.
   *
   * @param name  Data key.
   * @return true if set_data() was previously called with @p name.
   */
  bool has_data(StringView name);

  /**
   * @brief Store a bool value under @p name for retrieval across mock calls.
   *
   * @param name Key.
   * @param value Value.
   */
  void set_data(StringView name, bool value);

  /**
   * @brief Store an int value.
   * @param name Key.
   * @param value Value.
   */
  void set_data(StringView name, int value);

  /**
   * @brief Store an unsigned int value.
   * @param name Key.
   * @param value Value.
   */
  void set_data(StringView name, unsigned int value);

  /**
   * @brief Store a long int value.
   * @param name Key.
   * @param value Value.
   */
  void set_data(StringView name, long int value);

  /**
   * @brief Store an unsigned long int value.
   * @param name Key.
   * @param value Value.
   */
  void set_data(StringView name, unsigned long int value);

  /**
   * @brief Store a string value.
   * @param name Key.
   * @param value Value.
   */
  void set_data(StringView name, const char* value);

  /**
   * @brief Store a double value.
   * @param name Key.
   * @param value Value.
   */
  void set_data(StringView name, double value);

  /**
   * @brief Store a void* value.
   * @param name Key.
   * @param value Value.
   */
  void set_data(StringView name, void* value);

  /**
   * @brief Store a const void* value.
   * @param name Key.
   * @param value Value.
   */
  void set_data(StringView name, const void* value);

  /**
   * @brief Store a long long value.
   * @param name Key.
   * @param value Value.
   */
  void set_data(StringView name, long long value);

  /**
   * @brief Store an unsigned long long value.
   * @param name Key.
   * @param value Value.
   */
  void set_data(StringView name, unsigned long long value);

  /**
   * @brief Store a function pointer value.
   * @param name Key.
   * @param value Value.
   */
  void set_data(StringView name, FunctionPointerValue value);

  /**
   * @brief Store a mutable object pointer with a type name.
   *
   * @param name   Data key.
   * @param type   Type name string (used for display on failure).
   * @param value  Pointer to the object.
   */
  void set_data_object(StringView name, StringView type, void* value);

  /**
   * @brief Store a const object pointer with a type name.
   *
   * @param name   Data key.
   * @param type   Type name string.
   * @param value  Const pointer to the object.
   */
  void set_data_const_object(
      StringView name,
      StringView type,
      const void* value
  );

  /**
   * @brief Retrieve a previously stored data value by name.
   *
   * @param name  Data key.
   * @return The stored NamedValue; type is undefined if @p name was not
   * set.
   */
  NamedValue get_data(StringView name);

  /**
   * @brief Get (or create) a named child scope.
   *
   * Equivalent to calling mock("scope") but from an existing @ref Support
   * reference. Useful when you already hold a reference to the global scope.
   *
   * @param name  Scope name.
   * @return Pointer to the child Support scope.
   */
  Support* get_mock_support_scope(StringView name);

  /**
   * @return A C string containing all traced call output, or empty string if
   *         tracing was not enabled.
   */
  const char* get_trace_output();

  /** @brief Disable this scope; all @ref actual_call() invocations become
   * no-ops. */
  virtual void disable();

  /** @brief Re-enable this scope after @ref disable(). */
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
   * All calls to @ref actual_call() are silently accepted without matching an
   * expectation. Useful when testing code that uses a collaborator you
   * do not want to fully specify.
   */
  virtual void ignore_other_calls();

  /**
   * @brief Verify all expected calls were fulfilled; fail the test if not.
   *
   * Typically called in @ref mu::tiny::test::Test::teardown() after all
   * assertions have run. Reports every unsatisfied expectation as a separate
   * test failure.
   */
  virtual void check_expectations();

  /** @return true if there are registered expectations that have not been
   * called yet. */
  virtual bool expected_calls_left();

  /**
   * @brief Remove all expectations, actual calls, data, and child scopes.
   *
   * Call in @ref mu::tiny::test::Test::teardown() after @ref
   * check_expectations() to leave the mock support in a clean state for the
   * next test.
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
   * @param reporter  New standard reporter; pass @c nullptr to restore the
   * default.
   */
  virtual void set_mock_failure_standard_reporter(FailureReporter* reporter);

  /**
   * @brief Set the active failure reporter for the current call chain.
   *
   * @param active_reporter  Reporter to use; may differ from the standard one.
   */
  virtual void set_active_reporter(FailureReporter* active_reporter);

  /** @brief Restore the default comparators-and-copiers repository. */
  virtual void set_default_comparators_and_copiers_repository();

  /**
   * @brief Register a custom comparator for objects of type @p type_name.
   *
   * Required when passing objects via @ref
   * mu::tiny::mock::ExpectedCall::with_parameter_of_type().
   *
   * @param type_name   Type name string, must match the one used in
   * @ref mu::tiny::mock::ExpectedCall::with_parameter_of_type().
   * @param comparator  Comparator whose lifetime must exceed the test.
   */
  virtual void install_comparator(
      StringView type_name,
      NamedValueComparator& comparator
  );

  /**
   * @brief Register a custom copier for objects of type @p type_name.
   *
   * Required when using @ref
   * mu::tiny::mock::ExpectedCall::with_output_parameter_of_type_returning().
   *
   * @param type_name  Type name string.
   * @param copier     Copier whose lifetime must exceed the test.
   */
  virtual void install_copier(StringView type_name, NamedValueCopier& copier);

  /**
   * @brief Install all comparators and copiers from a repository at once.
   *
   * @param repository  Repository containing the comparators and copiers to
   * install.
   */
  virtual void install_comparators_and_copiers(
      const NamedValueComparatorsAndCopiersRepository& repository
  );

  /** @brief Remove all installed comparators and copiers from this scope. */
  virtual void remove_all_comparators_and_copiers();

protected:
  /** @brief Create a child Support scope with the given name. */
  Support* clone(StringView mock_name);

  /** @brief Factory method for the actual-call object (override in tests). */
  virtual CheckedActualCall* create_actual_call();

  /** @brief Record a mock failure via the active failure reporter. */
  virtual void fail_test(Failure& failure);

  /** @brief Increment the assertion-check count in the active Result. */
  void count_check();

private:
  class Impl;
  Impl* impl_;

  unsigned int actual_call_order_{ 0 };
  unsigned int expected_call_order_{ 0 };
  bool strict_ordering_{ false };
  bool ignore_other_calls_{ false };
  bool enabled_{ true };
  const String mock_name_;
  bool tracing_{ false };

  void check_expectations_of_last_actual_call();
  bool was_last_actual_call_fulfilled();
  void fail_test_with_expected_calls_not_fulfilled();
  void fail_test_with_out_of_order_calls();

  NamedValue* retrieve_data_from_store(StringView name);

  Support* get_mock_support(NamedValueListNode* node);

  bool call_is_ignored(StringView function_name);
  bool has_calls_out_of_order();

  String append_scope_to_name(StringView function_name);
};

/**
 * @brief Access the global mock support or a named scope.
 *
 * This is the primary entry point for all mock interaction in test code.
 *
 * @param mock_name                     Scope name; empty string is the global
 * scope.
 * @param failure_reporter_for_this_call  Optional reporter override for this
 * call chain.
 * @return Reference to the Support for the requested scope.
 */
MUTINY_EXPORT Support& mock(
    StringView mock_name = "",
    FailureReporter* failure_reporter_for_this_call = nullptr
);

} // namespace mock
} // namespace tiny
} // namespace mu

#endif
