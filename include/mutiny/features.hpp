/**
 * @file
 * @brief Compiler feature-detection macros.
 *
 * Defines two capability macros by inspecting compiler-specific flags.
 * Both can be overridden before including this header to force a specific
 * value regardless of the detected compiler state.
 *
 * - **MUTINY_HAVE_EXCEPTIONS** — 1 if C++ exception support is enabled
 *   (`__EXCEPTIONS`, `_CPPUNWIND`, etc.), 0 otherwise.
 * - **MUTINY_HAVE_RTTI** — 1 if C++ RTTI support is enabled
 *   (`__GXX_RTTI`, `_CPPRTTI`, etc.), 0 otherwise.
 */

#ifndef INCLUDED_MUTINY_TEST_FEATURES_HPP
#define INCLUDED_MUTINY_TEST_FEATURES_HPP

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

#ifndef MUTINY_HAVE_RTTI
#if ((__cplusplus >= 202002L) && !__cpp_rtti) ||                               \
    (defined(_MSC_VER) && !_CPPRTTI) || (defined(__GNUC__) && !__GXX_RTTI) ||  \
    (defined(__ghs__) && !__RTTI) || (defined(__WATCOMC__) && !_CPPRTTI)
#define MUTINY_HAVE_RTTI 0
#else
#define MUTINY_HAVE_RTTI 1
#endif
#endif

#endif /* INCLUDED_MUTINY_TEST_FEATURES_HPP */
