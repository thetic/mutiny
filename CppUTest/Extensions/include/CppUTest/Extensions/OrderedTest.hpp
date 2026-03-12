#ifndef D_OrderedTest_h
#define D_OrderedTest_h

#include "CppUTest/TestShell.hpp"

namespace cpputest {
namespace extensions {

class OrderedTestShell : public cpputest::TestShell
{
public:
  OrderedTestShell();
  virtual ~OrderedTestShell() override;

  virtual OrderedTestShell* addOrderedTest(OrderedTestShell* test);
  virtual OrderedTestShell* getNextOrderedTest();

  int getLevel();
  void setLevel(int level);

  static void addOrderedTestToHead(OrderedTestShell* test);
  static OrderedTestShell* getOrderedTestHead();
  static bool firstOrderedTest();

  static void setOrderedTestHead(OrderedTestShell* test);

private:
  static OrderedTestShell* _orderedTestsHead;
  OrderedTestShell* _nextOrderedTest;

  int _level;
};

class OrderedTestInstaller
{
public:
  explicit OrderedTestInstaller(OrderedTestShell& test,
      const char* groupName,
      const char* testName,
      const char* fileName,
      size_t lineNumber,
      int level);
  virtual ~OrderedTestInstaller();

private:
  void addOrderedTestInOrder(OrderedTestShell* test);
  void addOrderedTestInOrderNotAtHeadPosition(OrderedTestShell* test);
};

} // namespace extensions
} // namespace cpputest

#define TEST_ORDERED(testGroup, testName, testLevel)                           \
  /* declarations for compilers */                                             \
  class TEST_##testGroup##_##testName##_TestShell;                             \
  extern TEST_##testGroup##_##testName##_TestShell                             \
      TEST_##testGroup##_##testName##_Instance;                                \
  class TEST_##testGroup##_##testName##_Test                                   \
    : public TEST_GROUP_##CppUTestGroup##testGroup                             \
  {                                                                            \
  public:                                                                      \
    TEST_##testGroup##_##testName##_Test()                                     \
      : TEST_GROUP_##CppUTestGroup##testGroup()                                \
    {                                                                          \
    }                                                                          \
    void testBody() override;                                                  \
  };                                                                           \
  class TEST_##testGroup##_##testName##_TestShell                              \
    : public cpputest::extensions::OrderedTestShell                            \
  {                                                                            \
    virtual cpputest::Test* createTest() override                              \
    {                                                                          \
      return new TEST_##testGroup##_##testName##_Test;                         \
    }                                                                          \
  } TEST_##testGroup##_##testName##_Instance;                                  \
  static cpputest::extensions::OrderedTestInstaller                            \
      TEST_##testGroup##_##testName##_Installer(                               \
          TEST_##testGroup##_##testName##_Instance,                            \
          #testGroup,                                                          \
          #testName,                                                           \
          __FILE__,                                                            \
          __LINE__,                                                            \
          testLevel);                                                          \
  void TEST_##testGroup##_##testName##_Test::testBody()

#define TEST_ORDERED_C_WRAPPER(group_name, test_name, testLevel)               \
  extern "C" void test_##group_name##_##test_name##_wrapper_c(void);           \
  TEST_ORDERED(group_name, test_name, testLevel)                               \
  {                                                                            \
    test_##group_name##_##test_name##_wrapper_c();                             \
  }

#endif
