/* Un-comment to use buffer instead of std out */
// #define USE_BUFFER_OUTPUT 1
#include "CppUTest/PlatformSpecificFunctions.h"

#include "CppUTest/TestHarness.hpp"

#include <cstdlib>

#define far // eliminate "meaningless type qualifier" warning
#include <ctype.h>
#include <math.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static jmp_buf test_exit_jmp_buf[10];
static int jmp_buf_index = 0;

static int
DosSetJmp(void (*function)(void* data), void* data)
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
DosLongJmp()
{
  jmp_buf_index--;
  longjmp(test_exit_jmp_buf[jmp_buf_index], 1);
}

static void
DosRestoreJumpBuffer()
{
  jmp_buf_index--;
}

int (*PlatformSpecificSetJmp)(void (*function)(void*), void*) = DosSetJmp;
void (*PlatformSpecificLongJmp)(void) = DosLongJmp;
void (*PlatformSpecificRestoreJumpBuffer)(void) = DosRestoreJumpBuffer;

static unsigned long
DosTimeInMillis()
{
  return (unsigned long)(clock() * 1000 / CLOCKS_PER_SEC);
}

static const char*
DosTimeString()
{
  time_t tm = time(NULL);
  return ctime(&tm);
}

static int
DosVSNprintf(char* str, size_t size, const char* format, va_list args)
{
  return vsnprintf(str, size, format, args);
}

unsigned long (*GetPlatformSpecificTimeInMillis)() = DosTimeInMillis;
const char* (*GetPlatformSpecificTimeString)() = DosTimeString;
int (*PlatformSpecificVSNprintf)(char*,
    size_t,
    const char*,
    va_list) = DosVSNprintf;

PlatformSpecificFile
DosFOpen(const char* filename, const char* flag)
{
  return fopen(filename, flag);
}

static void
DosFPuts(const char* str, PlatformSpecificFile file)
{
  fputs(str, (FILE*)file);
}

static void
DosFClose(PlatformSpecificFile file)
{
  fclose((FILE*)file);
}

PlatformSpecificFile PlatformSpecificStdOut = stdout;
PlatformSpecificFile (
    *PlatformSpecificFOpen)(const char* filename, const char* flag) = DosFOpen;
void (*PlatformSpecificFPuts)(const char* str,
    PlatformSpecificFile file) = DosFPuts;
void (*PlatformSpecificFClose)(PlatformSpecificFile file) = DosFClose;

static void
DosFlush()
{
  fflush(stdout);
}

void (*PlatformSpecificFlush)(void) = DosFlush;

static void*
DosMalloc(size_t size)
{
  return malloc(size);
}

static void*
DosRealloc(void* memory, size_t size)
{
  return realloc(memory, size);
}

static void
DosFree(void* memory)
{
  free(memory);
}

static void*
DosMemCpy(void* s1, const void* s2, size_t size)
{
  return memcpy(s1, s2, size);
}

static void*
DosMemset(void* mem, int c, size_t size)
{
  return memset(mem, c, size);
}

void* (*PlatformSpecificMalloc)(size_t size) = DosMalloc;
void* (*PlatformSpecificRealloc)(void* memory, size_t size) = DosRealloc;
void (*PlatformSpecificFree)(void* memory) = DosFree;
void* (
    *PlatformSpecificMemCpy)(void* s1, const void* s2, size_t size) = DosMemCpy;
void* (*PlatformSpecificMemset)(void* mem, int c, size_t size) = DosMemset;

static void
DosSrand(unsigned int seed)
{
  srand(seed);
}

static int
DosRand()
{
  return rand();
}

static double
DosFabs(double d)
{
  return fabs(d);
}

static int
DosIsNan(double d)
{
  return isnan(d);
}

static int
DosIsInf(double d)
{
  return isinf(d);
}

void (*PlatformSpecificSrand)(unsigned int) = DosSrand;
int (*PlatformSpecificRand)(void) = DosRand;
double (*PlatformSpecificFabs)(double) = DosFabs;
int (*PlatformSpecificIsNan)(double d) = DosIsNan;
int (*PlatformSpecificIsInf)(double d) = DosIsInf;

static void
DosAbort()
{
  abort();
}

void (*PlatformSpecificAbort)(void) = DosAbort;
