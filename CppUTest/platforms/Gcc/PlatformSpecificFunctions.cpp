/*
 * Copyright (c) 2007, Michael Feathers, James Grenning and Bas Vodde
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE EARLIER MENTIONED AUTHORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "CppUTest/PlatformSpecificFunctions.hpp"

#include "CppUTest/TestHarness.hpp"

#include <ctype.h>
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

[[noreturn]]
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
  return ((static_cast<unsigned long>(tv.tv_sec) * 1000) +
          (static_cast<unsigned long>(tv.tv_usec) / 1000));
#else
  return 0;
#endif
}

static const char*
TimeStringImplementation()
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

unsigned long (*GetPlatformSpecificTimeInMillis)() = TimeInMillisImplementation;
const char* (*GetPlatformSpecificTimeString)() = TimeStringImplementation;

/* Wish we could add an attribute to the format for discovering mis-use... but
 * the __attribute__(format) seems to not work on va_list */
#ifdef __clang__
#pragma clang diagnostic ignored "-Wformat-nonliteral"
#endif

#ifdef __clang__
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
#endif
int (*PlatformSpecificVSNprintf)(char* str,
    size_t size,
    const char* format,
    va_list va_args_list) = vsnprintf;

static PlatformSpecificFile
PlatformSpecificFOpenImplementation(const char* filename, const char* flag)
{
#if defined(__STDC_LIB_EXT1__) || defined(__STDC_SECURE_LIB__)
  FILE* file;
  fopen_s(&file, filename, flag);
  return file;
#else
  return fopen(filename, flag);
#endif
}

static void
PlatformSpecificFPutsImplementation(const char* str, PlatformSpecificFile file)
{
  fputs(str, static_cast<FILE*>(file));
}

static void
PlatformSpecificFCloseImplementation(PlatformSpecificFile file)
{
  fclose(static_cast<FILE*>(file));
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

/* GCC 4.9.x introduces -Wfloat-conversion, which causes a warning / error
 * in GCC's own (macro) implementation of isnan() and isinf().
 */
#if defined(__GNUC__) &&                                                       \
    (__GNUC__ >= 5 || (__GNUC__ == 4 && __GNUC_MINOR__ > 8))
#pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

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

double (*PlatformSpecificFabs)(double) = fabs;
void (*PlatformSpecificSrand)(unsigned int) = srand;
int (*PlatformSpecificRand)(void) = rand;
int (*PlatformSpecificIsNan)(double) = IsNanImplementation;
int (*PlatformSpecificIsInf)(double) = IsInfImplementation;
int (*PlatformSpecificAtExit)(
    void (*func)(void)) = atexit; /// this was undefined before

void (*PlatformSpecificAbort)(void) = abort;
