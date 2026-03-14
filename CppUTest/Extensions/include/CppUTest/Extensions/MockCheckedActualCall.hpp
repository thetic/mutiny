#ifndef D_MockCheckedActualCall_h
#define D_MockCheckedActualCall_h

#include "CppUTest/Extensions/MockActualCall.hpp"
#include "CppUTest/Extensions/MockExpectedCallsList.hpp"
#include "CppUTest/Extensions/MockFailure.hpp"

namespace cpputest {
namespace extensions {

class MockCheckedActualCall : public MockActualCall
{
public:
  MockCheckedActualCall(unsigned int callOrder,
      MockFailureReporter* reporter,
      const MockExpectedCallsList& expectations);
  virtual ~MockCheckedActualCall() override;

  virtual MockActualCall& withName(const cpputest::String& name) override;
  virtual MockActualCall& withCallOrder(unsigned int) override;
  virtual MockActualCall& withBoolParameter(const cpputest::String& name,
      bool value) override;
  virtual MockActualCall& withIntParameter(const cpputest::String& name,
      int value) override;
  virtual MockActualCall& withUnsignedIntParameter(const cpputest::String& name,
      unsigned int value) override;
  virtual MockActualCall& withLongIntParameter(const cpputest::String& name,
      long int value) override;
  virtual MockActualCall& withUnsignedLongIntParameter(
      const cpputest::String& name,
      unsigned long int value) override;
  virtual MockActualCall& withLongLongIntParameter(const cpputest::String& name,
      long long value) override;
  virtual MockActualCall& withUnsignedLongLongIntParameter(
      const cpputest::String& name,
      unsigned long long value) override;
  virtual MockActualCall& withDoubleParameter(const cpputest::String& name,
      double value) override;
  virtual MockActualCall& withStringParameter(const cpputest::String& name,
      const char* value) override;
  virtual MockActualCall& withPointerParameter(const cpputest::String& name,
      void* value) override;
  virtual MockActualCall& withConstPointerParameter(
      const cpputest::String& name,
      const void* value) override;
  virtual MockActualCall& withFunctionPointerParameter(
      const cpputest::String& name,
      void (*value)()) override;
  virtual MockActualCall& withMemoryBufferParameter(
      const cpputest::String& name,
      const unsigned char* value,
      size_t size) override;
  virtual MockActualCall& withMemoryBufferParameter(const char* name,
      const unsigned char* value,
      size_t size) override;
  virtual MockActualCall& withParameterOfType(const cpputest::String& type,
      const cpputest::String& name,
      const void* value) override;
  virtual MockActualCall& withParameterOfType(const char* typeName,
      const char* name,
      const void* value) override;
  virtual MockActualCall& withOutputParameter(const cpputest::String& name,
      void* output) override;
  virtual MockActualCall& withOutputParameterOfType(
      const cpputest::String& type,
      const cpputest::String& name,
      void* output) override;

  virtual bool hasReturnValue() override;
  virtual MockNamedValue returnValue() override;

  virtual bool returnBoolValueOrDefault(bool default_value) override;
  virtual bool returnBoolValue() override;

  virtual int returnIntValueOrDefault(int default_value) override;
  virtual int returnIntValue() override;

  virtual unsigned long int returnUnsignedLongIntValue() override;
  virtual unsigned long int returnUnsignedLongIntValueOrDefault(
      unsigned long int) override;

  virtual long int returnLongIntValue() override;
  virtual long int returnLongIntValueOrDefault(long int default_value) override;

  virtual unsigned long long returnUnsignedLongLongIntValue() override;
  virtual unsigned long long returnUnsignedLongLongIntValueOrDefault(
      unsigned long long default_value) override;

  virtual long long returnLongLongIntValue() override;
  virtual long long returnLongLongIntValueOrDefault(
      long long default_value) override;

  virtual unsigned int returnUnsignedIntValue() override;
  virtual unsigned int returnUnsignedIntValueOrDefault(
      unsigned int default_value) override;

  virtual const char* returnStringValueOrDefault(
      const char* default_value) override;
  virtual const char* returnStringValue() override;

  virtual double returnDoubleValue() override;
  virtual double returnDoubleValueOrDefault(double default_value) override;

  virtual const void* returnConstPointerValue() override;
  virtual const void* returnConstPointerValueOrDefault(
      const void* default_value) override;

  virtual void* returnPointerValue() override;
  virtual void* returnPointerValueOrDefault(void*) override;

  virtual FunctionPointerReturnValue returnFunctionPointerValue() override;
  virtual FunctionPointerReturnValue returnFunctionPointerValueOrDefault(
      void (*)()) override;

  virtual MockActualCall& onObject(const void* objectPtr) override;

  virtual bool isFulfilled() const;
  virtual bool hasFailed() const;

  virtual void checkExpectations();

  virtual void setMockFailureReporter(MockFailureReporter* reporter);
  void setNameAndCheck(cpputest::String name);

protected:
  void setName(const cpputest::String& name);
  const cpputest::String& getName() const;
  virtual cpputest::TestShell* getTest() const;
  virtual void callHasSucceeded();
  virtual void copyOutputParameters(MockCheckedExpectedCall* call);
  virtual void completeCallWhenMatchIsFound();
  void failWith(MockFailure failure);
  virtual void checkInputParameter(MockNamedValue actualParameter);
  virtual void checkOutputParameter(MockNamedValue outputParameter);
  virtual void discardCurrentlyMatchingExpectations();

  enum class ActualCallState
  {
    IN_PROGRESS,
    FAILED,
    SUCCESS
  };
  virtual void setState(ActualCallState state);

private:
  cpputest::String functionName_;
  unsigned int callOrder_;
  MockFailureReporter* reporter_;

  ActualCallState state_;
  bool expectationsChecked_;
  MockCheckedExpectedCall* matchingExpectation_;

  MockExpectedCallsList potentiallyMatchingExpectations_;
  const MockExpectedCallsList& allExpectations_;

  class MockOutputParametersListNode
  {
  public:
    cpputest::String name_;
    cpputest::String type_;
    void* ptr_;

    MockOutputParametersListNode* next_;
    MockOutputParametersListNode(const cpputest::String& name,
        const cpputest::String& type,
        void* ptr)
      : name_(name)
      , type_(type)
      , ptr_(ptr)
      , next_(nullptr)
    {
    }
  };

  MockOutputParametersListNode* outputParameterExpectations_;

  virtual void addOutputParameter(const cpputest::String& name,
      const cpputest::String& type,
      void* ptr);
  void cleanUpOutputParameterList();
};

} // namespace extensions
} // namespace cpputest

#endif
