/**
 * @file
 * @brief Interface and helpers for comparing mock parameters of custom types.
 *
 * When a mock expectation is set with @ref
 * mu::tiny::mock::ExpectedCall::with_parameter_of_type(), the framework needs
 * to know how to compare the expected value against the actual value.
 * Implement @ref mu::tiny::mock::NamedValueComparator for your type and
 * register it with @ref mu::tiny::mock::Support::install_comparator().
 *
 * Three concrete implementations are provided:
 * - @ref mu::tiny::mock::FunctionComparator wraps plain function pointers
 * - @ref mu::tiny::mock::TypedMockComparator uses `T::operator==` and
 *   @ref mu::tiny::string_from()
 *
 * See also @ref mu::tiny::mock::NamedValueCopier for output-parameter support.
 */

#ifndef INCLUDED_MU_TINY_MOCK_NAMEDVALUECOMPARATOR_HPP
#define INCLUDED_MU_TINY_MOCK_NAMEDVALUECOMPARATOR_HPP

#include "mu/tiny/String.hpp"
#include "mu/tiny/export.h"

namespace mu {
namespace tiny {
namespace mock {

/**
 * @brief Abstract interface for comparing values of a custom mock parameter
 * type.
 *
 * Implement both pure virtual methods and pass an instance to
 * @ref mu::tiny::mock::Support::install_comparator(). The comparator's lifetime
 * must extend at least as long as the test (or @ref SupportPlugin scope).
 */
class MUTINY_EXPORT NamedValueComparator
{
public:
  NamedValueComparator() = default;
  virtual ~NamedValueComparator() = default;

  /**
   * @brief Return true if @p object1 and @p object2 are equal.
   *
   * Both pointers point to objects of the type associated with this comparator.
   *
   * @param object1  Pointer to the first object (typically the expected value).
   * @param object2  Pointer to the second object (typically the actual value).
   * @return true if the two objects are considered equal.
   */
  virtual bool is_equal(const void* object1, const void* object2) = 0;

  /**
   * @brief Return a human-readable string representation of @p object.
   *
   * Used by the failure reporter to generate "expected X but got Y" output.
   *
   * @param object  Pointer to an object of the associated type.
   * @return Display string.
   */
  virtual String value_to_string(const void* object) = 0;
};

/**
 * @brief NamedValueComparator backed by plain function pointers.
 *
 * Convenient alternative to subclassing when you want to keep comparator
 * logic in standalone functions.
 */
class MUTINY_EXPORT FunctionComparator : public NamedValueComparator
{
public:
  /** Function type for the equality predicate. */
  using IsEqualFunction = bool (*)(const void*, const void*);
  /** Function type for the string-conversion function. */
  using ValueToStringFunction = String (*)(const void*);

  /**
   * @brief Construct from two plain function pointers.
   *
   * @param equal          Equality predicate.
   * @param val_to_string  String-conversion function.
   */
  FunctionComparator(IsEqualFunction equal, ValueToStringFunction val_to_string)
    : equal_(equal)
    , value_to_string_(val_to_string)
  {
  }

  bool is_equal(const void* object1, const void* object2) override
  {
    return equal_(object1, object2);
  }

  String value_to_string(const void* object) override
  {
    return value_to_string_(object);
  }

private:
  IsEqualFunction equal_;
  ValueToStringFunction value_to_string_;
};

/**
 * @brief NamedValueComparator that uses T::operator== and string_from(T).
 *
 * Zero-overhead wrapper for types that already have the right operators.
 * Requires that a string_from() overload exists for @p T.
 *
 * @tparam T  Type to compare; must support operator== and string_from().
 */
template<typename T>
class TypedMockComparator : public NamedValueComparator
{
public:
  bool is_equal(const void* object1, const void* object2) override
  {
    return *static_cast<const T*>(object1) == *static_cast<const T*>(object2);
  }

  String value_to_string(const void* object) override
  {
    return string_from(*static_cast<const T*>(object));
  }
};

} // namespace mock
} // namespace tiny
} // namespace mu

#endif
