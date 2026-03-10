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

#define far // eliminate "meaningless type qualifier" warning

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
/*
 *  In Keil MDK-ARM, clock() default implementation used semihosting.
 *  Resolutions is user adjustable (1 ms for now)
 */
static unsigned long
TimeInMillisImplementation()
{
  clock_t t = clock();
  return (unsigned long)t;
}

///////////// Time in String

static const char*
DummyTimeStringImplementation()
{
  time_t tm = 0;
  return ctime(&tm);
}

unsigned long (*GetPlatformSpecificTimeInMillis)() = TimeInMillisImplementation;
const char* (*GetPlatformSpecificTimeString)() = DummyTimeStringImplementation;

int (*PlatformSpecificVSNprintf)(char* str,
    size_t size,
    const char* format,
    va_list args) = vsnprintf;

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

void (*PlatformSpecificSrand)(unsigned int) = srand;
int (*PlatformSpecificRand)(void) = rand;

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

static int
AtExitImplementation(void (*func)(void))
{
  return atexit(func);
}

double (*PlatformSpecificFabs)(double) = fabs;
int (*PlatformSpecificIsNan)(double) = IsNanImplementation;
int (*PlatformSpecificIsInf)(double) = IsInfImplementation;
int (*PlatformSpecificAtExit)(void (*func)(void)) = AtExitImplementation;

void (*PlatformSpecificAbort)(void) = abort;
