#include "CppUTest/PlatformSpecificFunctions.hpp"

#include <math.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(_WIN32)
#include <float.h>
#include <windows.h>
#elif defined(CPPUTEST_HAVE_GETTIMEOFDAY)
#include <sys/time.h>
#endif

namespace {

// ── Jump buffer ───────────────────────────────────────────────────────────

jmp_buf test_exit_jmp_buf[10];
int jmp_buf_index = 0;

// ── Time implementations ──────────────────────────────────────────────────

unsigned long
TimeInMillisImpl()
{
#if defined(_WIN32)
  static LARGE_INTEGER s_frequency;
  static const BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
  if (s_use_qpc) {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return static_cast<unsigned long>(
        now.QuadPart * 1000 / s_frequency.QuadPart);
  } else {
    return static_cast<unsigned long>(GetTickCount64());
  }
#elif defined(CPPUTEST_HAVE_GETTIMEOFDAY)
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (static_cast<unsigned long>(tv.tv_sec) * 1000) +
         (static_cast<unsigned long>(tv.tv_usec) / 1000);
#else
  return static_cast<unsigned long>(clock()) * 1000 /
         static_cast<unsigned long>(CLOCKS_PER_SEC);
#endif
}

const char*
TimeStringImpl()
{
  time_t theTime = time(nullptr);
  static char dateTime[80];
#if defined(__STDC_LIB_EXT1__) || defined(__STDC_SECURE_LIB__)
  static struct tm lastlocaltime;
  localtime_s(&lastlocaltime, &theTime);
  struct tm* tmp = &lastlocaltime;
#else
  struct tm* tmp = localtime(&theTime);
#endif
  strftime(dateTime, 80, "%Y-%m-%dT%H:%M:%S", tmp);
  return dateTime;
}

// ── File I/O implementations ──────────────────────────────────────────────

PlatformSpecificFile
FOpenImpl(const char* filename, const char* flag)
{
#if defined(__STDC_LIB_EXT1__) || defined(__STDC_SECURE_LIB__)
  FILE* file;
  fopen_s(&file, filename, flag);
  return file;
#else
  return fopen(filename, flag);
#endif
}

void
FPutsImpl(const char* str, PlatformSpecificFile file)
{
  fputs(str, static_cast<FILE*>(file));
}

void
FCloseImpl(PlatformSpecificFile file)
{
  fclose(static_cast<FILE*>(file));
}

// ── IsNan / IsInf implementations ────────────────────────────────────────

int
IsNanImpl(double d)
{
#if defined(_MSC_VER)
  return _isnan(d);
#else
  return isnan(d);
#endif
}

int
IsInfImpl(double d)
{
#if defined(_MSC_VER)
  return !_finite(d);
#else
  return isinf(d);
#endif
}

} // namespace

// ── Jump buffer (regular functions) ──────────────────────────────────────

int
PlatformSpecificSetJmp(void (*function)(void*), void* data)
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

// ── Time (function pointers, test-overridable) ────────────────────────────

unsigned long (*GetPlatformSpecificTimeInMillis)() = TimeInMillisImpl;
const char* (*GetPlatformSpecificTimeString)() = TimeStringImpl;

// ── VSNprintf (regular function) ──────────────────────────────────────────

int
PlatformSpecificVSNprintf(char* str,
    size_t size,
    const char* format,
    va_list va_args_list)
{
#if defined(_MSC_VER)
  int result = _vsnprintf_s(str, size, _TRUNCATE, format, va_args_list);
  if (result < 0)
    str[size - 1] = '\0';
  return result;
#else
  return vsnprintf(str, size, format, va_args_list);
#endif
}

// ── IsNan / IsInf (function pointers, test-overridable) ──────────────────

int (*PlatformSpecificIsNan)(double) = IsNanImpl;
int (*PlatformSpecificIsInf)(double) = IsInfImpl;

// ── Simple regular functions (no platform variation) ──────────────────────

double
PlatformSpecificFabs(double d)
{
  return fabs(d);
}

void
PlatformSpecificFlush()
{
  fflush(stdout);
}

// ── File I/O (function pointers, test-overridable) ────────────────────────

PlatformSpecificFile PlatformSpecificStdOut = stdout;
PlatformSpecificFile (
    *PlatformSpecificFOpen)(const char*, const char*) = FOpenImpl;
void (*PlatformSpecificFPuts)(const char*, PlatformSpecificFile) = FPutsImpl;
void (*PlatformSpecificFClose)(PlatformSpecificFile) = FCloseImpl;

// ── Rand (function pointer, test-overridable) ─────────────────────────────

int (*PlatformSpecificRand)(void) = rand;
