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
 * @brief Tag type used to select overloads at compile time.
 *
 * @tparam B  Compile-time boolean selector.
 */
template<bool B>
struct BoolTag
{};

/** @brief Tag type for function pointer types in overload dispatch. */
struct FunctionPointerTag
{};

/**
 * @brief Detects whether @p T behaves as an unsigned integer type.
 *
 * @c T(-1) wraps around to the maximum value for unsigned types, which is
 * always greater than @c T(0). For signed types, @c T(-1) equals @c -1,
 * which is less than @c T(0). The check works for all integer types in
 * C++11 without requiring @c \<type_traits\>.
 *
 * The nested @c tag typedef resolves to @c BoolTag<true> for unsigned types
 * and @c BoolTag<false> for signed types, enabling tag dispatch without a
 * constant-condition @c if.
 */
template<typename T>
struct IsUnsignedInteger
{
  enum
  {
    value = (T(-1) > T(0)) ? 1 : 0
  };
  using Tag = BoolTag<(T(-1) > T(0))>;
};

/**
 * @brief Maps a type to the appropriate dispatch tag.
 *
 * Produces @ref FunctionPointerTag for any function pointer type
 * (@c Ret(*)(Args...)), @ref BoolTag<true> for unsigned integer types,
 * and @ref BoolTag<false> for signed integer types.  Used by
 * @ref mu::tiny::mock::ActualCall::return_value_as() and
 * @ref mu::tiny::mock::NamedValue::get_value_as() to dispatch without
 * @c if on compile-time constants.
 *
 * @tparam T  Type to classify.
 */
template<typename T>
struct TypeCategory
{
  using Tag = BoolTag<(T(-1) > T(0))>;
};

/** @cond INTERNAL */
template<typename RET, typename... ARGS>
struct TypeCategory<RET (*)(ARGS...)>
{
  using Tag = FunctionPointerTag;
};
/** @endcond */

} // namespace detail

} // namespace mock
} // namespace tiny
} // namespace mu

#endif
