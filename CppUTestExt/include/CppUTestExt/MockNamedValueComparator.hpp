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

#ifndef D_MockNamedValueComparator_h
#define D_MockNamedValueComparator_h

#include "CppUTest/String.hpp"

namespace cpputest {
namespace extensions {

/*
 * MockNamedValueComparator is an interface that needs to be used when creating
 * Comparators. This is needed when comparing values of non-native type.
 */

class MockNamedValueComparator
{
public:
  MockNamedValueComparator() {}
  virtual ~MockNamedValueComparator() {}

  virtual bool isEqual(const void* object1, const void* object2) = 0;
  virtual cpputest::String valueToString(const void* object) = 0;
};

/*
 * MockNamedValueCopier is an interface that needs to be used when creating
 * Copiers. This is needed when copying values of non-native type.
 */

class MockNamedValueCopier
{
public:
  MockNamedValueCopier() {}
  virtual ~MockNamedValueCopier() {}

  virtual void copy(void* out, const void* in) = 0;
};

class MockFunctionComparator : public MockNamedValueComparator
{
public:
  typedef bool (*isEqualFunction)(const void*, const void*);
  typedef cpputest::String (*valueToStringFunction)(const void*);

  MockFunctionComparator(isEqualFunction equal,
      valueToStringFunction valToString)
    : equal_(equal)
    , valueToString_(valToString)
  {
  }

  virtual bool isEqual(const void* object1, const void* object2) override
  {
    return equal_(object1, object2);
  }
  virtual cpputest::String valueToString(const void* object) override
  {
    return valueToString_(object);
  }

private:
  isEqualFunction equal_;
  valueToStringFunction valueToString_;
};

class MockFunctionCopier : public MockNamedValueCopier
{
public:
  typedef void (*copyFunction)(void*, const void*);

  MockFunctionCopier(copyFunction copier)
    : copier_(copier)
  {
  }

  virtual void copy(void* dst, const void* src) override { copier_(dst, src); }

private:
  copyFunction copier_;
};

} // namespace extensions
} // namespace cpputest

#endif
