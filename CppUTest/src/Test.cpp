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

#include "CppUTest/Test.hpp"

#include "CppUTest/PlatformSpecificFunctions.hpp"
#include "CppUTest/TestFailure.hpp"
#include "CppUTest/TestShell.hpp"

namespace cpputest {

static void
helperDoTestSetup(void* data)
{
  static_cast<Test*>(data)->setup();
}

static void
helperDoTestBody(void* data)
{
  static_cast<Test*>(data)->testBody();
}

static void
helperDoTestTeardown(void* data)
{
  static_cast<Test*>(data)->teardown();
}

Test::Test() {}

Test::~Test() {}

#if CPPUTEST_HAVE_EXCEPTIONS

void
Test::run()
{
  TestShell* current = TestShell::getCurrent();
  int jumpResult = 0;
  try {
    current->printVeryVerbose("\n-------- before setup: ");
    jumpResult = PlatformSpecificSetJmp(helperDoTestSetup, this);
    current->printVeryVerbose("\n-------- after  setup: ");

    if (jumpResult) {
      current->printVeryVerbose("\n----------  before body: ");
      PlatformSpecificSetJmp(helperDoTestBody, this);
      current->printVeryVerbose("\n----------  after body: ");
    }
  } catch (FailedException&) {
    PlatformSpecificRestoreJumpBuffer();
  }
#if CPPUTEST_USE_STD_CPP_LIB
  catch (const std::exception& e) {
    current->addFailure(UnexpectedExceptionFailure(current, e));
    PlatformSpecificRestoreJumpBuffer();
    if (current->isRethrowingExceptions()) {
      throw;
    }
  }
#endif
  catch (...) {
    current->addFailure(UnexpectedExceptionFailure(current));
    PlatformSpecificRestoreJumpBuffer();
    if (current->isRethrowingExceptions()) {
      throw;
    }
  }

  try {
    current->printVeryVerbose("\n--------  before teardown: ");
    PlatformSpecificSetJmp(helperDoTestTeardown, this);
    current->printVeryVerbose("\n--------  after teardown: ");
  } catch (FailedException&) {
    PlatformSpecificRestoreJumpBuffer();
  }
#if CPPUTEST_USE_STD_CPP_LIB
  catch (const std::exception& e) {
    current->addFailure(UnexpectedExceptionFailure(current, e));
    PlatformSpecificRestoreJumpBuffer();
    if (current->isRethrowingExceptions()) {
      throw;
    }
  }
#endif
  catch (...) {
    current->addFailure(UnexpectedExceptionFailure(current));
    PlatformSpecificRestoreJumpBuffer();
    if (current->isRethrowingExceptions()) {
      throw;
    }
  }
}
#else

void
Test::run()
{
  if (PlatformSpecificSetJmp(helperDoTestSetup, this)) {
    PlatformSpecificSetJmp(helperDoTestBody, this);
  }
  PlatformSpecificSetJmp(helperDoTestTeardown, this);
}

#endif

void
Test::setup()
{
}

void
Test::testBody()
{
}

void
Test::teardown()
{
}

} // namespace cpputest
