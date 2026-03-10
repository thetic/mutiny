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

#ifndef D_MockSupport_h
#define D_MockSupport_h

#include "CppUTestExt/MockCheckedActualCall.h"
#include "CppUTestExt/MockCheckedExpectedCall.h"
#include "CppUTestExt/MockExpectedCallsList.h"
#include "CppUTestExt/MockFailure.h"

namespace cpputest {
namespace extensions {

class MockSupport;

/* This allows access to "the global" mocking support for easier testing */
MockSupport&
mock(const cpputest::String& mockName = "",
     MockFailureReporter* failureReporterForThisCall = nullptr);

class MockSupport
{
public:
  MockSupport(const cpputest::String& mockName = "");
  virtual ~MockSupport();

  virtual void strictOrder();
  virtual MockExpectedCall& expectOneCall(const cpputest::String& functionName);
  virtual void expectNoCall(const cpputest::String& functionName);
  virtual MockExpectedCall& expectNCalls(unsigned int amount,
                                         const cpputest::String& functionName);
  virtual MockActualCall& actualCall(const cpputest::String& functionName);
  virtual MockActualCall& actualCall(const char* functionName);
  virtual bool hasReturnValue();
  virtual MockNamedValue returnValue();
  virtual bool boolReturnValue();
  virtual bool returnBoolValueOrDefault(bool defaultValue);
  virtual int intReturnValue();
  virtual int returnIntValueOrDefault(int defaultValue);
  virtual unsigned int unsignedIntReturnValue();
  virtual long int longIntReturnValue();
  virtual long int returnLongIntValueOrDefault(long int defaultValue);
  virtual unsigned long int unsignedLongIntReturnValue();
  virtual unsigned long int returnUnsignedLongIntValueOrDefault(
    unsigned long int defaultValue);
  virtual long long longLongIntReturnValue();
  virtual long long returnLongLongIntValueOrDefault(long long defaultValue);
  virtual unsigned long long unsignedLongLongIntReturnValue();
  virtual unsigned long long returnUnsignedLongLongIntValueOrDefault(
    unsigned long long defaultValue);
  virtual unsigned int returnUnsignedIntValueOrDefault(
    unsigned int defaultValue);
  virtual const char* stringReturnValue();
  virtual const char* returnStringValueOrDefault(const char* defaultValue);
  virtual double returnDoubleValueOrDefault(double defaultValue);
  virtual double doubleReturnValue();
  virtual void* pointerReturnValue();
  virtual void* returnPointerValueOrDefault(void* defaultValue);
  virtual const void* returnConstPointerValueOrDefault(
    const void* defaultValue);
  virtual const void* constPointerReturnValue();
  virtual void (*returnFunctionPointerValueOrDefault(void (*defaultValue)()))();
  virtual void (*functionPointerReturnValue())();

  bool hasData(const cpputest::String& name);
  void setData(const cpputest::String& name, bool value);
  void setData(const cpputest::String& name, int value);
  void setData(const cpputest::String& name, unsigned int value);
  void setData(const cpputest::String& name, long int value);
  void setData(const cpputest::String& name, unsigned long int value);
  void setData(const cpputest::String& name, const char* value);
  void setData(const cpputest::String& name, double value);
  void setData(const cpputest::String& name, void* value);
  void setData(const cpputest::String& name, const void* value);
  void setData(const cpputest::String& name, void (*value)());
  void setDataObject(const cpputest::String& name,
                     const cpputest::String& type,
                     void* value);
  void setDataConstObject(const cpputest::String& name,
                          const cpputest::String& type,
                          const void* value);
  MockNamedValue getData(const cpputest::String& name);

  MockSupport* getMockSupportScope(const cpputest::String& name);

  const char* getTraceOutput();
  /*
   * The following functions are recursively through the lower MockSupports
   * scopes This means, if you do mock().disable() it will disable *all* mocking
   * scopes, including mock("myScope").
   */

  virtual void disable();
  virtual void enable();
  virtual void tracing(bool enabled);
  virtual void ignoreOtherCalls();

  virtual void checkExpectations();
  virtual bool expectedCallsLeft();

  virtual void clear();
  virtual void crashOnFailure(bool shouldFail = true);

  /*
   * Each mock() call will set the activeReporter to standard, unless a special
   * reporter is passed for this call.
   */

  virtual void setMockFailureStandardReporter(MockFailureReporter* reporter);
  virtual void setActiveReporter(MockFailureReporter* activeReporter);
  virtual void setDefaultComparatorsAndCopiersRepository();

  virtual void installComparator(const cpputest::String& typeName,
                                 MockNamedValueComparator& comparator);
  virtual void installCopier(const cpputest::String& typeName,
                             MockNamedValueCopier& copier);
  virtual void installComparatorsAndCopiers(
    const MockNamedValueComparatorsAndCopiersRepository& repository);
  virtual void removeAllComparatorsAndCopiers();

protected:
  MockSupport* clone(const cpputest::String& mockName);
  virtual MockCheckedActualCall* createActualCall();
  virtual void failTest(MockFailure& failure);
  void countCheck();

private:
  unsigned int actualCallOrder_;
  unsigned int expectedCallOrder_;
  bool strictOrdering_;
  MockFailureReporter* activeReporter_;
  MockFailureReporter* standardReporter_;
  MockFailureReporter defaultReporter_;
  MockExpectedCallsList expectations_;
  bool ignoreOtherCalls_;
  bool enabled_;
  MockCheckedActualCall* lastActualFunctionCall_;
  MockNamedValueComparatorsAndCopiersRepository
    comparatorsAndCopiersRepository_;
  MockNamedValueList data_;
  const cpputest::String mockName_;

  bool tracing_;

  void checkExpectationsOfLastActualCall();
  bool wasLastActualCallFulfilled();
  void failTestWithExpectedCallsNotFulfilled();
  void failTestWithOutOfOrderCalls();

  MockNamedValue* retrieveDataFromStore(const cpputest::String& name);

  MockSupport* getMockSupport(MockNamedValueListNode* node);

  bool callIsIgnored(const cpputest::String& functionName);
  bool hasCallsOutOfOrder();

  cpputest::String appendScopeToName(const cpputest::String& functionName);
};

} // namespace extensions
} // namespace cpputest

#endif
