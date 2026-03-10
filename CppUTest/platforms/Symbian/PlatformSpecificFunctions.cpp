#include "CppUTest/PlatformSpecificFunctions.hpp"

#include "CppUTest/TestHarness.hpp"

#include <sys/time.h>

#include <e32def.h>
#include <e32std.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static jmp_buf test_exit_jmp_buf[10];
static int jmp_buf_index = 0;

int
PlatformSpecificSetJmp(void (*function)(void* data), void* data)
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
PlatformSpecificLongJmp()
{
  jmp_buf_index--;
  longjmp(test_exit_jmp_buf[jmp_buf_index], 1);
}

void
PlatformSpecificRestoreJumpBuffer()
{
  jmp_buf_index--;
}

static unsigned long
TimeInMillisImplementation()
{
  struct timeval tv;
  struct timezone tz;
  ::gettimeofday(&tv, &tz);
  return ((unsigned long)tv.tv_sec * 1000) + ((unsigned long)tv.tv_usec / 1000);
}

unsigned long (*GetPlatformSpecificTimeInMillis)() = TimeInMillisImplementation;

namespace cpputest {
TestOutput::WorkingEnvironment
PlatformSpecificGetWorkingEnvironment()
{
  return TestOutput::eclipse;
}
}

static cpputest::String
TimeStringImplementation()
{
  time_t tm = time(NULL);
  return ctime(&tm);
}

cpputest::String
GetPlatformSpecificTimeString() = TimeStringImplementation;

int
PlatformSpecificVSNprintf(char* str,
    size_t size,
    const char* format,
    va_list args)
{
  return vsnprintf(str, size, format, args);
}

void
PlatformSpecificFlush()
{
  fflush(stdout);
}

double
PlatformSpecificFabs(double d)
{
  return fabs(d);
}

void*
PlatformSpecificMalloc(size_t size)
{
  return malloc(size);
}

void*
PlatformSpecificRealloc(void* memory, size_t size)
{
  return realloc(memory, size);
}

void
PlatformSpecificFree(void* memory)
{
  free(memory);
}

void*
PlatformSpecificMemCpy(void* s1, const void* s2, size_t size)
{
  return memcpy(s1, s2, size);
}

void*
PlatformSpecificMemset(void* mem, int c, size_t size)
{
  return memset(mem, c, size);
}

PlatformSpecificFile PlatformSpecificStdOut = stdout;

PlatformSpecificFile
PlatformSpecificFOpen(const char* filename, const char* flag)
{
  return fopen(filename, flag);
}

void
PlatformSpecificFPuts(const char* str, PlatformSpecificFile file)
{
  fputs(str, (FILE*)file);
}

void
PlatformSpecificFClose(PlatformSpecificFile file)
{
  fclose((FILE*)file);
}

static int
IsNanImplementation(double d)
{
  return isnan(d);
}

static int
IsInfImplementation(double d)
{
  return isinf(d);
}

int (*PlatformSpecificIsNan)(double) = IsNanImplementation;
int (*PlatformSpecificIsInf)(double) = IsInfImplementation;

void (*PlatformSpecificAbort)(void) = abort;
