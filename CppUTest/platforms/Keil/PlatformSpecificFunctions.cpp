#include "CppUTest/PlatformSpecificFunctions.h"

#include "CppUTest/TestHarness.hpp"

#include <stdlib.h>

#define far // eliminate "meaningless type qualifier" warning
#include <ctype.h>
#include <math.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

namespace {

jmp_buf test_exit_jmp_buf[10];
int jmp_buf_index = 0;

int
PlatformSpecificSetJmpImplementation(void (*function)(void* data), void* data)
{
  if (0 == setjmp(test_exit_jmp_buf[jmp_buf_index])) {
    jmp_buf_index++;
    function(data);
    jmp_buf_index--;
    return 1;
  }
  return 0;
}

void
PlatformSpecificLongJmpImplementation()
{
  jmp_buf_index--;
  longjmp(test_exit_jmp_buf[jmp_buf_index], 1);
}

void
PlatformSpecificRestoreJumpBufferImplementation()
{
  jmp_buf_index--;
}

/*
 *  In Keil MDK-ARM, clock() default implementation used semihosting.
 *  Resolutions is user adjustable (1 ms for now)
 */
unsigned long
TimeInMillisImplementation()
{
  clock_t t = clock();

  t = t * 10;

  return (unsigned long)t;
}

const char*
TimeStringImplementation()
{
  time_t tm = 0; // time(NULL); // todo
  return ctime(&tm);
}

PlatformSpecificFile
PlatformSpecificFOpenImplementation(const char* filename, const char* flag)
{
  return 0;
}

void
PlatformSpecificFPutsImplementation(const char* str, PlatformSpecificFile file)
{
  printf("%s", str);
}

void
PlatformSpecificFCloseImplementation(PlatformSpecificFile file)
{
}

void
PlatformSpecificFlushImplementation()
{
}

int
DummyAtExit(void (*)(void))
{
  return 0;
}

int
IsNanImplementation(double d)
{
#ifdef __MICROLIB
  return 0;
#else
  return isnan(d);
#endif
}

int
IsInfImplementation(double d)
{
#ifdef __MICROLIB
  return 0;
#else
  return isinf(d);
#endif
}

} // namespace

void (*PlatformSpecificLongJmp)() = PlatformSpecificLongJmpImplementation;
int (*PlatformSpecificSetJmp)(void (*function)(void*),
    void*) = PlatformSpecificSetJmpImplementation;
void (*PlatformSpecificRestoreJumpBuffer)() =
    PlatformSpecificRestoreJumpBufferImplementation;

unsigned long (*GetPlatformSpecificTimeInMillis)() = TimeInMillisImplementation;

const char* (*GetPlatformSpecificTimeString)() = TimeStringImplementation;

int
PlatformSpecificAtoI(const char* str)
{
  return atoi(str);
}

/* The ARMCC compiler will compile this function with C++ linkage, unless
 * we specifically tell it to use C linkage again, in the function definiton.
 */
int (*PlatformSpecificVSNprintf)(char* str,
    size_t size,
    const char* format,
    va_list args) = vsnprintf;

PlatformSpecificFile PlatformSpecificStdOut = stdout;
PlatformSpecificFile (*PlatformSpecificFOpen)(const char*,
    const char*) = PlatformSpecificFOpenImplementation;
void (*PlatformSpecificFPuts)(const char*,
    PlatformSpecificFile) = PlatformSpecificFPutsImplementation;
void (*PlatformSpecificFClose)(
    PlatformSpecificFile) = PlatformSpecificFCloseImplementation;

void (*PlatformSpecificFlush)() = PlatformSpecificFlushImplementation;
void* (*PlatformSpecificMalloc)(size_t) = malloc;
void* (*PlatformSpecificRealloc)(void*, size_t) = realloc;
void (*PlatformSpecificFree)(void*) = free;
void* (*PlatformSpecificMemCpy)(void* s1, const void* s2, size_t size) = memcpy;
void* (*PlatformSpecificMemset)(void*, int, size_t) = memset;

double (*PlatformSpecificFabs)(double) = abs;
int (*PlatformSpecificIsNan)(double) = IsNanImplementation;
int (*PlatformSpecificIsInf)(double) = IsInfImplementation;
int (*PlatformSpecificAtExit)(void (*func)(void)) = DummyAtExit;

void (*PlatformSpecificAbort)(void) = abort;
