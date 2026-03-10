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

#ifndef D_MockNamedValue_h
#define D_MockNamedValue_h

#include "CppUTestExt/MockNamedValueComparator.hpp"
#include "CppUTestExt/MockNamedValueCopier.hpp"

namespace cpputest {
namespace extensions {

/*
 * MockNamedValue is the generic value class used. It encapsulates basic types
 * and can use them "as if one" Also it enables other types by putting object
 * pointers. They can be compared with comparators.
 *
 * Basically this class ties together a Name, a Value, a Type, and a Comparator
 */

class MockNamedValueComparatorsAndCopiersRepository;
class MockNamedValue
{
public:
  MockNamedValue(const cpputest::String& name);
  MockNamedValue(const MockNamedValue&) = default;
  MockNamedValue(MockNamedValue&&) noexcept;
  virtual ~MockNamedValue();

  virtual void setValue(bool value);
  virtual void setValue(int value);
  virtual void setValue(unsigned int value);
  virtual void setValue(long int value);
  virtual void setValue(unsigned long int value);
  virtual void setValue(long long value);
  virtual void setValue(unsigned long long value);
  virtual void setValue(double value);
  virtual void setValue(double value, double tolerance);
  virtual void setValue(void* value);
  virtual void setValue(const void* value);
  virtual void setValue(void (*value)());
  virtual void setValue(const char* value);
  virtual void setMemoryBuffer(const unsigned char* value, size_t size);
  virtual void setConstObjectPointer(const cpputest::String& type,
      const void* objectPtr);
  virtual void setObjectPointer(const cpputest::String& type, void* objectPtr);
  virtual void setSize(size_t size);

  virtual void setName(const char* name);

  virtual bool equals(const MockNamedValue& p) const;
  virtual bool compatibleForCopying(const MockNamedValue& p) const;

  virtual cpputest::String toString() const;

  virtual cpputest::String getName() const;
  virtual cpputest::String getType() const;

  virtual bool getBoolValue() const;
  virtual int getIntValue() const;
  virtual unsigned int getUnsignedIntValue() const;
  virtual long int getLongIntValue() const;
  virtual unsigned long int getUnsignedLongIntValue() const;
  virtual long long getLongLongIntValue() const;
  virtual unsigned long long getUnsignedLongLongIntValue() const;
  virtual double getDoubleValue() const;
  virtual double getDoubleTolerance() const;
  virtual const char* getStringValue() const;
  virtual void* getPointerValue() const;
  virtual const void* getConstPointerValue() const;
  virtual void (*getFunctionPointerValue() const)();
  virtual const unsigned char* getMemoryBuffer() const;
  virtual const void* getConstObjectPointer() const;
  virtual void* getObjectPointer() const;
  virtual bool isConstObject() const;
  virtual size_t getSize() const;

  virtual MockNamedValueComparator* getComparator() const;
  virtual MockNamedValueCopier* getCopier() const;

  static void setDefaultComparatorsAndCopiersRepository(
      MockNamedValueComparatorsAndCopiersRepository* repository);
  static MockNamedValueComparatorsAndCopiersRepository*
  getDefaultComparatorsAndCopiersRepository();

  static const double defaultDoubleTolerance;

private:
  cpputest::String name_;
  cpputest::String type_;
  bool isConstObject_;
  union
  {
    bool boolValue_;
    int intValue_;
    unsigned int unsignedIntValue_;
    long int longIntValue_;
    unsigned long int unsignedLongIntValue_;
    long long longLongIntValue_;
    unsigned long long unsignedLongLongIntValue_;
    struct
    {
      double value;
      double tolerance;
    } doubleValue_;
    const char* stringValue_;
    void* pointerValue_;
    const void* constPointerValue_;
    void (*functionPointerValue_)();
    const unsigned char* memoryBufferValue_;
    const void* constObjectPointerValue_;
    void* objectPointerValue_;
    const void* outputPointerValue_;
  } value_;
  size_t size_;
  MockNamedValueComparator* comparator_;
  MockNamedValueCopier* copier_;
  static MockNamedValueComparatorsAndCopiersRepository* defaultRepository_;
};

} // namespace extensions
} // namespace cpputest

#endif
