/**
 * @file
 * @brief Fluent interface returned by @ref
 * mu::tiny::mock::Support::expect_one_call() and friends.
 *
 * Use the fluent methods to constrain which parameter values and call order the
 * actual call must match, and to configure the return value the mock will
 * produce. The corresponding fluent interface for actual calls is
 * @ref mu::tiny::mock::ActualCall.
 */

#ifndef INCLUDED_MUTINY_MOCK_EXPECTEDCALL_HPP
#define INCLUDED_MUTINY_MOCK_EXPECTEDCALL_HPP

#include "mutiny/mock/IntegerTypeTraits.hpp"

#include "mutiny/String.hpp"
#include "mutiny/export.h"

namespace mu {
namespace tiny {
namespace mock {

class NamedValue;

/**
 * @brief Abstract fluent interface for configuring a single call expectation.
 *
 * Each method returns @c *this for chaining. Concrete implementations are
 * created internally by @ref Support. See also @ref ActualCall.
 *
 * Parameter constraints use exact type matching: an @c int expectation will
 * not match an @c unsigned @c int actual parameter even if the value is equal.
 */
class MUTINY_EXPORT ExpectedCall
{
public:
  ExpectedCall() = default;
  virtual ~ExpectedCall() = default;

  /** @brief Set the expected function name. @param name Function name. @return
   * *this. */
  virtual ExpectedCall& with_name(const String& name) = 0;

  /**
   * @brief Require this call to happen at a specific position in call order.
   *
   * @param order  Expected call order index.
   * @return *this for chaining.
   */
  virtual ExpectedCall& with_call_order(unsigned int order) = 0;

  /**
   * @brief Require this call to happen within a range of call order positions.
   *
   * @param min_order  Earliest acceptable order index.
   * @param max_order  Latest acceptable order index.
   * @return *this for chaining.
   */
  virtual ExpectedCall& with_call_order(
      unsigned int min_order,
      unsigned int max_order
  ) = 0;

  /**
   * @brief Constrain a parameter of any supported type.
   *
   * Supported types: @c bool, @c int, @c unsigned @c int, @c long @c int,
   * @c unsigned @c long @c int, @c long @c long, @c unsigned @c long @c long,
   * @c double (with optional tolerance), @c const @c char*, @c void*,
   * @c const @c void*, @c void(*)().
   *
   * Call `with_parameter<T>(name, value)` to be explicit about the type, or
   * pass a value of the exact expected type directly.
   *
   * @tparam T     Type of the parameter value.
   * @param name   Parameter name, must match what the mock implementation
   * reports.
   * @param value  Expected parameter value.
   * @return *this for chaining.
   */
  template<typename T>
  ExpectedCall& with_parameter(const String& name, T value)
  {
    return do_with_parameter(name, value);
  }

  /**
   * @brief Constrain a double parameter with an explicit tolerance.
   *
   * @param name       Parameter name.
   * @param value      Expected double value.
   * @param tolerance  Maximum allowed absolute difference from @p value.
   * @return *this for chaining.
   */
  ExpectedCall& with_parameter(
      const String& name,
      double value,
      double tolerance
  )
  {
    return with_double_parameter(name, value, tolerance);
  }

  /**
   * @brief Constrain a memory buffer parameter.
   *
   * @param name   Parameter name.
   * @param value  Pointer to the expected buffer content.
   * @param size   Number of bytes to compare.
   * @return *this for chaining.
   */
  ExpectedCall& with_parameter(
      const String& name,
      const unsigned char* value,
      size_t size
  )
  {
    return with_memory_buffer_parameter(name, value, size);
  }

  /**
   * @brief Constrain an object parameter identified by a type name string.
   *
   * Uses the NamedValueComparator installed for @p type_name to compare
   * the expected value against the actual call's parameter.
   *
   * @param type_name  Type name; must match the comparator key and the actual
   * call.
   * @param name       Parameter name.
   * @param value      Const pointer to the expected object.
   * @return *this for chaining.
   */
  virtual ExpectedCall& with_parameter_of_type(
      const String& type_name,
      const String& name,
      const void* value
  ) = 0;

  /**
   * @brief Configure an output parameter: the framework will copy @p value into
   *        the caller's buffer when the actual call reports this parameter.
   *
   * @param name   Parameter name.
   * @param value  Pointer to the data to copy out (plain memcpy).
   * @param size   Number of bytes to copy.
   * @return *this for chaining.
   */
  virtual ExpectedCall& with_output_parameter_returning(
      const String& name,
      const void* value,
      size_t size
  ) = 0;

  /**
   * @brief Configure an output parameter for a custom object type.
   *
   * Uses the NamedValueCopier installed for @p type_name to copy the
   * object into the caller's buffer.
   *
   * @param type_name  Type name; must match the copier key.
   * @param name       Parameter name.
   * @param value      Pointer to the expected object to copy out.
   * @return *this for chaining.
   */
  virtual ExpectedCall& with_output_parameter_of_type_returning(
      const String& type_name,
      const String& name,
      const void* value
  ) = 0;

  /**
   * @brief Expect an output parameter to be passed but leave the buffer
   * unmodified.
   *
   * @param name  Parameter name.
   * @return *this for chaining.
   */
  virtual ExpectedCall& with_unmodified_output_parameter(
      const String& name
  ) = 0;

  /**
   * @brief Allow the actual call to pass parameters not listed in this
   * expectation.
   *
   * By default every parameter must be explicitly listed.
   *
   * @return *this for chaining.
   */
  virtual ExpectedCall& ignore_other_parameters() = 0;

  /**
   * @brief Configure the return value the mock will produce.
   *
   * Overloads for @c bool, all integer types, @c double, @c const @c char*,
   * @c void*, @c const @c void*, and @c void(*)() are provided. Call exactly
   * one overload per expectation.
   *
   * For integer types, all widths are accepted and stored as @c long @c long
   * or @c unsigned @c long @c long so that @ref ActualCall::return_value_as()
   * can retrieve the value at the correct width.
   *
   * @param value  Value to return when this expectation is matched.
   * @return *this for chaining.
   */
  virtual ExpectedCall& and_return_value(bool value) = 0;
  /** @copydoc and_return_value(bool) */
  ExpectedCall& and_return_value(int value)
  {
    return and_return_value(static_cast<long long>(value));
  }
  /** @copydoc and_return_value(bool) */
  ExpectedCall& and_return_value(unsigned int value)
  {
    return and_return_value(static_cast<unsigned long long>(value));
  }
  /** @copydoc and_return_value(bool) */
  ExpectedCall& and_return_value(long int value)
  {
    return and_return_value(static_cast<long long>(value));
  }
  /** @copydoc and_return_value(bool) */
  ExpectedCall& and_return_value(unsigned long int value)
  {
    return and_return_value(static_cast<unsigned long long>(value));
  }
  /** @copydoc and_return_value(bool) */
  virtual ExpectedCall& and_return_value(long long value) = 0;
  /** @copydoc and_return_value(bool) */
  virtual ExpectedCall& and_return_value(unsigned long long value) = 0;
  /** @copydoc and_return_value(bool) */
  virtual ExpectedCall& and_return_value(double value) = 0;
  /** @copydoc and_return_value(bool) */
  virtual ExpectedCall& and_return_value(const char* value) = 0;
  /** @copydoc and_return_value(bool) */
  virtual ExpectedCall& and_return_value(void* value) = 0;
  /** @copydoc and_return_value(bool) */
  virtual ExpectedCall& and_return_value(const void* value) = 0;
  /** @copydoc and_return_value(bool) */
  virtual ExpectedCall& and_return_value(void (*value)()) = 0;

  /**
   * @brief Configure the return value as any integral type @p T.
   *
   * Widens signed types to @c long @c long and unsigned types to
   * @c unsigned @c long @c long before storing. Use @ref
   * ActualCall::return_value_as<T>() to retrieve the value at the desired
   * width on the actual-call side.
   *
   * @tparam T     Integral type.
   * @param value  Value to return when this expectation is matched.
   * @return *this for chaining.
   */
  template<typename T>
  ExpectedCall& and_return_value(T value)
  {
    return do_and_return_value(
        value, typename detail::IsUnsignedInteger<T>::Tag{}
    );
  }

  /**
   * @brief Restrict this expectation to calls made on a specific object.
   *
   * Only an actual call that reports the same @p object_ptr via
   * ActualCall::on_object() will match this expectation.
   *
   * @param object_ptr  Pointer to the expected object instance.
   * @return *this for chaining.
   */
  virtual ExpectedCall& on_object(void* object_ptr) = 0;

private:
  /**
   * @name NVI dispatch helpers
   *
   * Selected by the @ref with_parameter() template via overload resolution and
   * forward to the corresponding typed pure virtual. Unsupported types produce
   * a compile-time error.
   * @{
   */
  ExpectedCall& do_with_parameter(const String& n, bool v)
  {
    return with_bool_parameter(n, v);
  }
  ExpectedCall& do_with_parameter(const String& n, int v)
  {
    return with_int_parameter(n, v);
  }
  ExpectedCall& do_with_parameter(const String& n, unsigned int v)
  {
    return with_unsigned_int_parameter(n, v);
  }
  ExpectedCall& do_with_parameter(const String& n, long int v)
  {
    return with_long_int_parameter(n, v);
  }
  ExpectedCall& do_with_parameter(const String& n, unsigned long int v)
  {
    return with_unsigned_long_int_parameter(n, v);
  }
  ExpectedCall& do_with_parameter(const String& n, long long v)
  {
    return with_long_long_int_parameter(n, v);
  }
  ExpectedCall& do_with_parameter(const String& n, unsigned long long v)
  {
    return with_unsigned_long_long_int_parameter(n, v);
  }
  ExpectedCall& do_with_parameter(const String& n, double v)
  {
    return with_double_parameter(n, v);
  }
  ExpectedCall& do_with_parameter(const String& n, const char* v)
  {
    return with_string_parameter(n, v);
  }
  ExpectedCall& do_with_parameter(const String& n, void* v)
  {
    return with_pointer_parameter(n, v);
  }
  ExpectedCall& do_with_parameter(const String& n, void (*v)())
  {
    return with_function_pointer_parameter(n, v);
  }
  ExpectedCall& do_with_parameter(const String& n, const void* v)
  {
    return with_const_pointer_parameter(n, v);
  }
  /** @} */

  /**
   * @name and_return_value() tag-dispatch back-ends
   * @{
   */
  template<typename T>
  ExpectedCall& do_and_return_value(T value, detail::BoolTag<true>)
  {
    return and_return_value(static_cast<unsigned long long>(value));
  }
  template<typename T>
  ExpectedCall& do_and_return_value(T value, detail::BoolTag<false>)
  {
    return and_return_value(static_cast<long long>(value));
  }
  /** @} */

  /**
   * @name Typed pure virtuals (NVI back-end)
   *
   * Override these in concrete subclasses to implement parameter constraining.
   * They are private here so that the only public entry point is
   * @ref with_parameter().
   * @{
   */
  virtual ExpectedCall& with_bool_parameter(const String& name, bool value) = 0;
  virtual ExpectedCall& with_int_parameter(const String& name, int value) = 0;
  virtual ExpectedCall& with_unsigned_int_parameter(
      const String& name,
      unsigned int value
  ) = 0;
  virtual ExpectedCall& with_long_int_parameter(
      const String& name,
      long int value
  ) = 0;
  virtual ExpectedCall& with_unsigned_long_int_parameter(
      const String& name,
      unsigned long int value
  ) = 0;
  virtual ExpectedCall& with_long_long_int_parameter(
      const String& name,
      long long value
  ) = 0;
  virtual ExpectedCall& with_unsigned_long_long_int_parameter(
      const String& name,
      unsigned long long value
  ) = 0;
  virtual ExpectedCall& with_double_parameter(
      const String& name,
      double value
  ) = 0;
  virtual ExpectedCall& with_double_parameter(
      const String& name,
      double value,
      double tolerance
  ) = 0;
  virtual ExpectedCall& with_string_parameter(
      const String& name,
      const char* value
  ) = 0;
  virtual ExpectedCall& with_pointer_parameter(
      const String& name,
      void* value
  ) = 0;
  virtual ExpectedCall& with_function_pointer_parameter(
      const String& name,
      void (*value)()
  ) = 0;
  virtual ExpectedCall& with_const_pointer_parameter(
      const String& name,
      const void* value
  ) = 0;
  virtual ExpectedCall& with_memory_buffer_parameter(
      const String& name,
      const unsigned char* value,
      size_t size
  ) = 0;
  /** @} */
};

} // namespace mock

/**
 * @brief Produce a display string for a NamedValue.
 *
 * Used internally by the failure reporter to format expected vs actual output.
 *
 * @param parameter  Value to convert.
 * @return Human-readable string representation.
 */
String string_from(const mock::NamedValue& parameter);

} // namespace tiny
} // namespace mu

#endif
