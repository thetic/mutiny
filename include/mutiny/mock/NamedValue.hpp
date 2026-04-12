/**
 * @file
 * @brief Generic named-value container used to store mock parameters and data.
 *
 * @ref mu::tiny::mock::NamedValue ties together a name, a type tag, a value
 * (from a union of all supported native types plus opaque object pointers),
 * and optional @ref mu::tiny::mock::NamedValueComparator / @ref
 * mu::tiny::mock::NamedValueCopier instances for non-native types. Users
 * encounter @ref mu::tiny::mock::NamedValue primarily via @ref
 * mu::tiny::mock::Support::get_data() and the @ref
 * mu::tiny::mock::ActualCall::return_value() methods.
 */

#ifndef INCLUDED_MUTINY_MOCK_NAMEDVALUE_HPP
#define INCLUDED_MUTINY_MOCK_NAMEDVALUE_HPP

#include "mutiny/mock/IntegerTypeTraits.hpp"

#include "mutiny/String.hpp"
#include "mutiny/export.h"

namespace mu {
namespace tiny {
namespace mock {

class NamedValueComparatorsAndCopiersRepository;
class NamedValueComparator;
class NamedValueCopier;

/**
 * @brief A named, typed value used throughout the mock framework.
 *
 * Encapsulates all primitive types and opaque object pointers in a single
 * union so that a heterogeneous collection of parameters can be stored and
 * compared uniformly. Custom types register a @ref NamedValueComparator (for
 * comparison) and a @ref NamedValueCopier (for output parameters) via
 * @ref Support::install_comparator() and @ref Support::install_copier().
 */
class MUTINY_EXPORT NamedValue
{
public:
  /** @brief Type alias for a pointer to a function with signature `void()`. */
  using FunctionPointerValue = void (*)();
  /**
   * @brief Construct a named value with no data set.
   *
   * @param name  The parameter or data key.
   */
  NamedValue(const String& name);
  NamedValue(const NamedValue&) = default;
  NamedValue(NamedValue&&) noexcept;
  virtual ~NamedValue() = default;

  /** @brief Store a bool. @param value Value to store. */
  virtual void set_value(bool value);
  /** @brief Store an int. @param value Value to store. */
  virtual void set_value(int value);
  /** @brief Store an unsigned int. @param value Value to store. */
  virtual void set_value(unsigned int value);
  /** @brief Store a long int. @param value Value to store. */
  virtual void set_value(long int value);
  /** @brief Store an unsigned long int. @param value Value to store. */
  virtual void set_value(unsigned long int value);
  /** @brief Store a long long. @param value Value to store. */
  virtual void set_value(long long value);
  /** @brief Store an unsigned long long. @param value Value to store. */
  virtual void set_value(unsigned long long value);
  /**
   * @brief Store a double with the default tolerance.
   * @param value Value to store.
   */
  virtual void set_value(double value);
  /**
   * @brief Store a double with an explicit tolerance.
   * @param value     Value to store.
   * @param tolerance Maximum allowed difference when comparing.
   */
  virtual void set_value(double value, double tolerance);
  /** @brief Store a void*. @param value Value to store. */
  virtual void set_value(void* value);
  /** @brief Store a const void*. @param value Value to store. */
  virtual void set_value(const void* value);
  /** @brief Store a function pointer. @param value Value to store. */
  virtual void set_value(FunctionPointerValue value);
  /** @brief Store a C string (not copied). @param value Value to store. */
  virtual void set_value(const char* value);
  /**
   * @brief Store a raw memory buffer (not copied).
   *
   * @param value  Pointer to the buffer.
   * @param size   Size in bytes.
   */
  virtual void set_memory_buffer(const unsigned char* value, size_t size);
  /**
   * @brief Store a const object pointer with a type name.
   *
   * @param type        Type name string, matched against comparator keys.
   * @param object_ptr  Const pointer to the object.
   */
  virtual void set_const_object_pointer(
      const String& type,
      const void* object_ptr
  );
  /**
   * @brief Store a mutable object pointer with a type name.
   *
   * @param type        Type name string.
   * @param object_ptr  Pointer to the object.
   */
  virtual void set_object_pointer(const String& type, void* object_ptr);
  /**
   * @brief Set the size field (used for memory buffer parameters).
   *
   * @param size  Size in bytes.
   */
  virtual void set_size(size_t size);

  /**
   * @brief Override the name of this value.
   *
   * @param name  New name.
   */
  virtual void set_name(const char* name);

  /**
   * @brief Compare this value against @p p using the appropriate strategy.
   *
   * Native types are compared by value; object types use the installed
   * NamedValueComparator. Memory buffers are compared byte-by-byte.
   *
   * @param p  Value to compare against.
   * @return true if the values are considered equal.
   */
  virtual bool equals(const NamedValue& p) const;

  /**
   * @brief Check whether @p p can be copied into this value.
   *
   * Used for output-parameter handling: ensures type compatibility before
   * invoking the copier.
   *
   * @param p  Candidate source value.
   * @return true if the types are compatible for copying.
   */
  virtual bool compatible_for_copying(const NamedValue& p) const;

  /**
   * @brief Return a human-readable representation of this value.
   *
   * Used by the failure reporter to produce "expected X but got Y" messages.
   *
   * @return Display string.
   */
  virtual String to_string() const;

  /** @return The name of this value. */
  virtual String get_name() const;
  /** @return The type tag string (e.g. "int", "MyClass", "const char*"). */
  virtual String get_type() const;

  /**
   * @brief Retrieve the stored value as type @p T.
   *
   * For integer types the primary template routes through the widest signed
   * or unsigned 64-bit getter and casts down. Full specialisations handle
   * @c bool, @c double, @c const @c char*, pointer types, and
   * @ref FunctionPointerValue.
   *
   * @tparam T  Target type.
   * @return The stored value converted to @c T.
   */
  template<typename T>
  T get_value_as() const
  {
    return do_get_value_as(
        typename detail::TypeCategory<T>::Tag{}, static_cast<T*>(nullptr)
    );
  }

  /** @return The stored double tolerance. */
  virtual double get_double_tolerance() const;
  /** @return The stored memory buffer pointer. */
  virtual const unsigned char* get_memory_buffer() const;
  /** @return The stored const object pointer. */
  virtual const void* get_const_object_pointer() const;
  /** @return The stored mutable object pointer. */
  virtual void* get_object_pointer() const;
  /** @return true if the stored object pointer is const. */
  virtual bool is_const_object() const;
  /** @return The size field (meaningful for memory buffers). */
  virtual size_t get_size() const;

  /**
   * @return The comparator installed for this value's type, or nullptr if
   *         none was installed (native types do not need one).
   */
  virtual NamedValueComparator* get_comparator() const;

  /**
   * @return The copier installed for this value's type, or nullptr if
   *         none was installed.
   */
  virtual NamedValueCopier* get_copier() const;

  /**
   * @brief Replace the global repository of comparators and copiers.
   *
   * @param repository  New repository; pass nullptr to clear.
   */
  static void set_default_comparators_and_copiers_repository(
      NamedValueComparatorsAndCopiersRepository* repository
  );

  /** @return The current global comparators-and-copiers repository. */
  static NamedValueComparatorsAndCopiersRepository*
  get_default_comparators_and_copiers_repository();

  /**
   * @brief The default tolerance used when set_value(double) is called without
   *        an explicit tolerance argument.
   */
  static const double default_double_tolerance;

private:
  /** @return The stored bool. */
  virtual bool get_bool_value() const;
  /** @return The stored int. */
  virtual int get_int_value() const;
  /** @return The stored unsigned int. */
  virtual unsigned int get_unsigned_int_value() const;
  /** @return The stored long int. */
  virtual long int get_long_int_value() const;
  /** @return The stored unsigned long int. */
  virtual unsigned long int get_unsigned_long_int_value() const;
  /** @return The stored long long. */
  virtual long long get_long_long_int_value() const;
  /** @return The stored unsigned long long. */
  virtual unsigned long long get_unsigned_long_long_int_value() const;
  /** @return The stored double. */
  virtual double get_double_value() const;
  /** @return The stored C string pointer (not a copy). */
  virtual const char* get_string_value() const;
  /** @return The stored void* pointer. */
  virtual void* get_pointer_value() const;
  /** @return The stored const void* pointer. */
  virtual const void* get_const_pointer_value() const;
  /** @return The stored function pointer. */
  virtual FunctionPointerValue get_function_pointer_value() const;

  /**
   * @name get_value_as() tag-dispatch back-ends
   * @{
   */
  template<typename T>
  T do_get_value_as(detail::BoolTag<true>, T*) const
  {
    return static_cast<T>(get_unsigned_long_long_int_value());
  }
  template<typename T>
  T do_get_value_as(detail::BoolTag<false>, T*) const
  {
    return static_cast<T>(get_long_long_int_value());
  }
  template<typename T>
  T do_get_value_as(detail::FunctionPointerTag, T*) const
  {
    return get_function_pointer_value();
  }
  /** @} */

  String name_;
  String type_;
  bool is_const_object_{ false };
  union
  {
    bool bool_value;
    int int_value;
    unsigned int unsigned_int_value;
    long int long_int_value;
    unsigned long int unsigned_long_int_value;
    long long long_long_int_value;
    unsigned long long unsigned_long_long_int_value;
    struct
    {
      double value;
      double tolerance;
    } double_value;
    const char* string_value;
    void* pointer_value;
    const void* const_pointer_value;
    FunctionPointerValue function_pointer_value;
    const unsigned char* memory_buffer_value;
    const void* const_object_pointer_value;
    void* object_pointer_value;
    const void* output_pointer_value;
  } value_;
  size_t size_{ 0 };
  NamedValueComparator* comparator_{ nullptr };
  NamedValueCopier* copier_{ nullptr };
  static NamedValueComparatorsAndCopiersRepository* default_repository_;
};

} // namespace mock
} // namespace tiny
} // namespace mu

template<>
inline bool mu::tiny::mock::NamedValue::get_value_as<bool>() const
{
  return get_bool_value();
}

template<>
inline double mu::tiny::mock::NamedValue::get_value_as<double>() const
{
  return get_double_value();
}

template<>
inline const char* mu::tiny::mock::NamedValue::get_value_as<const char*>() const
{
  return get_string_value();
}

template<>
inline void* mu::tiny::mock::NamedValue::get_value_as<void*>() const
{
  return get_pointer_value();
}

template<>
inline const void* mu::tiny::mock::NamedValue::get_value_as<const void*>() const
{
  return get_const_pointer_value();
}

#endif
