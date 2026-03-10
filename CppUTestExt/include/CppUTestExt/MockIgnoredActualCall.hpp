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

#ifndef D_MockIgnoredActualCall_h
#define D_MockIgnoredActualCall_h

#include "CppUTestExt/MockActualCall.hpp"

namespace cpputest {
namespace extensions {

class MockIgnoredActualCall : public MockActualCall
{
public:
  virtual MockActualCall& withName(const cpputest::String&) override
  {
    return *this;
  }
  virtual MockActualCall& withCallOrder(unsigned int) override { return *this; }
  virtual MockActualCall& withBoolParameter(const cpputest::String&,
      bool) override
  {
    return *this;
  }
  virtual MockActualCall& withIntParameter(const cpputest::String&,
      int) override
  {
    return *this;
  }
  virtual MockActualCall& withUnsignedIntParameter(const cpputest::String&,
      unsigned int) override
  {
    return *this;
  }
  virtual MockActualCall& withLongIntParameter(const cpputest::String&,
      long int) override
  {
    return *this;
  }
  virtual MockActualCall& withUnsignedLongIntParameter(const cpputest::String&,
      unsigned long int) override
  {
    return *this;
  }
  virtual MockActualCall& withLongLongIntParameter(const cpputest::String&,
      long long) override
  {
    return *this;
  }
  virtual MockActualCall& withUnsignedLongLongIntParameter(
      const cpputest::String&,
      unsigned long long) override
  {
    return *this;
  }
  virtual MockActualCall& withDoubleParameter(const cpputest::String&,
      double) override
  {
    return *this;
  }
  virtual MockActualCall& withStringParameter(const cpputest::String&,
      const char*) override
  {
    return *this;
  }
  virtual MockActualCall& withPointerParameter(const cpputest::String&,
      void*) override
  {
    return *this;
  }
  virtual MockActualCall& withConstPointerParameter(const cpputest::String&,
      const void*) override
  {
    return *this;
  }
  virtual MockActualCall& withFunctionPointerParameter(const cpputest::String&,
      void (*)()) override
  {
    return *this;
  }
  virtual MockActualCall& withMemoryBufferParameter(const cpputest::String&,
      const unsigned char*,
      size_t) override
  {
    return *this;
  }
  virtual MockActualCall& withMemoryBufferParameter(const char*,
      const unsigned char*,
      size_t) override
  {
    return *this;
  }
  virtual MockActualCall& withParameterOfType(const cpputest::String&,
      const cpputest::String&,
      const void*) override
  {
    return *this;
  }
  virtual MockActualCall& withParameterOfType(const char*,
      const char*,
      const void*) override
  {
    return *this;
  }
  virtual MockActualCall& withOutputParameter(const cpputest::String&,
      void*) override
  {
    return *this;
  }
  virtual MockActualCall& withOutputParameterOfType(const cpputest::String&,
      const cpputest::String&,
      void*) override
  {
    return *this;
  }

  virtual bool hasReturnValue() override { return false; }
  virtual MockNamedValue returnValue() override { return MockNamedValue(""); }

  virtual bool returnBoolValueOrDefault(bool value) override { return value; }
  virtual bool returnBoolValue() override { return false; }

  virtual int returnIntValue() override { return 0; }
  virtual int returnIntValueOrDefault(int value) override { return value; }

  virtual unsigned long int returnUnsignedLongIntValue() override { return 0; }
  virtual unsigned long int returnUnsignedLongIntValueOrDefault(
      unsigned long int value) override
  {
    return value;
  }

  virtual long int returnLongIntValue() override { return 0; }
  virtual long int returnLongIntValueOrDefault(long int value) override
  {
    return value;
  }

  virtual unsigned long long returnUnsignedLongLongIntValue() override
  {
    return 0;
  }

  virtual unsigned long long returnUnsignedLongLongIntValueOrDefault(
      unsigned long long value) override
  {
    return value;
  }

  virtual long long returnLongLongIntValue() override { return 0; }

  virtual long long returnLongLongIntValueOrDefault(long long value) override
  {
    return value;
  }

  virtual unsigned int returnUnsignedIntValue() override { return 0; }
  virtual unsigned int returnUnsignedIntValueOrDefault(
      unsigned int value) override
  {
    return value;
  }

  virtual double returnDoubleValue() override { return 0.0; }
  virtual double returnDoubleValueOrDefault(double value) override
  {
    return value;
  }

  virtual const char* returnStringValue() override { return ""; }
  virtual const char* returnStringValueOrDefault(const char* value) override
  {
    return value;
  }

  virtual void* returnPointerValue() override { return nullptr; }
  virtual void* returnPointerValueOrDefault(void* value) override
  {
    return value;
  }

  virtual const void* returnConstPointerValue() override { return nullptr; }
  virtual const void* returnConstPointerValueOrDefault(
      const void* value) override
  {
    return value;
  }

  virtual FunctionPointerReturnValue returnFunctionPointerValue() override
  {
    return nullptr;
  }
  virtual FunctionPointerReturnValue returnFunctionPointerValueOrDefault(
      void (*value)()) override
  {
    return value;
  }

  virtual MockActualCall& onObject(const void*) override { return *this; }

  static MockIgnoredActualCall& instance()
  {
    static MockIgnoredActualCall call;
    return call;
  }
};

} // namespace extensions
} // namespace cpputest

#endif
