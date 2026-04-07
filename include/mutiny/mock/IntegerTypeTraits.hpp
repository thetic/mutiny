/**
 * @file
 * @brief Type traits for integer dispatch in the mock framework.
 *
 * Provides @ref mu::tiny::mock::detail::IsUnsignedInteger, used by the
 * template overloads of @ref mu::tiny::mock::ActualCall::with_parameter(),
 * @ref mu::tiny::mock::ExpectedCall::with_parameter(), and
 * @ref mu::tiny::mock::ExpectedCall::and_return_value() to dispatch
 * signed and unsigned integer types to the appropriate 64-bit virtual.
 */

#ifndef INCLUDED_MUTINY_MOCK_INTEGERTYPETRAITS_HPP
#define INCLUDED_MUTINY_MOCK_INTEGERTYPETRAITS_HPP

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

} // namespace mock
} // namespace tiny
} // namespace mu

#endif
