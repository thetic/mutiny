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
