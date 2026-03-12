#ifndef D_MockExpectedCall_h
#define D_MockExpectedCall_h

#include "CppUTest/String.hpp"

namespace cpputest {
namespace extensions {

class MockNamedValue;

cpputest::String
StringFrom(const MockNamedValue& parameter);

class MockExpectedCall
{
public:
  MockExpectedCall();
  virtual ~MockExpectedCall();

  virtual MockExpectedCall& withName(const cpputest::String& name) = 0;
  virtual MockExpectedCall& withCallOrder(unsigned int) = 0;
  virtual MockExpectedCall& withCallOrder(unsigned int, unsigned int) = 0;
  MockExpectedCall& withParameter(const cpputest::String& name, bool value)
  {
    return withBoolParameter(name, value);
  }
  MockExpectedCall& withParameter(const cpputest::String& name, int value)
  {
    return withIntParameter(name, value);
  }
  MockExpectedCall& withParameter(const cpputest::String& name,
      unsigned int value)
  {
    return withUnsignedIntParameter(name, value);
  }
  MockExpectedCall& withParameter(const cpputest::String& name, long int value)
  {
    return withLongIntParameter(name, value);
  }
  MockExpectedCall& withParameter(const cpputest::String& name,
      unsigned long int value)
  {
    return withUnsignedLongIntParameter(name, value);
  }
  MockExpectedCall& withParameter(const cpputest::String& name, long long value)
  {
    return withLongLongIntParameter(name, value);
  }
  MockExpectedCall& withParameter(const cpputest::String& name,
      unsigned long long value)
  {
    return withUnsignedLongLongIntParameter(name, value);
  }
  MockExpectedCall& withParameter(const cpputest::String& name, double value)
  {
    return withDoubleParameter(name, value);
  }
  MockExpectedCall& withParameter(const cpputest::String& name,
      double value,
      double tolerance)
  {
    return withDoubleParameter(name, value, tolerance);
  }
  MockExpectedCall& withParameter(const cpputest::String& name,
      const char* value)
  {
    return withStringParameter(name, value);
  }
  MockExpectedCall& withParameter(const cpputest::String& name, void* value)
  {
    return withPointerParameter(name, value);
  }
  MockExpectedCall& withParameter(const cpputest::String& name,
      const void* value)
  {
    return withConstPointerParameter(name, value);
  }
  MockExpectedCall& withParameter(const cpputest::String& name, void (*value)())
  {
    return withFunctionPointerParameter(name, value);
  }
  MockExpectedCall& withParameter(const cpputest::String& name,
      const unsigned char* value,
      size_t size)
  {
    return withMemoryBufferParameter(name, value, size);
  }
  virtual MockExpectedCall& withParameterOfType(
      const cpputest::String& typeName,
      const cpputest::String& name,
      const void* value) = 0;
  virtual MockExpectedCall& withOutputParameterReturning(
      const cpputest::String& name,
      const void* value,
      size_t size) = 0;
  virtual MockExpectedCall& withOutputParameterOfTypeReturning(
      const cpputest::String& typeName,
      const cpputest::String& name,
      const void* value) = 0;
  virtual MockExpectedCall& withUnmodifiedOutputParameter(
      const cpputest::String& name) = 0;
  virtual MockExpectedCall& ignoreOtherParameters() = 0;

  virtual MockExpectedCall& withBoolParameter(const cpputest::String& name,
      bool value) = 0;
  virtual MockExpectedCall& withIntParameter(const cpputest::String& name,
      int value) = 0;
  virtual MockExpectedCall& withUnsignedIntParameter(
      const cpputest::String& name,
      unsigned int value) = 0;
  virtual MockExpectedCall& withLongIntParameter(const cpputest::String& name,
      long int value) = 0;
  virtual MockExpectedCall& withUnsignedLongIntParameter(
      const cpputest::String& name,
      unsigned long int value) = 0;
  virtual MockExpectedCall& withLongLongIntParameter(
      const cpputest::String& name,
      long long value) = 0;
  virtual MockExpectedCall& withUnsignedLongLongIntParameter(
      const cpputest::String& name,
      unsigned long long value) = 0;
  virtual MockExpectedCall& withDoubleParameter(const cpputest::String& name,
      double value) = 0;
  virtual MockExpectedCall& withDoubleParameter(const cpputest::String& name,
      double value,
      double tolerance) = 0;
  virtual MockExpectedCall& withStringParameter(const cpputest::String& name,
      const char* value) = 0;
  virtual MockExpectedCall& withPointerParameter(const cpputest::String& name,
      void* value) = 0;
  virtual MockExpectedCall& withFunctionPointerParameter(
      const cpputest::String& name,
      void (*value)()) = 0;
  virtual MockExpectedCall& withConstPointerParameter(
      const cpputest::String& name,
      const void* value) = 0;
  virtual MockExpectedCall& withMemoryBufferParameter(
      const cpputest::String& name,
      const unsigned char* value,
      size_t size) = 0;
  virtual MockExpectedCall& andReturnValue(bool value) = 0;
  virtual MockExpectedCall& andReturnValue(int value) = 0;
  virtual MockExpectedCall& andReturnValue(unsigned int value) = 0;
  virtual MockExpectedCall& andReturnValue(long int value) = 0;
  virtual MockExpectedCall& andReturnValue(unsigned long int value) = 0;
  virtual MockExpectedCall& andReturnValue(long long value) = 0;
  virtual MockExpectedCall& andReturnValue(unsigned long long value) = 0;
  virtual MockExpectedCall& andReturnValue(double value) = 0;
  virtual MockExpectedCall& andReturnValue(const char* value) = 0;
  virtual MockExpectedCall& andReturnValue(void* value) = 0;
  virtual MockExpectedCall& andReturnValue(const void* value) = 0;
  virtual MockExpectedCall& andReturnValue(void (*value)()) = 0;

  virtual MockExpectedCall& onObject(void* objectPtr) = 0;
};

} // namespace extensions
} // namespace cpputest

#endif
