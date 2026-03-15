#include "CppUTest/PlatformSpecificFunctions.h"

#include "CppUTest/TestHarness.hpp"

#include <ctype.h>
#include <math.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
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
  time_t tm = time(NULL);
  char* pTimeStr = ctime(&tm);
  char* newlineChar =
      strchr(pTimeStr, '\n'); // Find the terminating newline character.
  if (newlineChar != NULL)
    *newlineChar =
        '\0'; // If newline is found replace it with the string terminator.
  return (pTimeStr);
}

PlatformSpecificFile
PlatformSpecificFOpenImplementation(const char* filename, const char* flag)
{
  static int fileNo = 0;
  (void)filename;
  (void)flag;
  fileNo++;
  return (void*)fileNo;
}

void
PlatformSpecificFPutsImplementation(const char* str, PlatformSpecificFile file)
{
  (void)str;
  (void)file;
  printf("FILE%d:%s", (int)file, str);
}

void
PlatformSpecificFCloseImplementation(PlatformSpecificFile file)
{
  (void)file;
}

void
PlatformSpecificFlushImplementation()
{
}

int
IsNanImplementation(double d)
{
  return isnan(d);
}

int
IsInfImplementation(double d)
{
  return isinf(d);
}

}

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
    va_list args) = vsnprintf;

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
int (*PlatformSpecificIsNan)(double) = IsNanImplementation;
int (*PlatformSpecificIsInf)(double) = IsInfImplementation;
int (*PlatformSpecificAtExit)(
    void (*func)(void)) = atexit; /// this was undefined before

void (*PlatformSpecificSrand)(unsigned int) = srand;
int (*PlatformSpecificRand)(void) = rand;
void (*PlatformSpecificAbort)(void) = abort;
