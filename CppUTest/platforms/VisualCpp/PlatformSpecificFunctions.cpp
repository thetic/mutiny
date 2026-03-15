#include "CppUTest/PlatformSpecificFunctions.h"

#include "CppUTest/TestHarness.hpp"

#include <Windows.h>
#include <float.h>
#include <math.h>
#include <mmsystem.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(__STDC_LIB_EXT1__) || defined(__STDC_SECURE_LIB__)
#define MAYBE_SECURE_FOPEN(fp, filename, flag) fopen_s((fp), (filename), (flag))
#define MAYBE_SECURE_VSNPRINTF(str, size, trunc, format, args)                 \
  _vsnprintf_s((str), (size), (trunc), (format), (args))
#define MAYBE_SECURE_LOCALTIME(_tm, timer) localtime_s((_tm), (timer))
#else
#define MAYBE_SECURE_FOPEN(fp, filename, flag) *(fp) = fopen((filename), (flag))
#define MAYBE_SECURE_VSNPRINTF(str, size, trunc, format, args)                 \
  _vsnprintf((str), (size), (format), (args))
#define MAYBE_SECURE_LOCALTIME(_tm, timer)                                     \
  memcpy(_tm, localtime(timer), sizeof(tm));
#endif

namespace {

jmp_buf test_exit_jmp_buf[10];
int jmp_buf_index = 0;

int
VisualCppSetJmp(void (*function)(void* data), void* data)
{
  if (0 == setjmp(test_exit_jmp_buf[jmp_buf_index])) {
    jmp_buf_index++;
    function(data);
    jmp_buf_index--;
    return 1;
  }
  return 0;
}

[[noreturn]] void
VisualCppLongJmp()
{
  jmp_buf_index--;
  longjmp(test_exit_jmp_buf[jmp_buf_index], 1);
}

void
VisualCppRestoreJumpBuffer()
{
  jmp_buf_index--;
}

unsigned long
VisualCppTimeInMillis()
{
  static LARGE_INTEGER s_frequency;
  static const BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
  if (s_use_qpc) {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (unsigned long)((now.QuadPart * 1000) / s_frequency.QuadPart);
  } else {
#ifdef TIMERR_NOERROR
    return (unsigned long)timeGetTime();
#else
#if !defined(_WIN32_WINNT) || !defined(_WIN32_WINNT_VISTA) ||                  \
    (_WIN32_WINNT < _WIN32_WINNT_VISTA)
    return (unsigned long)GetTickCount();
#else
    return (unsigned long)GetTickCount64();
#endif
#endif
  }
}

const char*
VisualCppTimeString()
{
  time_t the_time = time(nullptr);
  struct tm the_local_time;
  static char dateTime[80];
  MAYBE_SECURE_LOCALTIME(&the_local_time, &the_time);
  strftime(dateTime, 80, "%Y-%m-%dT%H:%M:%S", &the_local_time);
  return dateTime;
}

int
VisualCppVSNprintf(char* str, size_t size, const char* format, va_list args)
{
  char* buf = nullptr;
  size_t sizeGuess = size;

  int result = MAYBE_SECURE_VSNPRINTF(str, size, _TRUNCATE, format, args);
  str[size - 1] = 0;
  while (result == -1) {
    if (buf)
      free(buf);
    sizeGuess += 10;
    buf = (char*)malloc(sizeGuess);
    result = MAYBE_SECURE_VSNPRINTF(buf, sizeGuess, _TRUNCATE, format, args);
  }

  if (buf)
    free(buf);
  return result;
}

PlatformSpecificFile
VisualCppFOpen(const char* filename, const char* flag)
{
  FILE* file;
  MAYBE_SECURE_FOPEN(&file, filename, flag);
  return file;
}

void
VisualCppFPuts(const char* str, PlatformSpecificFile file)
{
  fputs(str, (FILE*)file);
}

void
VisualCppFClose(PlatformSpecificFile file)
{
  fclose((FILE*)file);
}

void
VisualCppFlush()
{
  fflush(stdout);
}

void*
VisualCppMalloc(size_t size)
{
  return malloc(size);
}

void*
VisualCppReAlloc(void* memory, size_t size)
{
  return realloc(memory, size);
}

void
VisualCppFree(void* memory)
{
  free(memory);
}

int
IsInfImplementation(double d)
{
  return !_finite(d);
}

} // namespace

int (*PlatformSpecificSetJmp)(void (*function)(void*),
    void* data) = VisualCppSetJmp;
void (*PlatformSpecificLongJmp)(void) = VisualCppLongJmp;
void (*PlatformSpecificRestoreJumpBuffer)(void) = VisualCppRestoreJumpBuffer;

unsigned long (*GetPlatformSpecificTimeInMillis)() = VisualCppTimeInMillis;

const char* (*GetPlatformSpecificTimeString)() = VisualCppTimeString;

int (*PlatformSpecificVSNprintf)(char* str,
    size_t size,
    const char* format,
    va_list va_args_list) = VisualCppVSNprintf;

PlatformSpecificFile PlatformSpecificStdOut = stdout;
PlatformSpecificFile (*PlatformSpecificFOpen)(const char* filename,
    const char* flag) = VisualCppFOpen;
void (*PlatformSpecificFPuts)(const char* str,
    PlatformSpecificFile file) = VisualCppFPuts;
void (*PlatformSpecificFClose)(PlatformSpecificFile file) = VisualCppFClose;

void (*PlatformSpecificFlush)(void) = VisualCppFlush;

void (*PlatformSpecificSrand)(unsigned int) = srand;
int (*PlatformSpecificRand)(void) = rand;
void* (*PlatformSpecificMalloc)(size_t size) = VisualCppMalloc;
void* (*PlatformSpecificRealloc)(void* memory, size_t size) = VisualCppReAlloc;
void (*PlatformSpecificFree)(void* memory) = VisualCppFree;
void* (*PlatformSpecificMemCpy)(void* s1, const void* s2, size_t size) = memcpy;
void* (*PlatformSpecificMemset)(void* mem, int c, size_t size) = memset;

double (*PlatformSpecificFabs)(double d) = fabs;
int (*PlatformSpecificIsNan)(double) = _isnan;
int (*PlatformSpecificIsInf)(double) = IsInfImplementation;
int (*PlatformSpecificAtExit)(void (*func)(void)) = atexit;

void (*PlatformSpecificAbort)(void) = abort;
