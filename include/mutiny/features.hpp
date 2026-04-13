/**
 * @file
 * @brief Compiler feature-detection macros and opt-in build knobs.
 *
 * Defines capability macros by inspecting compiler-specific flags, and
 * provides defaults for build-configuration macros that may be overridden
 * by the build system. All macros can be overridden before including this
 * header.
 */

#ifndef INCLUDED_MUTINY_TEST_FEATURES_HPP
#define INCLUDED_MUTINY_TEST_FEATURES_HPP

/**
 * @def MUTINY_HAVE_EXCEPTIONS
 * @brief Enable use of C++ exceptions.
 *
 * When not defined, the value is deduced from the compiler's predefined macros.
 */
#ifndef MUTINY_HAVE_EXCEPTIONS
#if ((__cplusplus >= 202002L) && !__cpp_exceptions) ||                         \
    (defined(_MSC_VER) && !_CPPUNWIND) ||                                      \
    (defined(__GNUC__) && !__EXCEPTIONS) ||                                    \
    (defined(__ghs__) && !__EXCEPTION_HANDLING) ||                             \
    (defined(__WATCOMC__) && !_CPPUNWIND)
#define MUTINY_HAVE_EXCEPTIONS 0
#else
#define MUTINY_HAVE_EXCEPTIONS 1
#endif
#endif

/**
 * @def MUTINY_HAVE_RTTI
 * @brief Enable use of C++ run-time type information.
 *
 * When not defined, the value is deduced from the compiler's predefined macros.
 */
#ifndef MUTINY_HAVE_RTTI
#if ((__cplusplus >= 202002L) && !__cpp_rtti) ||                               \
    (defined(_MSC_VER) && !_CPPRTTI) || (defined(__GNUC__) && !__GXX_RTTI) ||  \
    (defined(__ghs__) && !__RTTI) || (defined(__WATCOMC__) && !_CPPRTTI)
#define MUTINY_HAVE_RTTI 0
#else
#define MUTINY_HAVE_RTTI 1
#endif
#endif

/**
 * @def MUTINY_USE_STD_CPP_LIB
 * @brief Enable use of the C++ standard library.
 *
 * When set to 1, the framework may use standard library facilities such
 * as `<string>`, `<type_traits>`, and `<stdexcept>`. Set to 0 for
 * bare-metal or other constrained environments where the C++ standard
 * library is unavailable. Defaults to 1.
 */
#ifdef DOXYGEN_DOCUMENTATION_BUILD
#define MUTINY_USE_STD_CPP_LIB 0
#elif !defined(MUTINY_USE_STD_CPP_LIB)
#define MUTINY_USE_STD_CPP_LIB 1
#endif

/**
 * @def MUTINY_USE_STD_STRING
 * @brief Use `std::string` as @ref mu::tiny::String instead of the built-in
 * implementation.
 *
 * When set to 1, @ref mu::tiny::String becomes a type alias for `std::string`
 * and the custom class body in @ref String.hpp is omitted entirely.
 * Requires @ref MUTINY_USE_STD_CPP_LIB.
 */
#ifndef MUTINY_USE_STD_STRING
#define MUTINY_USE_STD_STRING MUTINY_USE_STD_CPP_LIB
#endif

#if MUTINY_USE_STD_STRING && !MUTINY_USE_STD_CPP_LIB
#error MUTINY_USE_STD_STRING requires MUTINY_USE_STD_CPP_LIB
#endif

#endif /* INCLUDED_MUTINY_TEST_FEATURES_HPP */
