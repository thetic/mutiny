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

#ifndef D_MockIgnoredExpectedCall_h
#define D_MockIgnoredExpectedCall_h

#include "CppUTestExt/MockExpectedCall.hpp"

namespace cpputest {
namespace extensions {

class MockIgnoredExpectedCall : public MockExpectedCall
{
public:
  virtual MockExpectedCall& withName(const cpputest::String&) override
  {
    return *this;
  }
  virtual MockExpectedCall& withCallOrder(unsigned int) override
  {
    return *this;
  }
  virtual MockExpectedCall& withCallOrder(unsigned int, unsigned int) override
  {
    return *this;
  }
  virtual MockExpectedCall& withBoolParameter(const cpputest::String&,
      bool) override
  {
    return *this;
  }
  virtual MockExpectedCall& withIntParameter(const cpputest::String&,
      int) override
  {
    return *this;
  }
  virtual MockExpectedCall& withUnsignedIntParameter(const cpputest::String&,
      unsigned int) override
  {
    return *this;
  }
  virtual MockExpectedCall& withLongIntParameter(const cpputest::String&,
      long int) override
  {
    return *this;
  }
  virtual MockExpectedCall& withUnsignedLongIntParameter(
      const cpputest::String&,
      unsigned long int) override
  {
    return *this;
  }
  virtual MockExpectedCall& withLongLongIntParameter(const cpputest::String&,
      long long) override
  {
    return *this;
  }
  virtual MockExpectedCall& withUnsignedLongLongIntParameter(
      const cpputest::String&,
      unsigned long long) override
  {
    return *this;
  }
  virtual MockExpectedCall& withDoubleParameter(const cpputest::String&,
      double) override
  {
    return *this;
  }
  virtual MockExpectedCall& withDoubleParameter(const cpputest::String&,
      double,
      double) override
  {
    return *this;
  }
  virtual MockExpectedCall& withStringParameter(const cpputest::String&,
      const char*) override
  {
    return *this;
  }
  virtual MockExpectedCall& withPointerParameter(const cpputest::String&,
      void*) override
  {
    return *this;
  }
  virtual MockExpectedCall& withConstPointerParameter(const cpputest::String&,
      const void*) override
  {
    return *this;
  }
  virtual MockExpectedCall& withFunctionPointerParameter(
      const cpputest::String&,
      void (*)()) override
  {
    return *this;
  }
  virtual MockExpectedCall& withMemoryBufferParameter(const cpputest::String&,
      const unsigned char*,
      size_t) override
  {
    return *this;
  }
  virtual MockExpectedCall& withParameterOfType(const cpputest::String&,
      const cpputest::String&,
      const void*) override
  {
    return *this;
  }
  virtual MockExpectedCall& withOutputParameterReturning(
      const cpputest::String&,
      const void*,
      size_t) override
  {
    return *this;
  }
  virtual MockExpectedCall& withOutputParameterOfTypeReturning(
      const cpputest::String&,
      const cpputest::String&,
      const void*) override
  {
    return *this;
  }
  virtual MockExpectedCall& withUnmodifiedOutputParameter(
      const cpputest::String&) override
  {
    return *this;
  }
  virtual MockExpectedCall& ignoreOtherParameters() override { return *this; }

  virtual MockExpectedCall& andReturnValue(bool) override { return *this; }
  virtual MockExpectedCall& andReturnValue(int) override { return *this; }
  virtual MockExpectedCall& andReturnValue(unsigned int) override
  {
    return *this;
  }
  virtual MockExpectedCall& andReturnValue(long int) override { return *this; }
  virtual MockExpectedCall& andReturnValue(unsigned long int) override
  {
    return *this;
  }
  virtual MockExpectedCall& andReturnValue(long long) override { return *this; }
  virtual MockExpectedCall& andReturnValue(unsigned long long) override
  {
    return *this;
  }
  virtual MockExpectedCall& andReturnValue(double) override { return *this; }
  virtual MockExpectedCall& andReturnValue(const char*) override
  {
    return *this;
  }
  virtual MockExpectedCall& andReturnValue(void*) override { return *this; }
  virtual MockExpectedCall& andReturnValue(const void*) override
  {
    return *this;
  }
  virtual MockExpectedCall& andReturnValue(void (*)()) override
  {
    return *this;
  }

  virtual MockExpectedCall& onObject(void*) override { return *this; }

  static MockExpectedCall& instance();
};

} // namespace extensions
} // namespace cpputest

#endif
