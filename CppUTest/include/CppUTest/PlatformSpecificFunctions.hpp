#ifndef INCLUDED_CPPUTEST_PLATFORMSPECIFICFUNCTIONS_HPP
#define INCLUDED_CPPUTEST_PLATFORMSPECIFICFUNCTIONS_HPP

#include <stdarg.h>
#include <stddef.h>

/* Time operations */
extern unsigned long (*GetPlatformSpecificTimeInMillis)();
extern const char* (*GetPlatformSpecificTimeString)();

/* Numeric */
extern int (*PlatformSpecificIsNan)(double d);
extern int (*PlatformSpecificIsInf)(double d);

/* IO operations */
using PlatformSpecificFile = void*;

extern PlatformSpecificFile PlatformSpecificStdOut;

extern PlatformSpecificFile (
    *PlatformSpecificFOpen)(const char* filename, const char* flag);
extern void (
    *PlatformSpecificFPuts)(const char* str, PlatformSpecificFile file);
extern void (*PlatformSpecificFClose)(PlatformSpecificFile file);

/* Random operations */
extern int (*PlatformSpecificRand)();

#endif /* INCLUDED_CPPUTEST_PLATFORMSPECIFICFUNCTIONS_HPP */
