#ifndef INCLUDED_CPPUTEST_EXTENSIONS_MOCKSUPPORT_HPP
#define INCLUDED_CPPUTEST_EXTENSIONS_MOCKSUPPORT_HPP

#include "CppUTest/Extensions/MockCheckedActualCall.hpp"
#include "CppUTest/Extensions/MockCheckedExpectedCall.hpp"
#include "CppUTest/Extensions/MockExpectedCallsList.hpp"
#include "CppUTest/Extensions/MockFailure.hpp"
#include "CppUTest/Extensions/MockNamedValueComparatorsAndCopiersRepository.hpp"
#include "CppUTest/Extensions/MockNamedValueList.hpp"

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
