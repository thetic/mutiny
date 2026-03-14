#ifndef D_MockCheckedExpectedCall_h
#define D_MockCheckedExpectedCall_h

#include "CppUTest/Extensions/MockExpectedCall.hpp"
#include "CppUTest/Extensions/MockNamedValue.hpp"
#include "CppUTest/Extensions/MockNamedValueList.hpp"

namespace cpputest {
namespace extensions {

class MockCheckedExpectedCall : public MockExpectedCall
{

public:
  MockCheckedExpectedCall();
  MockCheckedExpectedCall(unsigned int numCalls);
  virtual ~MockCheckedExpectedCall() override;

  virtual MockExpectedCall& withName(const cpputest::String& name) override;
  virtual MockExpectedCall& withCallOrder(unsigned int callOrder) override
  {
    return withCallOrder(callOrder, callOrder);
  }
  virtual MockExpectedCall& withCallOrder(unsigned int initialCallOrder,
      unsigned int finalCallOrder) override;
  virtual MockExpectedCall& withBoolParameter(const cpputest::String& name,
      bool value) override;
  virtual MockExpectedCall& withIntParameter(const cpputest::String& name,
      int value) override;
  virtual MockExpectedCall& withUnsignedIntParameter(
      const cpputest::String& name,
      unsigned int value) override;
  virtual MockExpectedCall& withLongIntParameter(const cpputest::String& name,
      long int value) override;
  virtual MockExpectedCall& withUnsignedLongIntParameter(
      const cpputest::String& name,
      unsigned long int value) override;
  virtual MockExpectedCall& withLongLongIntParameter(
      const cpputest::String& name,
      long long value) override;
  virtual MockExpectedCall& withUnsignedLongLongIntParameter(
      const cpputest::String& name,
      unsigned long long value) override;
  virtual MockExpectedCall& withDoubleParameter(const cpputest::String& name,
      double value) override;
  virtual MockExpectedCall& withDoubleParameter(const cpputest::String& name,
      double value,
      double tolerance) override;
  virtual MockExpectedCall& withStringParameter(const cpputest::String& name,
      const char* value) override;
  virtual MockExpectedCall& withPointerParameter(const cpputest::String& name,
      void* value) override;
  virtual MockExpectedCall& withConstPointerParameter(
      const cpputest::String& name,
      const void* value) override;
  virtual MockExpectedCall& withFunctionPointerParameter(
      const cpputest::String& name,
      void (*value)()) override;
  virtual MockExpectedCall& withMemoryBufferParameter(
      const cpputest::String& name,
      const unsigned char* value,
      size_t size) override;
  virtual MockExpectedCall& withParameterOfType(
      const cpputest::String& typeName,
      const cpputest::String& name,
      const void* value) override;
  virtual MockExpectedCall& withOutputParameterReturning(
      const cpputest::String& name,
      const void* value,
      size_t size) override;
  virtual MockExpectedCall& withOutputParameterOfTypeReturning(
      const cpputest::String& typeName,
      const cpputest::String& name,
      const void* value) override;
  virtual MockExpectedCall& withUnmodifiedOutputParameter(
      const cpputest::String& name) override;
  virtual MockExpectedCall& ignoreOtherParameters() override;

  virtual MockExpectedCall& andReturnValue(bool value) override;
  virtual MockExpectedCall& andReturnValue(int value) override;
  virtual MockExpectedCall& andReturnValue(unsigned int value) override;
  virtual MockExpectedCall& andReturnValue(long int value) override;
  virtual MockExpectedCall& andReturnValue(unsigned long int value) override;
  virtual MockExpectedCall& andReturnValue(long long value) override;
  virtual MockExpectedCall& andReturnValue(unsigned long long value) override;
  virtual MockExpectedCall& andReturnValue(double value) override;
  virtual MockExpectedCall& andReturnValue(const char* value) override;
  virtual MockExpectedCall& andReturnValue(void* value) override;
  virtual MockExpectedCall& andReturnValue(const void* value) override;
  virtual MockExpectedCall& andReturnValue(void (*value)()) override;

  virtual MockNamedValue returnValue();

  virtual MockExpectedCall& onObject(void* objectPtr) override;

  virtual MockNamedValue getInputParameter(const cpputest::String& name);
  virtual MockNamedValue getOutputParameter(const cpputest::String& name);
  virtual cpputest::String getInputParameterType(const cpputest::String& name);
  virtual cpputest::String getInputParameterValueString(
      const cpputest::String& name);

  virtual bool hasInputParameterWithName(const cpputest::String& name);
  virtual bool hasInputParameter(const MockNamedValue& parameter);
  virtual bool hasOutputParameterWithName(const cpputest::String& name);
  virtual bool hasOutputParameter(const MockNamedValue& parameter);
  virtual bool relatesTo(const cpputest::String& functionName);
  virtual bool relatesToObject(const void* objectPtr) const;

  virtual bool isFulfilled();
  virtual bool canMatchActualCalls();
  virtual bool isMatchingActualCallAndFinalized();
  virtual bool isMatchingActualCall();
  virtual bool areParametersMatchingActualCall();
  virtual bool isOutOfOrder() const;

  virtual void callWasMade(unsigned int callOrder);
  virtual void inputParameterWasPassed(const cpputest::String& name);
  virtual void outputParameterWasPassed(const cpputest::String& name);
  virtual void finalizeActualCallMatch();
  virtual void wasPassedToObject();
  virtual void resetActualCallMatchingState();

  virtual cpputest::String callToString();
  virtual cpputest::String missingParametersToString();

  static constexpr unsigned int NO_EXPECTED_CALL_ORDER = 0;

  virtual unsigned int getActualCallsFulfilled() const;

protected:
  void setName(const cpputest::String& name);
  cpputest::String getName() const;

private:
  cpputest::String functionName_;

  class MockExpectedFunctionParameter : public MockNamedValue
  {
  public:
    MockExpectedFunctionParameter(const cpputest::String& name);
    void setMatchesActualCall(bool b);
    bool isMatchingActualCall() const;

  private:
    bool matchesActualCall_;
  };

  MockExpectedFunctionParameter* item(MockNamedValueListNode* node);

  bool ignoreOtherParameters_;
  bool isActualCallMatchFinalized_;
  unsigned int initialExpectedCallOrder_;
  unsigned int finalExpectedCallOrder_;
  bool outOfOrder_;
  MockNamedValueList* inputParameters_;
  MockNamedValueList* outputParameters_;
  MockNamedValue returnValue_;
  void* objectPtr_;
  bool isSpecificObjectExpected_;
  bool wasPassedToObject_;
  unsigned int actualCalls_;
  unsigned int expectedCalls_;
};

} // namespace extensions
} // namespace cpputest

#endif
