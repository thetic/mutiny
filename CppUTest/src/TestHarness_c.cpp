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

#include "CppUTest/TestHarness_c.h"

#include "CppUTest/TestHarness.h"

void
CHECK_EQUAL_C_BOOL_LOCATION(int expected,
    int actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertEquals(!!expected != !!actual,
      expected ? "true" : "false",
      actual ? "true" : "false",
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_INT_LOCATION(int expected,
    int actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertLongsEqual(
      static_cast<long>(expected),
      static_cast<long>(actual),
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_UINT_LOCATION(unsigned int expected,
    unsigned int actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertUnsignedLongsEqual(
      static_cast<unsigned long>(expected),
      static_cast<unsigned long>(actual),
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_LONG_LOCATION(long expected,
    long actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertLongsEqual(expected,
      actual,
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_ULONG_LOCATION(unsigned long expected,
    unsigned long actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertUnsignedLongsEqual(expected,
      actual,
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_LONGLONG_LOCATION(long long expected,
    long long actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertLongLongsEqual(expected,
      actual,
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_ULONGLONG_LOCATION(unsigned long long expected,
    unsigned long long actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertUnsignedLongLongsEqual(expected,
      actual,
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_REAL_LOCATION(double expected,
    double actual,
    double threshold,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertDoublesEqual(expected,
      actual,
      threshold,
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_CHAR_LOCATION(char expected,
    char actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertEquals(((expected) != (actual)),
      cpputest::StringFrom(expected),
      cpputest::StringFrom(actual),
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_UBYTE_LOCATION(unsigned char expected,
    unsigned char actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertEquals(((expected) != (actual)),
      cpputest::StringFrom(static_cast<int>(expected)),
      cpputest::StringFrom(static_cast<int>(actual)),
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_SBYTE_LOCATION(char signed expected,
    signed char actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertEquals(((expected) != (actual)),
      cpputest::StringFrom(static_cast<int>(expected)),
      cpputest::StringFrom(static_cast<int>(actual)),
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_STRING_LOCATION(const char* expected,
    const char* actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertCstrEqual(expected,
      actual,
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_POINTER_LOCATION(const void* expected,
    const void* actual,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertPointersEqual(expected,
      actual,
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_EQUAL_C_MEMCMP_LOCATION(const void* expected,
    const void* actual,
    size_t size,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertBinaryEqual(expected,
      actual,
      size,
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
FAIL_TEXT_C_LOCATION(const char* text, const char* fileName, size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->fail(text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
FAIL_C_LOCATION(const char* fileName, size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->fail("",
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}

void
CHECK_C_LOCATION(int condition,
    const char* conditionString,
    const char* text,
    const char* fileName,
    size_t lineNumber)
{
  cpputest::TestShell::getCurrent()->assertTrue(condition != 0,
      "CHECK_C",
      conditionString,
      text,
      fileName,
      lineNumber,
      cpputest::TestShell::getCurrentTestTerminatorWithoutExceptions());
}
