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

#ifndef D_ExecFunctionTestShell_h
#define D_ExecFunctionTestShell_h

#include "CppUTest/Utest.hpp"

namespace cpputest {

class ExecFunctionTestShell;

class ExecFunctionTest : public Test
{
public:
  ExecFunctionTest(ExecFunctionTestShell* shell);
  void testBody() override;
  virtual void setup() override;
  virtual void teardown() override;

private:
  ExecFunctionTestShell* shell_;
};

//////////////////// ExecFunction

class ExecFunction
{
public:
  ExecFunction();
  virtual ~ExecFunction();

  virtual void exec() = 0;
};

class ExecFunctionWithoutParameters : public ExecFunction
{
public:
  void (*testFunction_)();

  ExecFunctionWithoutParameters(void (*testFunction)());
  virtual ~ExecFunctionWithoutParameters() override;

  virtual void exec() override;
};

//////////////////// ExecFunctionTestShell

class ExecFunctionTestShell : public TestShell
{
public:
  void (*setup_)();
  void (*teardown_)();
  ExecFunction* testFunction_;

  ExecFunctionTestShell(void (*set)() = nullptr, void (*tear)() = nullptr)
    : TestShell("ExecFunction", "ExecFunction", "ExecFunction", 1)
    , setup_(set)
    , teardown_(tear)
    , testFunction_(nullptr)
  {
  }

  Test* createTest() override { return new ExecFunctionTest(this); }
  virtual ~ExecFunctionTestShell() override;
};

} // namespace cpputest

#endif
