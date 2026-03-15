#ifndef INCLUDED_CPPUTEST_PLATFORMSPECIFICFUNCTIONS_HPP
#define INCLUDED_CPPUTEST_PLATFORMSPECIFICFUNCTIONS_HPP

#include <stdarg.h>
#include <stddef.h>

/* Jumping operations. They manage their own jump buffers */
int
PlatformSpecificSetJmp(void (*function)(void*), void* data);
void
PlatformSpecificLongJmp();
void
PlatformSpecificRestoreJumpBuffer();

/* Time operations */
extern unsigned long (*GetPlatformSpecificTimeInMillis)();
extern const char* (*GetPlatformSpecificTimeString)();

/* String operations */
int
PlatformSpecificVSNprintf(char* str,
    size_t size,
    const char* format,
    va_list va_args_list);

/* Numeric */
double
PlatformSpecificFabs(double d);
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

void
PlatformSpecificFlush();

/* Random operations */
extern int (*PlatformSpecificRand)();

#endif /* INCLUDED_CPPUTEST_PLATFORMSPECIFICFUNCTIONS_HPP */
