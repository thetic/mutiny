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

#ifndef D_CompositeTestOutput_h
#define D_CompositeTestOutput_h

#include "CppUTest/TestOutput.hpp"

namespace cpputest {

class CompositeTestOutput : public TestOutput
{
public:
  virtual void setOutputOne(TestOutput* output);
  virtual void setOutputTwo(TestOutput* output);

  CompositeTestOutput();
  virtual ~CompositeTestOutput() override;

  virtual void printTestsStarted() override;
  virtual void printTestsEnded(const TestResult& result) override;

  virtual void printCurrentTestStarted(const TestShell& test) override;
  virtual void printCurrentTestEnded(const TestResult& res) override;
  virtual void printCurrentGroupStarted(const TestShell& test) override;
  virtual void printCurrentGroupEnded(const TestResult& res) override;

  virtual void verbose(VerbosityLevel level) override;
  virtual void color() override;
  virtual void printBuffer(const char*) override;
  virtual void print(const char*) override;
  virtual void print(long) override;
  virtual void print(size_t) override;
  virtual void printDouble(double) override;
  virtual void printFailure(const TestFailure& failure) override;
  virtual void setProgressIndicator(const char*) override;

  virtual void printVeryVerbose(const char*) override;

  virtual void flush() override;

protected:
  CompositeTestOutput(const TestOutput&);
  CompositeTestOutput& operator=(const TestOutput&);

private:
  TestOutput* outputOne_;
  TestOutput* outputTwo_;
};

} // namespace cpputest

#endif
