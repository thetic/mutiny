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

#include "mutiny/mock/IntegerTypeTraits.hpp"
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
 *
 * Use @ref with_parameter() to report each argument. The framework compares
 * each parameter against the registered expectation using exact type matching:
 * an @c int argument does not match an @c unsigned @c int expectation even if
 * the numeric value is the same.
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
   * Supported types: @c bool, @c int, @c unsigned @c int, @c long @c int,
   * @c unsigned @c long @c int, @c long @c long, @c unsigned @c long @c long,
   * @c double, @c const @c char*, @c void*, @c void(*)(), @c const @c void*.
   * Each value is compared against the expectation with exact type matching.
   *
   * Call `with_parameter<T>(name, value)` to be explicit about the type, or
   * pass a value of the exact expected type directly.
   *
   * @tparam T     Type of the parameter value.
   * @param name   Parameter name, must match the expectation.
   * @param value  Parameter value.
   * @return *this for chaining.
   */
  template<typename T>
  ActualCall& with_parameter(const String& name, T value)
  {
    return do_with_parameter(name, value);
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
    return with_memory_buffer_parameter(name, value, size);
  }
  /** @overload */
  ActualCall& with_parameter(
      const char* name,
      const unsigned char* value,
      size_t size
  )
  {
    return with_memory_buffer_parameter(name, value, size);
  }

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

  /** Function pointer return type alias for readability. */
  using FunctionPointerReturnValue = void (*)();

  /**
   * @brief Return the configured value as type @p T.
   *
   * Specializations exist for @c bool, @c double, @c const @c char*,
   * @c void*, @c const @c void*, and @c FunctionPointerReturnValue, each
   * delegating to the appropriate typed virtual.
   *
   * For integer types, dispatches to @c return_unsigned_long_long_int_value()
   * for unsigned types and @c return_long_long_int_value() for signed types,
   * then casts to @p T. The wide getters accept all narrower storage widths,
   * making this method portable for fixed-width types (@c uint8_t, @c
   * uint64_t, @c int8_t, @c int64_t, @c size_t, @c ptrdiff_t, @c uintptr_t,
   * @c intptr_t, etc.) regardless of the underlying fundamental type chosen
   * by the platform.
   *
   * @tparam T  Target type.
   * @return The configured return value as @c T.
   */
  template<typename T>
  T return_value_as()
  {
    return do_return_value_as(
        typename detail::IsUnsignedInteger<T>::Tag{}, static_cast<T*>(nullptr)
    );
  }

  /**
   * @brief Return the configured value as @p T, or @p default_value if none
   * is set.
   *
   * @tparam T             Target type.
   * @param default_value  Fallback value.
   * @return The configured return value as @c T, or @p default_value.
   */
  template<typename T>
  T return_value_as_or_default(T default_value)
  {
    return do_return_value_as_or_default(
        default_value, typename detail::IsUnsignedInteger<T>::Tag{}
    );
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

private:
  ActualCall& do_with_parameter(const String& n, bool v)
  {
    return with_bool_parameter(n, v);
  }
  ActualCall& do_with_parameter(const String& n, int v)
  {
    return with_int_parameter(n, v);
  }
  ActualCall& do_with_parameter(const String& n, unsigned int v)
  {
    return with_unsigned_int_parameter(n, v);
  }
  ActualCall& do_with_parameter(const String& n, long int v)
  {
    return with_long_int_parameter(n, v);
  }
  ActualCall& do_with_parameter(const String& n, unsigned long int v)
  {
    return with_unsigned_long_int_parameter(n, v);
  }
  ActualCall& do_with_parameter(const String& n, long long v)
  {
    return with_long_long_int_parameter(n, v);
  }
  ActualCall& do_with_parameter(const String& n, unsigned long long v)
  {
    return with_unsigned_long_long_int_parameter(n, v);
  }
  ActualCall& do_with_parameter(const String& n, double v)
  {
    return with_double_parameter(n, v);
  }
  ActualCall& do_with_parameter(const String& n, const char* v)
  {
    return with_string_parameter(n, v);
  }
  ActualCall& do_with_parameter(const String& n, void* v)
  {
    return with_pointer_parameter(n, v);
  }
  ActualCall& do_with_parameter(const String& n, void (*v)())
  {
    return with_function_pointer_parameter(n, v);
  }
  ActualCall& do_with_parameter(const String& n, const void* v)
  {
    return with_const_pointer_parameter(n, v);
  }

  /**
   * @name return_value_as() tag-dispatch back-ends
   * @{
   */
  template<typename T>
  T do_return_value_as(detail::BoolTag<true>, T*)
  {
    return static_cast<T>(return_unsigned_long_long_int_value());
  }
  template<typename T>
  T do_return_value_as(detail::BoolTag<false>, T*)
  {
    return static_cast<T>(return_long_long_int_value());
  }
  template<typename T>
  T do_return_value_as_or_default(T default_value, detail::BoolTag<true>)
  {
    return static_cast<T>(return_unsigned_long_long_int_value_or_default(
        static_cast<unsigned long long>(default_value)
    ));
  }
  template<typename T>
  T do_return_value_as_or_default(T default_value, detail::BoolTag<false>)
  {
    return static_cast<T>(return_long_long_int_value_or_default(
        static_cast<long long>(default_value)
    ));
  }
  /** @} */

  virtual bool return_bool_value() = 0;
  virtual bool return_bool_value_or_default(bool default_value) = 0;
  virtual unsigned long long return_unsigned_long_long_int_value() = 0;
  virtual unsigned long long return_unsigned_long_long_int_value_or_default(
      unsigned long long default_value
  ) = 0;
  virtual long long return_long_long_int_value() = 0;
  virtual long long return_long_long_int_value_or_default(
      long long default_value
  ) = 0;
  virtual const char* return_string_value() = 0;
  virtual const char* return_string_value_or_default(
      const char* default_value
  ) = 0;
  virtual double return_double_value() = 0;
  virtual double return_double_value_or_default(double default_value) = 0;
  virtual void* return_pointer_value() = 0;
  virtual void* return_pointer_value_or_default(void* default_value) = 0;
  virtual const void* return_const_pointer_value() = 0;
  virtual const void* return_const_pointer_value_or_default(
      const void* default_value
  ) = 0;
  virtual FunctionPointerReturnValue return_function_pointer_value() = 0;
  virtual FunctionPointerReturnValue return_function_pointer_value_or_default(
      void (*default_value)()
  ) = 0;

  virtual ActualCall& with_bool_parameter(const String& name, bool value) = 0;
  virtual ActualCall& with_int_parameter(const String& name, int value) = 0;
  virtual ActualCall& with_unsigned_int_parameter(
      const String& name,
      unsigned int value
  ) = 0;
  virtual ActualCall& with_long_int_parameter(
      const String& name,
      long int value
  ) = 0;
  virtual ActualCall& with_unsigned_long_int_parameter(
      const String& name,
      unsigned long int value
  ) = 0;
  virtual ActualCall& with_long_long_int_parameter(
      const String& name,
      long long value
  ) = 0;
  virtual ActualCall& with_unsigned_long_long_int_parameter(
      const String& name,
      unsigned long long value
  ) = 0;
  virtual ActualCall& with_double_parameter(
      const String& name,
      double value
  ) = 0;
  virtual ActualCall& with_string_parameter(
      const String& name,
      const char* value
  ) = 0;
  virtual ActualCall& with_pointer_parameter(
      const String& name,
      void* value
  ) = 0;
  virtual ActualCall& with_function_pointer_parameter(
      const String& name,
      void (*value)()
  ) = 0;
  virtual ActualCall& with_const_pointer_parameter(
      const String& name,
      const void* value
  ) = 0;
  virtual ActualCall& with_memory_buffer_parameter(
      const String& name,
      const unsigned char* value,
      size_t size
  ) = 0;
  virtual ActualCall& with_memory_buffer_parameter(
      const char* name,
      const unsigned char* value,
      size_t size
  ) = 0;
};

} // namespace mock
} // namespace tiny
} // namespace mu

template<>
inline bool mu::tiny::mock::ActualCall::return_value_as<bool>()
{
  return return_bool_value();
}

template<>
inline bool mu::tiny::mock::ActualCall::return_value_as_or_default<bool>(
    bool default_value
)
{
  return return_bool_value_or_default(default_value);
}

template<>
inline double mu::tiny::mock::ActualCall::return_value_as<double>()
{
  return return_double_value();
}

template<>
inline double mu::tiny::mock::ActualCall::return_value_as_or_default<double>(
    double default_value
)
{
  return return_double_value_or_default(default_value);
}

template<>
inline const char* mu::tiny::mock::ActualCall::return_value_as<const char*>()
{
  return return_string_value();
}

template<>
inline const char* mu::tiny::mock::ActualCall::return_value_as_or_default<
    const char*>(const char* default_value)
{
  return return_string_value_or_default(default_value);
}

template<>
inline void* mu::tiny::mock::ActualCall::return_value_as<void*>()
{
  return return_pointer_value();
}

template<>
inline void* mu::tiny::mock::ActualCall::return_value_as_or_default<void*>(
    void* default_value
)
{
  return return_pointer_value_or_default(default_value);
}

template<>
inline const void* mu::tiny::mock::ActualCall::return_value_as<const void*>()
{
  return return_const_pointer_value();
}

template<>
inline const void* mu::tiny::mock::ActualCall::return_value_as_or_default<
    const void*>(const void* default_value)
{
  return return_const_pointer_value_or_default(default_value);
}

template<>
inline mu::tiny::mock::ActualCall::FunctionPointerReturnValue mu::tiny::mock::
    ActualCall::return_value_as<
        mu::tiny::mock::ActualCall::FunctionPointerReturnValue>()
{
  return return_function_pointer_value();
}

template<>
inline mu::tiny::mock::ActualCall::FunctionPointerReturnValue mu::tiny::mock::
    ActualCall::return_value_as_or_default<
        mu::tiny::mock::ActualCall::FunctionPointerReturnValue>(
        FunctionPointerReturnValue default_value
    )
{
  return return_function_pointer_value_or_default(default_value);
}

#endif
