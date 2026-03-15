#include "CppUTest/PlatformSpecificFunctions.h"

#include "CppUTest/TestHarness.hpp"

#include <ctype.h>
#include <float.h>
#include <math.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef CPPUTEST_HAVE_GETTIMEOFDAY
#include <sys/time.h>
#endif

const std::nothrow_t std::nothrow;

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

unsigned long
TimeInMillisImplementation()
{
#ifdef CPPUTEST_HAVE_GETTIMEOFDAY
  struct timeval tv;
  gettimeofday(&tv, NULL);
    return ((unsigned long)tv.tv_sec * 1000) + ((unsigned long)tv.tv_usec / 1000));
#else
  return 0;
#endif
}

const char*
TimeStringImplementation()
{
  time_t theTime = time(nullptr);
  static char dateTime[80];
  struct tm* tmp = localtime(&theTime);
  strftime(dateTime, 80, "%Y-%m-%dT%H:%M:%S", tmp);
  return dateTime;
}

int
BorlandVSNprintf(char* str, size_t size, const char* format, va_list args)
{
  int result = vsnprintf(str, size, format, args);
  str[size - 1] = 0;
  return result;
}

PlatformSpecificFile
PlatformSpecificFOpenImplementation(const char* filename, const char* flag)
{
  return fopen(filename, flag);
}

void
PlatformSpecificFPutsImplementation(const char* str, PlatformSpecificFile file)
{
  fputs(str, (FILE*)file);
}

void
PlatformSpecificFCloseImplementation(PlatformSpecificFile file)
{
  fclose((FILE*)file);
}

void
PlatformSpecificFlushImplementation()
{
  fflush(stdout);
}

int
IsNanImplementation(double d)
{
  return _isnan(d);
}

int
IsInfImplementation(double d)
{
  return !(_finite(d) || _isnan(d));
}

} // namespace

void (*PlatformSpecificLongJmp)() = PlatformSpecificLongJmpImplementation;
int (*PlatformSpecificSetJmp)(void (*)(void*),
    void*) = PlatformSpecificSetJmpImplementation;
void (*PlatformSpecificRestoreJumpBuffer)() =
    PlatformSpecificRestoreJumpBufferImplementation;

unsigned long (*GetPlatformSpecificTimeInMillis)() = TimeInMillisImplementation;
const char* (*GetPlatformSpecificTimeString)() = TimeStringImplementation;

int (*PlatformSpecificVSNprintf)(char* str,
    size_t size,
    const char* format,
    va_list va_args_list) = BorlandVSNprintf;

PlatformSpecificFile PlatformSpecificStdOut = stdout;
PlatformSpecificFile (*PlatformSpecificFOpen)(const char*,
    const char*) = PlatformSpecificFOpenImplementation;
void (*PlatformSpecificFPuts)(const char*,
    PlatformSpecificFile) = PlatformSpecificFPutsImplementation;
void (*PlatformSpecificFClose)(
    PlatformSpecificFile) = PlatformSpecificFCloseImplementation;

void (*PlatformSpecificFlush)() = PlatformSpecificFlushImplementation;

void* (*PlatformSpecificMalloc)(size_t size) = malloc;
void* (*PlatformSpecificRealloc)(void*, size_t) = realloc;
void (*PlatformSpecificFree)(void* memory) = free;
void* (*PlatformSpecificMemCpy)(void*, const void*, size_t) = memcpy;
void* (*PlatformSpecificMemset)(void*, int, size_t) = memset;

double (*PlatformSpecificFabs)(double) = fabs;
void (*PlatformSpecificSrand)(unsigned int) = srand;
int (*PlatformSpecificRand)(void) = rand;
int (*PlatformSpecificIsNan)(double) = IsNanImplementation;
int (*PlatformSpecificIsInf)(double) = IsInfImplementation;
int (*PlatformSpecificAtExit)(
    void (*func)(void)) = atexit; /// this was undefined before

void (*PlatformSpecificAbort)(void) = abort;
