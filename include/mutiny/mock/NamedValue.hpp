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
  NamedValue& operator=(NamedValue&&) noexcept = default;
  virtual ~NamedValue() = default;

  /** @brief Store a bool. @param value Value to store. */
  void set_value(bool value);
  /** @brief Store an int. @param value Value to store. */
  void set_value(int value);
  /** @brief Store an unsigned int. @param value Value to store. */
  void set_value(unsigned int value);
  /** @brief Store a long int. @param value Value to store. */
  void set_value(long int value);
  /** @brief Store an unsigned long int. @param value Value to store. */
  void set_value(unsigned long int value);
  /** @brief Store a long long. @param value Value to store. */
  void set_value(long long value);
  /** @brief Store an unsigned long long. @param value Value to store. */
  void set_value(unsigned long long value);
  /**
   * @brief Store a double with the default tolerance.
   * @param value Value to store.
   */
  void set_value(double value);
  /**
   * @brief Store a double with an explicit tolerance.
   * @param value     Value to store.
   * @param tolerance Maximum allowed difference when comparing.
   */
  void set_value(double value, double tolerance);
  /** @brief Store a void*. @param value Value to store. */
  void set_value(void* value);
  /** @brief Store a const void*. @param value Value to store. */
  void set_value(const void* value);
  /** @brief Store a function pointer. @param value Value to store. */
  void set_value(FunctionPointerValue value);
  /** @brief Store a C string (not copied). @param value Value to store. */
  void set_value(const char* value);
  /**
   * @brief Store a raw memory buffer (not copied).
   *
   * @param value  Pointer to the buffer.
   * @param size   Size in bytes.
   */
  void set_memory_buffer(const unsigned char* value, size_t size);
  /**
   * @brief Store a const object pointer with a type name.
   *
   * @param type        Type name string, matched against comparator keys.
   * @param object_ptr  Const pointer to the object.
   */
  void set_const_object_pointer(const String& type, const void* object_ptr);
  /**
   * @brief Store a mutable object pointer with a type name.
   *
   * @param type        Type name string.
   * @param object_ptr  Pointer to the object.
   */
  void set_object_pointer(const String& type, void* object_ptr);
  /**
   * @brief Set the size field (used for memory buffer parameters).
   *
   * @param size  Size in bytes.
   */
  void set_size(size_t size);

  /**
   * @brief Override the name of this value.
   *
   * @param name  New name.
   */
  void set_name(const char* name);

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
  String get_name() const;
  /** @return The type tag string (e.g. "int", "MyClass", "const char*"). */
  String get_type() const;

  /**
   * @brief Type-safe value accessor template.
   *
   * Explicit specializations are provided for every fundamental type and
   * pointer type supported by the mock framework.  Because fixed-width
   * types from @c \<stdint.h\> are typedefs for fundamental types, writing
   * @code get_value<int32_t>() @endcode automatically dispatches to the
   * correct specialization on every platform.
   *
   * @tparam T  The type to retrieve.
   * @return The stored value converted to @p T.
   */
  template<typename T>
  T get_value() const;

  /**
   * @fn FunctionPointerValue NamedValue::get_value<FunctionPointerValue>()
   * const
   * @brief get_value() specialization for FunctionPointerValue (`void(*)()`).
   */

  /** @return The stored double tolerance. */
  double get_double_tolerance() const;
  /** @return The stored memory buffer pointer. */
  const unsigned char* get_memory_buffer() const;
  /** @return The stored const object pointer. */
  const void* get_const_object_pointer() const;
  /** @return The stored mutable object pointer. */
  void* get_object_pointer() const;
  /** @return true if the stored object pointer is const. */
  bool is_const_object() const;
  /** @return The size field (meaningful for memory buffers). */
  size_t get_size() const;

  /**
   * @return The comparator installed for this value's type, or nullptr if
   *         none was installed (native types do not need one).
   */
  NamedValueComparator* get_comparator() const;

  /**
   * @return The copier installed for this value's type, or nullptr if
   *         none was installed.
   */
  NamedValueCopier* get_copier() const;

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

// Full-width type specializations — declared here, defined in NamedValue.cpp
// (they use STRCMP_EQUAL for type-mismatch diagnostics).
template<>
MUTINY_EXPORT bool NamedValue::get_value<bool>() const;
template<>
MUTINY_EXPORT int NamedValue::get_value<int>() const;
template<>
MUTINY_EXPORT unsigned int NamedValue::get_value<unsigned int>() const;
template<>
MUTINY_EXPORT long int NamedValue::get_value<long int>() const;
template<>
MUTINY_EXPORT unsigned long int NamedValue::get_value<
    unsigned long int>() const;
template<>
MUTINY_EXPORT long long NamedValue::get_value<long long>() const;
template<>
MUTINY_EXPORT unsigned long long NamedValue::get_value<
    unsigned long long>() const;
template<>
MUTINY_EXPORT double NamedValue::get_value<double>() const;
template<>
MUTINY_EXPORT const char* NamedValue::get_value<const char*>() const;
template<>
MUTINY_EXPORT void* NamedValue::get_value<void*>() const;
template<>
MUTINY_EXPORT const void* NamedValue::get_value<const void*>() const;
/// @cond DOXYGEN_SKIP
// Doxygen misclassifies this specialization as namespace-scope due to the
// nested type alias appearing in both the return type and template argument.
// Documented via the explicit \fn comment in the class body above.
template<>
MUTINY_EXPORT NamedValue::FunctionPointerValue NamedValue::get_value<
    NamedValue::FunctionPointerValue>() const;
/// @endcond

// Sub-int type specializations — inline, delegate to the int specialization.
// clang-format off
template <> inline char NamedValue::get_value<char>() const { return static_cast<char>(get_value<int>()); }
template <> inline signed char NamedValue::get_value<signed char>() const { return static_cast<signed char>(get_value<int>()); }
template <> inline unsigned char NamedValue::get_value<unsigned char>() const { return static_cast<unsigned char>(get_value<int>()); }
template <> inline short NamedValue::get_value<short>() const { return static_cast<short>(get_value<int>()); }
template <> inline unsigned short NamedValue::get_value<unsigned short>() const { return static_cast<unsigned short>(get_value<int>()); }
// clang-format on

} // namespace mock
} // namespace tiny
} // namespace mu

#endif
