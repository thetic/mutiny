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

#ifndef D_TestOutput_h
#define D_TestOutput_h

///////////////////////////////////////////////////////////////////////////////
//
//  This is a minimal printer interface.
//  We kept streams out to keep footprint small, and so the test
//  harness could be used with less capable compilers so more
//  platforms could use this test harness
//
///////////////////////////////////////////////////////////////////////////////

#include "CppUTest/String.hpp"

namespace cpputest {

class TestShell;
class TestFailure;
class TestResult;

class TestOutput
{
public:
  enum WorkingEnvironment
  {
    visualStudio,
    eclipse,
    detectEnvironment
  };
  enum VerbosityLevel
  {
    level_quiet,
    level_verbose,
    level_veryVerbose
  };

  explicit TestOutput();
  virtual ~TestOutput();

  virtual void printTestsStarted();
  virtual void printTestsEnded(const TestResult& result);
  virtual void printCurrentTestStarted(const TestShell& test);
  virtual void printCurrentTestEnded(const TestResult& res);
  virtual void printCurrentGroupStarted(const TestShell& test);
  virtual void printCurrentGroupEnded(const TestResult& res);

  virtual void verbose(VerbosityLevel level);
  virtual void color();
  virtual void printBuffer(const char*) = 0;
  virtual void print(const char*);
  virtual void print(long);
  virtual void print(size_t);
  virtual void printDouble(double);
  virtual void printFailure(const TestFailure& failure);
  virtual void printTestRun(size_t number, size_t total);
  virtual void setProgressIndicator(const char*);

  virtual void printVeryVerbose(const char*);

  virtual void flush() = 0;

  static void setWorkingEnvironment(WorkingEnvironment workEnvironment);
  static WorkingEnvironment getWorkingEnvironment();

protected:
  virtual void printEclipseErrorInFileOnLine(String file, size_t lineNumber);
  virtual void printVisualStudioErrorInFileOnLine(String file,
      size_t lineNumber);

  virtual void printProgressIndicator();
  void printFileAndLineForTestAndFailure(const TestFailure& failure);
  void printFileAndLineForFailure(const TestFailure& failure);
  void printFailureInTest(String testName);
  void printFailureMessage(String reason);
  void printErrorInFileOnLineFormattedForWorkingEnvironment(String testFile,
      size_t lineNumber);

  TestOutput(const TestOutput&);
  TestOutput& operator=(const TestOutput&);

  int dotCount_;
  VerbosityLevel verbose_;
  bool color_;
  const char* progressIndication_;

  static WorkingEnvironment workingEnvironment_;
};

TestOutput&
operator<<(TestOutput&, const char*);
TestOutput&
operator<<(TestOutput&, long);

} // namespace cpputest

#endif
