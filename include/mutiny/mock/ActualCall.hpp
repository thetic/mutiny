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

namespace detail {

/**
 * @brief Detects whether @p T behaves as an unsigned integer type.
 *
 * @c T(-1) wraps around to the maximum value for unsigned types, which is
 * always greater than @c T(0). For signed types, @c T(-1) equals @c -1,
 * which is less than @c T(0). The check works for all integer types in
 * C++11 without requiring @c \<type_traits\>.
 */
template<typename T>
struct IsUnsignedInteger
{
  enum
  {
    value = (T(-1) > T(0)) ? 1 : 0
  };
};

} // namespace detail

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
   * provided. Each delegates to the corresponding typed with_*_parameter()
   * virtual method.
   *
   * @param name   Parameter name, must match the expectation.
   * @param value  Parameter value.
   * @return *this for chaining.
   */
  ActualCall& with_parameter(const String& name, bool value)
  {
    return with_bool_parameter(name, value);
  }
  /** @copydoc with_parameter(const String&, bool) */
  ActualCall& with_parameter(const String& name, int value)
  {
    return with_int_parameter(name, value);
  }
  /** @copydoc with_parameter(const String&, bool) */
  ActualCall& with_parameter(const String& name, unsigned int value)
  {
    return with_unsigned_int_parameter(name, value);
  }
  /** @copydoc with_parameter(const String&, bool) */
  ActualCall& with_parameter(const String& name, long int value)
  {
    return with_long_int_parameter(name, value);
  }
  /** @copydoc with_parameter(const String&, bool) */
  ActualCall& with_parameter(const String& name, unsigned long int value)
  {
    return with_unsigned_long_int_parameter(name, value);
  }
  /** @copydoc with_parameter(const String&, bool) */
  ActualCall& with_parameter(const String& name, long long value)
  {
    return with_long_long_int_parameter(name, value);
  }
  /** @copydoc with_parameter(const String&, bool) */
  ActualCall& with_parameter(const String& name, unsigned long long value)
  {
    return with_unsigned_long_long_int_parameter(name, value);
  }
  /** @copydoc with_parameter(const String&, bool) */
  ActualCall& with_parameter(const String& name, double value)
  {
    return with_double_parameter(name, value);
  }
  /** @copydoc with_parameter(const String&, bool) */
  ActualCall& with_parameter(const String& name, const char* value)
  {
    return with_string_parameter(name, value);
  }
  /** @copydoc with_parameter(const String&, bool) */
  ActualCall& with_parameter(const String& name, void* value)
  {
    return with_pointer_parameter(name, value);
  }
  /** @copydoc with_parameter(const String&, bool) */
  ActualCall& with_parameter(const String& name, void (*value)())
  {
    return with_function_pointer_parameter(name, value);
  }
  /** @copydoc with_parameter(const String&, bool) */
  ActualCall& with_parameter(const String& name, const void* value)
  {
    return with_const_pointer_parameter(name, value);
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

  /** @brief Report a bool parameter. @param name Name. @param value Value.
   * @return *this. */
  virtual ActualCall& with_bool_parameter(const String& name, bool value) = 0;
  /** @brief Report an int parameter. @param name Name. @param value Value.
   * @return *this. */
  virtual ActualCall& with_int_parameter(const String& name, int value) = 0;
  /** @brief Report an unsigned int parameter. @param name Name. @param value
   * Value. @return *this. */
  virtual ActualCall& with_unsigned_int_parameter(
      const String& name,
      unsigned int value
  ) = 0;
  /** @brief Report a long int parameter. @param name Name. @param value Value.
   * @return *this. */
  virtual ActualCall& with_long_int_parameter(
      const String& name,
      long int value
  ) = 0;
  /** @brief Report an unsigned long int parameter. @param name Name. @param
   * value Value. @return *this. */
  virtual ActualCall& with_unsigned_long_int_parameter(
      const String& name,
      unsigned long int value
  ) = 0;
  /** @brief Report a long long int parameter. @param name Name. @param value
   * Value. @return *this. */
  virtual ActualCall& with_long_long_int_parameter(
      const String& name,
      long long value
  ) = 0;
  /** @brief Report an unsigned long long int parameter. @param name Name.
   * @param value Value. @return *this. */
  virtual ActualCall& with_unsigned_long_long_int_parameter(
      const String& name,
      unsigned long long value
  ) = 0;
  /** @brief Report a double parameter. @param name Name. @param value Value.
   * @return *this. */
  virtual ActualCall& with_double_parameter(
      const String& name,
      double value
  ) = 0;
  /** @brief Report a C string parameter. @param name Name. @param value Value.
   * @return *this. */
  virtual ActualCall& with_string_parameter(
      const String& name,
      const char* value
  ) = 0;
  /** @brief Report a void* parameter. @param name Name. @param value Value.
   * @return *this. */
  virtual ActualCall& with_pointer_parameter(
      const String& name,
      void* value
  ) = 0;
  /** @brief Report a function pointer parameter. @param name Name. @param value
   * Value. @return *this. */
  virtual ActualCall& with_function_pointer_parameter(
      const String& name,
      void (*value)()
  ) = 0;
  /** @brief Report a const void* parameter. @param name Name. @param value
   * Value. @return *this. */
  virtual ActualCall& with_const_pointer_parameter(
      const String& name,
      const void* value
  ) = 0;
  /** @brief Report a memory buffer parameter (String name). @param name Name.
   * @param value Buffer pointer. @param size Buffer size. @return *this. */
  virtual ActualCall& with_memory_buffer_parameter(
      const String& name,
      const unsigned char* value,
      size_t size
  ) = 0;
  /** @brief Report a memory buffer parameter (C string name). @param name Name.
   * @param value Buffer pointer. @param size Buffer size. @return *this. */
  virtual ActualCall& with_memory_buffer_parameter(
      const char* name,
      const unsigned char* value,
      size_t size
  ) = 0;

  /** @return true if the matching expectation set a return value. */
  virtual bool has_return_value() = 0;
  /** @return The configured return value as a generic NamedValue. */
  virtual NamedValue return_value() = 0;

  /**
   * @return The configured return value if set, otherwise @p default_value.
   * @param default_value  Fallback value.
   */
  virtual bool return_bool_value_or_default(bool default_value) = 0;
  /** @return The configured bool return value. */
  virtual bool return_bool_value() = 0;

  /** @return The configured unsigned long long int return value. */
  virtual unsigned long long return_unsigned_long_long_int_value() = 0;
  /**
   * @return The unsigned long long return value if set, otherwise @p
   * default_value.
   * @param default_value  Fallback value.
   */
  virtual unsigned long long return_unsigned_long_long_int_value_or_default(
      unsigned long long default_value
  ) = 0;

  /** @return The configured long long int return value. */
  virtual long long return_long_long_int_value() = 0;
  /**
   * @return The long long return value if set, otherwise @p default_value.
   * @param default_value  Fallback value.
   */
  virtual long long return_long_long_int_value_or_default(
      long long default_value
  ) = 0;

  /**
   * @return The configured string return value if set, otherwise @p
   * default_value.
   * @param default_value  Fallback value.
   */
  virtual const char* return_string_value_or_default(
      const char* default_value
  ) = 0;
  /** @return The configured C string return value. */
  virtual const char* return_string_value() = 0;

  /** @return The configured double return value. */
  virtual double return_double_value() = 0;
  /**
   * @return The configured double return value if set, otherwise @p
   * default_value.
   * @param default_value  Fallback value.
   */
  virtual double return_double_value_or_default(double default_value) = 0;

  /** @return The configured void* return value. */
  virtual void* return_pointer_value() = 0;
  /**
   * @return The configured void* return value if set, otherwise @p
   * default_value.
   * @param default_value  Fallback value.
   */
  virtual void* return_pointer_value_or_default(void* default_value) = 0;

  /** @return The configured const void* return value. */
  virtual const void* return_const_pointer_value() = 0;
  /**
   * @return The configured const void* return value if set, otherwise @p
   * default_value.
   * @param default_value  Fallback value.
   */
  virtual const void* return_const_pointer_value_or_default(
      const void* default_value
  ) = 0;

  /** Function pointer return type alias for readability. */
  using FunctionPointerReturnValue = void (*)();
  /** @return The configured function pointer return value. */
  virtual FunctionPointerReturnValue return_function_pointer_value() = 0;
  /**
   * @return The configured function pointer return value if set, otherwise @p
   * default_value.
   * @param default_value  Fallback value.
   */
  virtual FunctionPointerReturnValue return_function_pointer_value_or_default(
      void (*default_value)()
  ) = 0;

  /**
   * @brief Return the configured value as any integral type @p T.
   *
   * Dispatches to @c return_unsigned_long_long_int_value() for unsigned types
   * and @c return_long_long_int_value() for signed types, then casts to @p T.
   * The wide getters accept all narrower storage widths, making this method
   * portable for fixed-width types (@c uint8_t, @c uint64_t, @c int8_t,
   * @c int64_t, @c size_t, @c ptrdiff_t, @c uintptr_t, @c intptr_t, etc.)
   * regardless of
   * the underlying fundamental type chosen by the platform.
   *
   * @tparam T  Target integral type.
   * @return The configured return value cast to @c T.
   */
  template<typename T>
  T return_value_as()
  {
    if (detail::IsUnsignedInteger<T>::value) {
      return static_cast<T>(return_unsigned_long_long_int_value());
    }
    return static_cast<T>(return_long_long_int_value());
  }

  /**
   * @brief Return the configured value as @p T, or @p default_value if none
   * is set.
   *
   * @tparam T             Target integral type.
   * @param default_value  Fallback value.
   * @return The configured return value cast to @c T, or @p default_value.
   */
  template<typename T>
  T return_value_as_or_default(T default_value)
  {
    if (detail::IsUnsignedInteger<T>::value) {
      return static_cast<T>(return_unsigned_long_long_int_value_or_default(
          static_cast<unsigned long long>(default_value)
      ));
    }
    return static_cast<T>(return_long_long_int_value_or_default(
        static_cast<long long>(default_value)
    ));
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
