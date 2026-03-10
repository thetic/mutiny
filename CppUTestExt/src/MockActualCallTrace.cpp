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

#include "CppUTestExt/MockActualCallTrace.hpp"

#include "CppUTest/TestHarness.hpp"

namespace cpputest {
namespace extensions {

MockActualCallTrace* MockActualCallTrace::instance_ = nullptr;

MockActualCallTrace::MockActualCallTrace() {}

MockActualCallTrace::~MockActualCallTrace() {}

MockActualCall&
MockActualCallTrace::withName(const String& name)
{
  traceBuffer_ += "\nFunction name:";
  traceBuffer_ += name;
  return *this;
}

MockActualCall&
MockActualCallTrace::withCallOrder(unsigned int callOrder)
{
  traceBuffer_ += " withCallOrder:";
  traceBuffer_ += cpputest::StringFrom(callOrder);
  return *this;
}

void
MockActualCallTrace::addParameterName(const String& name)
{
  traceBuffer_ += " ";
  traceBuffer_ += name;
  traceBuffer_ += ":";
}

MockActualCall&
MockActualCallTrace::withBoolParameter(const String& name, bool value)
{
  addParameterName(name);
  traceBuffer_ += cpputest::StringFrom(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::withUnsignedIntParameter(const String& name,
    unsigned int value)
{
  addParameterName(name);
  traceBuffer_ +=
      cpputest::StringFrom(value) + " " + BracketsFormattedHexStringFrom(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::withIntParameter(const String& name, int value)
{
  addParameterName(name);
  traceBuffer_ +=
      cpputest::StringFrom(value) + " " + BracketsFormattedHexStringFrom(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::withUnsignedLongIntParameter(const String& name,
    unsigned long int value)
{
  addParameterName(name);
  traceBuffer_ +=
      cpputest::StringFrom(value) + " " + BracketsFormattedHexStringFrom(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::withLongIntParameter(const String& name, long int value)
{
  addParameterName(name);
  traceBuffer_ +=
      cpputest::StringFrom(value) + " " + BracketsFormattedHexStringFrom(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::withUnsignedLongLongIntParameter(const String& name,
    unsigned long long value)
{
  addParameterName(name);
  traceBuffer_ +=
      cpputest::StringFrom(value) + " " + BracketsFormattedHexStringFrom(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::withLongLongIntParameter(const String& name,
    long long value)
{
  addParameterName(name);
  traceBuffer_ +=
      cpputest::StringFrom(value) + " " + BracketsFormattedHexStringFrom(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::withDoubleParameter(const String& name, double value)
{
  addParameterName(name);
  traceBuffer_ += cpputest::StringFrom(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::withStringParameter(const String& name, const char* value)
{
  addParameterName(name);
  traceBuffer_ += cpputest::StringFrom(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::withPointerParameter(const String& name, void* value)
{
  addParameterName(name);
  traceBuffer_ += cpputest::StringFrom(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::withConstPointerParameter(const String& name,
    const void* value)
{
  addParameterName(name);
  traceBuffer_ += cpputest::StringFrom(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::withFunctionPointerParameter(const String& name,
    void (*value)())
{
  addParameterName(name);
  traceBuffer_ += cpputest::StringFrom(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::withMemoryBufferParameter(const String& name,
    const unsigned char* value,
    size_t size)
{
  addParameterName(name);
  traceBuffer_ += StringFromBinaryWithSizeOrNull(value, size);
  return *this;
}

MockActualCall&
MockActualCallTrace::withParameterOfType(const String& typeName,
    const String& name,
    const void* value)
{
  traceBuffer_ += " ";
  traceBuffer_ += typeName;
  addParameterName(name);
  traceBuffer_ += cpputest::StringFrom(value);
  return *this;
}

MockActualCall&
MockActualCallTrace::withMemoryBufferParameter(const char* name,
    const unsigned char* value,
    size_t size)
{
  return withMemoryBufferParameter(String(name), value, size);
}

MockActualCall&
MockActualCallTrace::withParameterOfType(const char* typeName,
    const char* name,
    const void* value)
{
  return withParameterOfType(String(typeName), String(name), value);
}

MockActualCall&
MockActualCallTrace::withOutputParameter(const String& name, void* output)
{
  addParameterName(name);
  traceBuffer_ += cpputest::StringFrom(output);
  return *this;
}

MockActualCall&
MockActualCallTrace::withOutputParameterOfType(const String& typeName,
    const String& name,
    void* output)
{
  traceBuffer_ += " ";
  traceBuffer_ += typeName;
  addParameterName(name);
  traceBuffer_ += cpputest::StringFrom(output);
  return *this;
}

bool
MockActualCallTrace::hasReturnValue()
{
  return false;
}

MockNamedValue
MockActualCallTrace::returnValue()
{
  return MockNamedValue("");
}

long int
MockActualCallTrace::returnLongIntValue()
{
  return 0;
}

unsigned long int
MockActualCallTrace::returnUnsignedLongIntValue()
{
  return 0;
}

unsigned long int
MockActualCallTrace::returnUnsignedLongIntValueOrDefault(unsigned long)
{
  return 0;
}

long int
MockActualCallTrace::returnLongIntValueOrDefault(long int)
{
  return 0;
}

long long
MockActualCallTrace::returnLongLongIntValue()
{
  return 0;
}

unsigned long long
MockActualCallTrace::returnUnsignedLongLongIntValue()
{
  return 0;
}

unsigned long long
MockActualCallTrace::returnUnsignedLongLongIntValueOrDefault(unsigned long long)
{
  return 0;
}

long long
MockActualCallTrace::returnLongLongIntValueOrDefault(long long)
{
  return 0;
}

bool
MockActualCallTrace::returnBoolValue()
{
  return false;
}

bool
MockActualCallTrace::returnBoolValueOrDefault(bool)
{
  return false;
}

int
MockActualCallTrace::returnIntValue()
{
  return 0;
}

double
MockActualCallTrace::returnDoubleValue()
{
  return 0.0;
}

double
MockActualCallTrace::returnDoubleValueOrDefault(double)
{
  return returnDoubleValue();
}

unsigned int
MockActualCallTrace::returnUnsignedIntValue()
{
  return 0;
}

void*
MockActualCallTrace::returnPointerValue()
{
  return nullptr;
}

const void*
MockActualCallTrace::returnConstPointerValue()
{
  return nullptr;
}

MockActualCall::FunctionPointerReturnValue
MockActualCallTrace::returnFunctionPointerValue()
{
  return nullptr;
}

const void*
MockActualCallTrace::returnConstPointerValueOrDefault(const void*)
{
  return returnConstPointerValue();
}

void*
MockActualCallTrace::returnPointerValueOrDefault(void*)
{
  return returnPointerValue();
}

MockActualCall::FunctionPointerReturnValue
MockActualCallTrace::returnFunctionPointerValueOrDefault(void (*)())
{
  return returnFunctionPointerValue();
}

const char*
MockActualCallTrace::returnStringValue()
{
  return "";
}

const char*
MockActualCallTrace::returnStringValueOrDefault(const char*)
{
  return returnStringValue();
}

int
MockActualCallTrace::returnIntValueOrDefault(int)
{
  return 0;
}

unsigned int
MockActualCallTrace::returnUnsignedIntValueOrDefault(unsigned int)
{
  return returnUnsignedIntValue();
}

MockActualCall&
MockActualCallTrace::onObject(const void* objectPtr)
{
  traceBuffer_ += " onObject:";
  traceBuffer_ += cpputest::StringFrom(objectPtr);
  return *this;
}

void
MockActualCallTrace::clear()
{
  traceBuffer_ = "";
}

const char*
MockActualCallTrace::getTraceOutput()
{
  return traceBuffer_.c_str();
}

MockActualCallTrace&
MockActualCallTrace::instance()
{
  if (instance_ == nullptr)
    instance_ = new MockActualCallTrace;
  return *instance_;
}

void
MockActualCallTrace::clearInstance()
{
  delete instance_;
  instance_ = nullptr;
}

} // namespace extensions
} // namespace cpputest
