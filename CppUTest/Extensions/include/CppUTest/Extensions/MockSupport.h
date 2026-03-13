#ifndef D_MockSupport_c_h
#define D_MockSupport_c_h

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>

  enum MockValueType
  {
    MOCKVALUETYPE_BOOL,
    MOCKVALUETYPE_UNSIGNED_INTEGER,
    MOCKVALUETYPE_INTEGER,
    MOCKVALUETYPE_LONG_INTEGER,
    MOCKVALUETYPE_UNSIGNED_LONG_INTEGER,
    MOCKVALUETYPE_LONG_LONG_INTEGER,
    MOCKVALUETYPE_UNSIGNED_LONG_LONG_INTEGER,
    MOCKVALUETYPE_DOUBLE,
    MOCKVALUETYPE_STRING,
    MOCKVALUETYPE_POINTER,
    MOCKVALUETYPE_CONST_POINTER,
    MOCKVALUETYPE_FUNCTIONPOINTER,
    MOCKVALUETYPE_MEMORYBUFFER,
    MOCKVALUETYPE_OBJECT,
    MOCKVALUETYPE_CONST_OBJECT
  };

  struct MockValue
  {
    enum MockValueType type;
    union
    {
      int boolValue;
      int intValue;
      unsigned int unsignedIntValue;
      long int longIntValue;
      unsigned long int unsignedLongIntValue;
      long long longLongIntValue;
      unsigned long long unsignedLongLongIntValue;
      double doubleValue;
      const char* stringValue;
      void* pointerValue;
      const void* constPointerValue;
      void (*functionPointerValue)(void);
      const unsigned char* memoryBufferValue;
      void* objectValue;
      const void* constObjectValue;
    } value;
  };

  struct MockActualCall
  {
    struct MockActualCall* (*withBoolParameters)(const char* name, int value);
    struct MockActualCall* (*withIntParameters)(const char* name, int value);
    struct MockActualCall* (
        *withUnsignedIntParameters)(const char* name, unsigned int value);
    struct MockActualCall* (
        *withLongIntParameters)(const char* name, long int value);
    struct MockActualCall* (*withUnsignedLongIntParameters)(const char* name,
        unsigned long int value);
    struct MockActualCall* (
        *withLongLongIntParameters)(const char* name, long long value);
    struct MockActualCall* (*withUnsignedLongLongIntParameters)(
        const char* name,
        unsigned long long value);
    struct MockActualCall* (
        *withDoubleParameters)(const char* name, double value);
    struct MockActualCall* (
        *withStringParameters)(const char* name, const char* value);
    struct MockActualCall* (
        *withPointerParameters)(const char* name, void* value);
    struct MockActualCall* (
        *withConstPointerParameters)(const char* name, const void* value);
    struct MockActualCall* (
        *withFunctionPointerParameters)(const char* name, void (*value)(void));
    struct MockActualCall* (*withMemoryBufferParameter)(const char* name,
        const unsigned char* value,
        size_t size);
    struct MockActualCall* (*withParameterOfType)(const char* type,
        const char* name,
        const void* value);
    struct MockActualCall* (
        *withOutputParameter)(const char* name, void* value);
    struct MockActualCall* (*withOutputParameterOfType)(const char* type,
        const char* name,
        void* value);
    int (*hasReturnValue)(void);
    struct MockValue (*returnValue)(void);
    int (*boolReturnValue)(void);
    int (*returnBoolValueOrDefault)(int defaultValue);
    int (*intReturnValue)(void);
    int (*returnIntValueOrDefault)(int defaultValue);
    unsigned int (*unsignedIntReturnValue)(void);
    unsigned int (*returnUnsignedIntValueOrDefault)(unsigned int defaultValue);
    long int (*longIntReturnValue)(void);
    long int (*returnLongIntValueOrDefault)(long int defaultValue);
    unsigned long int (*unsignedLongIntReturnValue)(void);
    unsigned long int (*returnUnsignedLongIntValueOrDefault)(
        unsigned long int defaultValue);
    long long (*longLongIntReturnValue)(void);
    long long (*returnLongLongIntValueOrDefault)(long long defaultValue);
    unsigned long long (*unsignedLongLongIntReturnValue)(void);
    unsigned long long (*returnUnsignedLongLongIntValueOrDefault)(
        unsigned long long defaultValue);
    const char* (*stringReturnValue)(void);
    const char* (*returnStringValueOrDefault)(const char* defaultValue);
    double (*doubleReturnValue)(void);
    double (*returnDoubleValueOrDefault)(double defaultValue);
    void* (*pointerReturnValue)(void);
    void* (*returnPointerValueOrDefault)(void* defaultValue);
    const void* (*constPointerReturnValue)(void);
    const void* (*returnConstPointerValueOrDefault)(const void* defaultValue);
    void (*(*functionPointerReturnValue)(void))(void);
    void (*(*returnFunctionPointerValueOrDefault)(void (*defaultValue)(void)))(
        void);
  };

  struct MockExpectedCall
  {
    struct MockExpectedCall* (*withBoolParameters)(const char* name, int value);
    struct MockExpectedCall* (*withIntParameters)(const char* name, int value);
    struct MockExpectedCall* (
        *withUnsignedIntParameters)(const char* name, unsigned int value);
    struct MockExpectedCall* (
        *withLongIntParameters)(const char* name, long int value);
    struct MockExpectedCall* (*withUnsignedLongIntParameters)(const char* name,
        unsigned long int value);
    struct MockExpectedCall* (
        *withLongLongIntParameters)(const char* name, long long value);
    struct MockExpectedCall* (*withUnsignedLongLongIntParameters)(
        const char* name,
        unsigned long long value);
    struct MockExpectedCall* (
        *withDoubleParameters)(const char* name, double value);
    struct MockExpectedCall* (*withDoubleParametersAndTolerance)(
        const char* name,
        double value,
        double tolerance);
    struct MockExpectedCall* (
        *withStringParameters)(const char* name, const char* value);
    struct MockExpectedCall* (
        *withPointerParameters)(const char* name, void* value);
    struct MockExpectedCall* (
        *withConstPointerParameters)(const char* name, const void* value);
    struct MockExpectedCall* (
        *withFunctionPointerParameters)(const char* name, void (*value)(void));
    struct MockExpectedCall* (*withMemoryBufferParameter)(const char* name,
        const unsigned char* value,
        size_t size);
    struct MockExpectedCall* (*withParameterOfType)(const char* type,
        const char* name,
        const void* value);
    struct MockExpectedCall* (*withOutputParameterReturning)(const char* name,
        const void* value,
        size_t size);
    struct MockExpectedCall* (*withOutputParameterOfTypeReturning)(
        const char* type,
        const char* name,
        const void* value);
    struct MockExpectedCall* (*withUnmodifiedOutputParameter)(const char* name);
    struct MockExpectedCall* (*ignoreOtherParameters)(void);

    struct MockExpectedCall* (*andReturnBoolValue)(int value);
    struct MockExpectedCall* (*andReturnUnsignedIntValue)(unsigned int value);
    struct MockExpectedCall* (*andReturnIntValue)(int value);
    struct MockExpectedCall* (*andReturnLongIntValue)(long int value);
    struct MockExpectedCall* (*andReturnUnsignedLongIntValue)(
        unsigned long int value);
    struct MockExpectedCall* (*andReturnLongLongIntValue)(long long value);
    struct MockExpectedCall* (*andReturnUnsignedLongLongIntValue)(
        unsigned long long value);
    struct MockExpectedCall* (*andReturnDoubleValue)(double value);
    struct MockExpectedCall* (*andReturnStringValue)(const char* value);
    struct MockExpectedCall* (*andReturnPointerValue)(void* value);
    struct MockExpectedCall* (*andReturnConstPointerValue)(const void* value);
    struct MockExpectedCall* (*andReturnFunctionPointerValue)(
        void (*value)(void));
  };

  typedef int (
      *MockTypeEqualFunction)(const void* object1, const void* object2);
  typedef const char* (*MockTypeValueToStringFunction)(const void* object1);
  typedef void (*MockTypeCopyFunction)(void* dst, const void* src);

  struct MockSupport
  {
    void (*strictOrder)(void);
    struct MockExpectedCall* (*expectOneCall)(const char* name);
    void (*expectNoCall)(const char* name);
    struct MockExpectedCall* (
        *expectNCalls)(unsigned int number, const char* name);
    struct MockActualCall* (*actualCall)(const char* name);
    int (*hasReturnValue)(void);
    struct MockValue (*returnValue)(void);
    int (*boolReturnValue)(void);
    int (*returnBoolValueOrDefault)(int defaultValue);
    int (*intReturnValue)(void);
    int (*returnIntValueOrDefault)(int defaultValue);
    unsigned int (*unsignedIntReturnValue)(void);
    unsigned int (*returnUnsignedIntValueOrDefault)(unsigned int defaultValue);
    long int (*longIntReturnValue)(void);
    long int (*returnLongIntValueOrDefault)(long int defaultValue);
    unsigned long int (*unsignedLongIntReturnValue)(void);
    unsigned long int (*returnUnsignedLongIntValueOrDefault)(
        unsigned long int defaultValue);
    long long (*longLongIntReturnValue)(void);
    long long (*returnLongLongIntValueOrDefault)(long long defaultValue);
    unsigned long long (*unsignedLongLongIntReturnValue)(void);
    unsigned long long (*returnUnsignedLongLongIntValueOrDefault)(
        unsigned long long defaultValue);
    const char* (*stringReturnValue)(void);
    const char* (*returnStringValueOrDefault)(const char* defaultValue);
    double (*doubleReturnValue)(void);
    double (*returnDoubleValueOrDefault)(double defaultValue);
    void* (*pointerReturnValue)(void);
    void* (*returnPointerValueOrDefault)(void* defaultValue);
    const void* (*constPointerReturnValue)(void);
    const void* (*returnConstPointerValueOrDefault)(const void* defaultValue);
    void (*(*functionPointerReturnValue)(void))(void);
    void (*(*returnFunctionPointerValueOrDefault)(void (*defaultValue)(void)))(
        void);

    void (*setBoolData)(const char* name, int value);
    void (*setIntData)(const char* name, int value);
    void (*setUnsignedIntData)(const char* name, unsigned int value);
    void (*setLongIntData)(const char* name, long int value);
    void (*setUnsignedLongIntData)(const char* name, unsigned long int value);
    void (*setStringData)(const char* name, const char* value);
    void (*setDoubleData)(const char* name, double value);
    void (*setPointerData)(const char* name, void* value);
    void (*setConstPointerData)(const char* name, const void* value);
    void (*setFunctionPointerData)(const char* name, void (*value)(void));
    void (*setDataObject)(const char* name, const char* type, void* value);
    void (*setDataConstObject)(const char* name,
        const char* type,
        const void* value);
    struct MockValue (*getData)(const char* name);

    void (*disable)(void);
    void (*enable)(void);
    void (*ignoreOtherCalls)(void);

    void (*checkExpectations)(void);
    int (*expectedCallsLeft)(void);

    void (*clear)(void);
    void (*crashOnFailure)(unsigned shouldCrash);

    void (*installComparator)(const char* typeName,
        MockTypeEqualFunction isEqual,
        MockTypeValueToStringFunction valueToString);
    void (*installCopier)(const char* typeName, MockTypeCopyFunction copier);
    void (*removeAllComparatorsAndCopiers)(void);
  };

  struct MockSupport* mock(void);
  struct MockSupport* mock_scope(const char* scope);

#ifdef __cplusplus
}
#endif

#endif
