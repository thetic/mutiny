#include "CppUTest/PlatformSpecificFunctions.hpp"

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

static jmp_buf test_exit_jmp_buf[10];
static int jmp_buf_index = 0;

namespace cpputest {
TestOutput::WorkingEnvironment
PlatformSpecificGetWorkingEnvironment()
{
  return TestOutput::eclipse;
}
}

static int
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

static void
PlatformSpecificLongJmpImplementation()
{
  jmp_buf_index--;
  longjmp(test_exit_jmp_buf[jmp_buf_index], 1);
}

static void
PlatformSpecificRestoreJumpBufferImplementation()
{
  jmp_buf_index--;
}

void (*PlatformSpecificLongJmp)() = PlatformSpecificLongJmpImplementation;
int (*PlatformSpecificSetJmp)(void (*)(void*),
    void*) = PlatformSpecificSetJmpImplementation;
void (*PlatformSpecificRestoreJumpBuffer)() =
    PlatformSpecificRestoreJumpBufferImplementation;

///////////// Time in millis

static unsigned long
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

static const char*
TimeStringImplementation()
{
  time_t theTime = time(nullptr);
  static char dateTime[80];
  struct tm* tmp = localtime(&theTime);
  strftime(dateTime, 80, "%Y-%m-%dT%H:%M:%S", tmp);
  return dateTime;
}

unsigned long (*GetPlatformSpecificTimeInMillis)() = TimeInMillisImplementation;
const char* (*GetPlatformSpecificTimeString)() = TimeStringImplementation;

static int
BorlandVSNprintf(char* str, size_t size, const char* format, va_list args)
{
  int result = vsnprintf(str, size, format, args);
  str[size - 1] = 0;
  return result;
}

int (*PlatformSpecificVSNprintf)(char* str,
    size_t size,
    const char* format,
    va_list va_args_list) = BorlandVSNprintf;

static PlatformSpecificFile
PlatformSpecificFOpenImplementation(const char* filename, const char* flag)
{
  return fopen(filename, flag);
}

static void
PlatformSpecificFPutsImplementation(const char* str, PlatformSpecificFile file)
{
  fputs(str, (FILE*)file);
}

static void
PlatformSpecificFCloseImplementation(PlatformSpecificFile file)
{
  fclose((FILE*)file);
}

static void
PlatformSpecificFlushImplementation()
{
  fflush(stdout);
}

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

static int
IsNanImplementation(double d)
{
  return _isnan(d);
}

static int
IsInfImplementation(double d)
{
  return !(_finite(d) || _isnan(d));
}

double (*PlatformSpecificFabs)(double) = fabs;
void (*PlatformSpecificSrand)(unsigned int) = srand;
int (*PlatformSpecificRand)(void) = rand;
int (*PlatformSpecificIsNan)(double) = IsNanImplementation;
int (*PlatformSpecificIsInf)(double) = IsInfImplementation;
int (*PlatformSpecificAtExit)(
    void (*func)(void)) = atexit; /// this was undefined before

void (*PlatformSpecificAbort)(void) = abort;
