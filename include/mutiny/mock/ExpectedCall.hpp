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
   * Overloads for bool, int, unsigned int, long int, unsigned long int,
   * long long, unsigned long long, double (with optional tolerance), const
   * char*, void*, const void*, void(*)(), and (const unsigned char*, size_t)
   * memory buffers are provided. Each delegates to the corresponding typed
   * with_*_parameter() virtual method.
   *
   * @param name   Parameter name, must match what the mock implementation
   * reports.
   * @param value  Expected parameter value.
   * @return *this for chaining.
   */
  ExpectedCall& with_parameter(const String& name, bool value)
  {
    return with_bool_parameter(name, value);
  }
  /** @copydoc with_parameter(const String&, bool) */
  ExpectedCall& with_parameter(const String& name, int value)
  {
    return with_int_parameter(name, value);
  }
  /** @copydoc with_parameter(const String&, bool) */
  ExpectedCall& with_parameter(const String& name, unsigned int value)
  {
    return with_unsigned_int_parameter(name, value);
  }
  /** @copydoc with_parameter(const String&, bool) */
  ExpectedCall& with_parameter(const String& name, long int value)
  {
    return with_long_int_parameter(name, value);
  }
  /** @copydoc with_parameter(const String&, bool) */
  ExpectedCall& with_parameter(const String& name, unsigned long int value)
  {
    return with_unsigned_long_int_parameter(name, value);
  }
  /** @copydoc with_parameter(const String&, bool) */
  ExpectedCall& with_parameter(const String& name, long long value)
  {
    return with_long_long_int_parameter(name, value);
  }
  /** @copydoc with_parameter(const String&, bool) */
  ExpectedCall& with_parameter(const String& name, unsigned long long value)
  {
    return with_unsigned_long_long_int_parameter(name, value);
  }
  /** @copydoc with_parameter(const String&, bool) */
  ExpectedCall& with_parameter(const String& name, double value)
  {
    return with_double_parameter(name, value);
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
  /** @copydoc with_parameter(const String&, bool) */
  ExpectedCall& with_parameter(const String& name, const char* value)
  {
    return with_string_parameter(name, value);
  }
  /** @copydoc with_parameter(const String&, bool) */
  ExpectedCall& with_parameter(const String& name, void* value)
  {
    return with_pointer_parameter(name, value);
  }
  /** @copydoc with_parameter(const String&, bool) */
  ExpectedCall& with_parameter(const String& name, const void* value)
  {
    return with_const_pointer_parameter(name, value);
  }
  /** @copydoc with_parameter(const String&, bool) */
  ExpectedCall& with_parameter(const String& name, void (*value)())
  {
    return with_function_pointer_parameter(name, value);
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

  /** @brief Expect a bool parameter. @param name Name. @param value Value.
   * @return *this. */
  virtual ExpectedCall& with_bool_parameter(const String& name, bool value) = 0;
  /** @brief Expect an int parameter. @param name Name. @param value Value.
   * @return *this. */
  virtual ExpectedCall& with_int_parameter(const String& name, int value) = 0;
  /** @brief Expect an unsigned int parameter. @param name Name. @param value
   * Value. @return *this. */
  virtual ExpectedCall& with_unsigned_int_parameter(
      const String& name,
      unsigned int value
  ) = 0;
  /** @brief Expect a long int parameter. @param name Name. @param value Value.
   * @return *this. */
  virtual ExpectedCall& with_long_int_parameter(
      const String& name,
      long int value
  ) = 0;
  /** @brief Expect an unsigned long int parameter. @param name Name. @param
   * value Value. @return *this. */
  virtual ExpectedCall& with_unsigned_long_int_parameter(
      const String& name,
      unsigned long int value
  ) = 0;
  /** @brief Expect a long long int parameter. @param name Name. @param value
   * Value. @return *this. */
  virtual ExpectedCall& with_long_long_int_parameter(
      const String& name,
      long long value
  ) = 0;
  /** @brief Expect an unsigned long long int parameter. @param name Name.
   * @param value Value. @return *this. */
  virtual ExpectedCall& with_unsigned_long_long_int_parameter(
      const String& name,
      unsigned long long value
  ) = 0;
  /**
   * @brief Expect a double parameter (exact comparison).
   * @param name Name. @param value Expected value. @return *this.
   */
  virtual ExpectedCall& with_double_parameter(
      const String& name,
      double value
  ) = 0;
  /**
   * @brief Expect a double parameter within a tolerance.
   * @param name Name. @param value Expected value. @param tolerance Max
   * difference. @return *this.
   */
  virtual ExpectedCall& with_double_parameter(
      const String& name,
      double value,
      double tolerance
  ) = 0;
  /** @brief Expect a C string parameter. @param name Name. @param value Value.
   * @return *this. */
  virtual ExpectedCall& with_string_parameter(
      const String& name,
      const char* value
  ) = 0;
  /** @brief Expect a void* parameter. @param name Name. @param value Value.
   * @return *this. */
  virtual ExpectedCall& with_pointer_parameter(
      const String& name,
      void* value
  ) = 0;
  /** @brief Expect a function pointer parameter. @param name Name. @param value
   * Value. @return *this. */
  virtual ExpectedCall& with_function_pointer_parameter(
      const String& name,
      void (*value)()
  ) = 0;
  /** @brief Expect a const void* parameter. @param name Name. @param value
   * Value. @return *this. */
  virtual ExpectedCall& with_const_pointer_parameter(
      const String& name,
      const void* value
  ) = 0;
  /** @brief Expect a memory buffer parameter. @param name Name. @param value
   * Buffer. @param size Size in bytes. @return *this. */
  virtual ExpectedCall& with_memory_buffer_parameter(
      const String& name,
      const unsigned char* value,
      size_t size
  ) = 0;

  /**
   * @brief Configure the return value the mock will produce.
   *
   * Overloads for bool, int, unsigned int, long int, unsigned long int, long
   * long, unsigned long long, double, const char*, void*, const void*, and
   * void(*)() are provided. Call exactly one overload per expectation.
   *
   * @param value  Value to return when this expectation is matched.
   * @return *this for chaining.
   */
  virtual ExpectedCall& and_return_value(bool value) = 0;
  /** @copydoc and_return_value(bool) */
  virtual ExpectedCall& and_return_value(int value) = 0;
  /** @copydoc and_return_value(bool) */
  virtual ExpectedCall& and_return_value(unsigned int value) = 0;
  /** @copydoc and_return_value(bool) */
  virtual ExpectedCall& and_return_value(long int value) = 0;
  /** @copydoc and_return_value(bool) */
  virtual ExpectedCall& and_return_value(unsigned long int value) = 0;
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
   * @brief Restrict this expectation to calls made on a specific object.
   *
   * Only an actual call that reports the same @p object_ptr via
   * ActualCall::on_object() will match this expectation.
   *
   * @param object_ptr  Pointer to the expected object instance.
   * @return *this for chaining.
   */
  virtual ExpectedCall& on_object(void* object_ptr) = 0;
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
