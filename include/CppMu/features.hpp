#ifndef INCLUDED_CPPMU_FEATURES_HPP
#define INCLUDED_CPPMU_FEATURES_HPP

#ifndef CPPMU_HAVE_EXCEPTIONS
#if ((__cplusplus >= 202002L) && !__cpp_exceptions) ||                         \
    (defined(_MSC_VER) && !_CPPUNWIND) ||                                      \
    (defined(__GNUC__) && !__EXCEPTIONS) ||                                    \
    (defined(__ghs__) && !__EXCEPTION_HANDLING) ||                             \
    (defined(__WATCOMC__) && !_CPPUNWIND)
#define CPPMU_HAVE_EXCEPTIONS 0
#else
#define CPPMU_HAVE_EXCEPTIONS 1
#endif
#endif

#ifndef CPPMU_HAVE_RTTI
#if ((__cplusplus >= 202002L) && !__cpp_rtti) ||                               \
    (defined(_MSC_VER) && !_CPPRTTI) || (defined(__GNUC__) && !__GXX_RTTI) ||  \
    (defined(__ghs__) && !__RTTI) || (defined(__WATCOMC__) && !_CPPRTTI)
#define CPPMU_HAVE_RTTI 0
#else
#define CPPMU_HAVE_RTTI 1
#endif
#endif

#endif /* INCLUDED_CPPMU_FEATURES_HPP */
