/**
 * @file
 * @brief Fluent interface returned by @ref
 * mu::tiny::mock::Support::actual_call().
 *
 * Inside a mock function implementation, call @ref
 * mu::tiny::mock::Support::actual_call() and then chain @ref
 * mu::tiny::mock::ActualCall::with_parameter() for each argument before
 * querying the return value. The framework matches the actual call against the
 * registered expectations and fails the test if a mismatch is found. See also
 * @ref mu::tiny::mock::ExpectedCall.
 */

#ifndef INCLUDED_MUTINY_MOCK_ACTUALCALL_HPP
#define INCLUDED_MUTINY_MOCK_ACTUALCALL_HPP

#include "mutiny/mock/NamedValue.hpp"

#include "mutiny/export.h"

namespace mu {
namespace tiny {
namespace mock {

class FailureReporter;

/**
 * @brief Abstract interface for reporting an actual call and its parameters.
 *
 * Each method returns @c *this so that parameter reporting can be chained.
 * Concrete implementations are created internally by Support.
 */
class MUTINY_EXPORT ActualCall
{
public:
  ActualCall() = default;
  virtual ~ActualCall() = default;

  /** @brief Report the function name for this call (used internally). @param
   * name Function name. @return *this. */
  virtual ActualCall& with_name(const String& name) = 0;
  /** @brief Set the call order index for strict-ordering checks. @param
   * call_order Order index. @return *this. */
  virtual ActualCall& with_call_order(unsigned int call_order) = 0;

  /**
   * @brief Report a parameter of any supported type.
   *
   * Overloads for bool, int, unsigned int, long int, unsigned long int,
   * long long, unsigned long long, double, const char*, void*, void(*)(),
   * const void*, and (const unsigned char*, size_t) memory buffers are
   * provided. Each creates a NamedValue and passes it to the single
   * with_typed_parameter() virtual method.
   *
   * @param name   Parameter name, must match the expectation.
   * @param value  Parameter value.
   * @return *this for chaining.
   */
  ActualCall& with_parameter(const String& name, bool value)
  {
    NamedValue nv(name);
    nv.set_value(value);
    return with_typed_parameter(static_cast<NamedValue&&>(nv));
  }
  /** @copydoc with_parameter(const String&, bool) */
  ActualCall& with_parameter(const String& name, int value)
  {
    NamedValue nv(name);
    nv.set_value(value);
    return with_typed_parameter(static_cast<NamedValue&&>(nv));
  }
  /** @copydoc with_parameter(const String&, bool) */
  ActualCall& with_parameter(const String& name, unsigned int value)
  {
    NamedValue nv(name);
    nv.set_value(value);
    return with_typed_parameter(static_cast<NamedValue&&>(nv));
  }
  /** @copydoc with_parameter(const String&, bool) */
  ActualCall& with_parameter(const String& name, long int value)
  {
    NamedValue nv(name);
    nv.set_value(value);
    return with_typed_parameter(static_cast<NamedValue&&>(nv));
  }
  /** @copydoc with_parameter(const String&, bool) */
  ActualCall& with_parameter(const String& name, unsigned long int value)
  {
    NamedValue nv(name);
    nv.set_value(value);
    return with_typed_parameter(static_cast<NamedValue&&>(nv));
  }
  /** @copydoc with_parameter(const String&, bool) */
  ActualCall& with_parameter(const String& name, long long value)
  {
    NamedValue nv(name);
    nv.set_value(value);
    return with_typed_parameter(static_cast<NamedValue&&>(nv));
  }
  /** @copydoc with_parameter(const String&, bool) */
  ActualCall& with_parameter(const String& name, unsigned long long value)
  {
    NamedValue nv(name);
    nv.set_value(value);
    return with_typed_parameter(static_cast<NamedValue&&>(nv));
  }
  /** @copydoc with_parameter(const String&, bool) */
  ActualCall& with_parameter(const String& name, double value)
  {
    NamedValue nv(name);
    nv.set_value(value);
    return with_typed_parameter(static_cast<NamedValue&&>(nv));
  }
  /** @copydoc with_parameter(const String&, bool) */
  ActualCall& with_parameter(const String& name, const char* value)
  {
    NamedValue nv(name);
    nv.set_value(value);
    return with_typed_parameter(static_cast<NamedValue&&>(nv));
  }
  /** @copydoc with_parameter(const String&, bool) */
  ActualCall& with_parameter(const String& name, void* value)
  {
    NamedValue nv(name);
    nv.set_value(value);
    return with_typed_parameter(static_cast<NamedValue&&>(nv));
  }
  /** @copydoc with_parameter(const String&, bool) */
  ActualCall& with_parameter(const String& name, void (*value)())
  {
    NamedValue nv(name);
    nv.set_value(value);
    return with_typed_parameter(static_cast<NamedValue&&>(nv));
  }
  /** @copydoc with_parameter(const String&, bool) */
  ActualCall& with_parameter(const String& name, const void* value)
  {
    NamedValue nv(name);
    nv.set_value(value);
    return with_typed_parameter(static_cast<NamedValue&&>(nv));
  }
  /**
   * @brief Report a memory buffer parameter.
   *
   * @param name   Parameter name.
   * @param value  Pointer to the buffer.
   * @param size   Buffer size in bytes.
   * @return *this for chaining.
   */
  ActualCall& with_parameter(
      const String& name,
      const unsigned char* value,
      size_t size
  )
  {
    NamedValue nv(name);
    nv.set_memory_buffer(value, size);
    return with_typed_parameter(static_cast<NamedValue&&>(nv));
  }
  /** @overload */
  ActualCall& with_parameter(
      const char* name,
      const unsigned char* value,
      size_t size
  )
  {
    return with_parameter(String(name), value, size);
  }

  /**
   * @brief Type-erased virtual entry point for parameter reporting.
   *
   * All non-virtual with_parameter() overloads delegate here after
   * constructing a NamedValue. Subclasses override this single method
   * instead of one method per type.
   *
   * @param parameter  Named, typed parameter value.
   * @return *this for chaining.
   */
  virtual ActualCall& with_typed_parameter(NamedValue parameter) = 0;

  /**
   * @brief Report an object parameter identified by a type name string.
   *
   * The mock framework uses the installed @ref NamedValueComparator for
   * @p type_name to compare against the expectation.
   *
   * @param type_name  Type name, must match the one used in the expectation.
   * @param name       Parameter name.
   * @param value      Const pointer to the object.
   * @return *this for chaining.
   */
  virtual ActualCall& with_parameter_of_type(
      const String& type_name,
      const String& name,
      const void* value
  ) = 0;
  /** @overload */
  virtual ActualCall& with_parameter_of_type(
      const char* type_name,
      const char* name,
      const void* value
  ) = 0;

  /**
   * @brief Report an output parameter by name.
   *
   * The framework copies the value configured in the expectation into @p output
   * (using the installed copier if the type is non-native).
   *
   * @param name    Parameter name.
   * @param output  Pointer to the caller's output buffer.
   * @return *this for chaining.
   */
  virtual ActualCall& with_output_parameter(
      const String& name,
      void* output
  ) = 0;

  /**
   * @brief Report an output parameter with a custom object type.
   *
   * @param type_name  Type name; must match the copier installed via
   * @ref Support::install_copier().
   * @param name       Parameter name.
   * @param output     Pointer to the caller's output buffer.
   * @return *this for chaining.
   */
  virtual ActualCall& with_output_parameter_of_type(
      const String& type_name,
      const String& name,
      void* output
  ) = 0;

  /** @return true if the matching expectation set a return value. */
  virtual bool has_return_value() = 0;
  /** @return The configured return value as a generic NamedValue. */
  virtual NamedValue return_value() = 0;

  /**
   * @brief Type-safe return-value accessor template.
   *
   * Returns the mock return value converted to @p T.  Because fixed-width
   * types from @c \<stdint.h\> are typedefs for fundamental types, writing
   * @code return_value<int32_t>() @endcode automatically dispatches to the
   * correct getter on every platform.
   *
   * @tparam T  The type to retrieve.
   * @return The stored return value converted to @p T.
   */
  template <typename T>
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
  template <typename T>
  T return_value_or_default(T default_value)
  {
    if (has_return_value())
      return return_value().get_value<T>();
    return default_value;
  }

  // -- Backward-compatible return-value accessors ---------------------------
  // The no-argument variants return a zero/null/empty default when no return
  // value was configured (matching the previous IgnoredActualCall behavior).

  bool return_bool_value() { return return_value_or_default<bool>(false); }
  bool return_bool_value_or_default(bool default_value)
  {
    return return_value_or_default<bool>(default_value);
  }

  int return_int_value() { return return_value_or_default<int>(0); }
  int return_int_value_or_default(int default_value)
  {
    return return_value_or_default<int>(default_value);
  }

  unsigned int return_unsigned_int_value()
  {
    return return_value_or_default<unsigned int>(0);
  }
  unsigned int return_unsigned_int_value_or_default(
      unsigned int default_value
  )
  {
    return return_value_or_default<unsigned int>(default_value);
  }

  long int return_long_int_value() { return return_value_or_default<long int>(0); }
  /** @return @p default_value when no return value was configured. */
  long int return_long_int_value_or_default(long int default_value)
  {
    return return_value_or_default<long int>(default_value);
  }

  unsigned long int return_unsigned_long_int_value()
  {
    return return_value_or_default<unsigned long int>(0);
  }
  unsigned long int return_unsigned_long_int_value_or_default(
      unsigned long int default_value
  )
  {
    return return_value_or_default<unsigned long int>(default_value);
  }

  long long return_long_long_int_value()
  {
    return return_value_or_default<long long>(0);
  }
  long long return_long_long_int_value_or_default(long long default_value)
  {
    return return_value_or_default<long long>(default_value);
  }

  unsigned long long return_unsigned_long_long_int_value()
  {
    return return_value_or_default<unsigned long long>(0);
  }
  unsigned long long return_unsigned_long_long_int_value_or_default(
      unsigned long long default_value
  )
  {
    return return_value_or_default<unsigned long long>(default_value);
  }

  const char* return_string_value()
  {
    return return_value_or_default<const char*>("");
  }
  const char* return_string_value_or_default(const char* default_value)
  {
    return return_value_or_default<const char*>(default_value);
  }

  double return_double_value()
  {
    return return_value_or_default<double>(0.0);
  }
  double return_double_value_or_default(double default_value)
  {
    return return_value_or_default<double>(default_value);
  }

  void* return_pointer_value()
  {
    return return_value_or_default<void*>(nullptr);
  }
  void* return_pointer_value_or_default(void* default_value)
  {
    return return_value_or_default<void*>(default_value);
  }

  const void* return_const_pointer_value()
  {
    return return_value_or_default<const void*>(nullptr);
  }
  const void* return_const_pointer_value_or_default(
      const void* default_value
  )
  {
    return return_value_or_default<const void*>(default_value);
  }

  /** Function pointer return type alias for readability. */
  using FunctionPointerReturnValue = void (*)();

  FunctionPointerReturnValue return_function_pointer_value()
  {
    return return_value_or_default<FunctionPointerReturnValue>(nullptr);
  }
  FunctionPointerReturnValue return_function_pointer_value_or_default(
      void (*default_value)()
  )
  {
    return return_value_or_default<FunctionPointerReturnValue>(default_value);
  }

  /**
   * @brief Restrict this actual call to a specific object instance.
   *
   * Matches only the expectation set with the same @p object_ptr via
   * ExpectedCall::on_object().
   *
   * @param object_ptr  Pointer to the object on which the method was called.
   * @return *this for chaining.
   */
  virtual ActualCall& on_object(const void* object_ptr) = 0;
};

} // namespace mock
} // namespace tiny
} // namespace mu

#endif
