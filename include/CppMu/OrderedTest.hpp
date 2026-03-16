#ifndef INCLUDED_CPPMU_ORDEREDTEST_HPP
#define INCLUDED_CPPMU_ORDEREDTEST_HPP

#include "CppMu/TestShell.hpp"

namespace cppmu {

class OrderedTestShell : public cppmu::TestShell
{
public:
  OrderedTestShell();
  ~OrderedTestShell() override;

  virtual OrderedTestShell* add_ordered_test(OrderedTestShell* test);
  virtual OrderedTestShell* get_next_ordered_test();

  bool is_ordered() const override;

  int get_level();
  void set_level(int level);

  static void add_ordered_test_to_head(OrderedTestShell* test);
  static OrderedTestShell* get_ordered_test_head();
  static bool first_ordered_test();

  static void set_ordered_test_head(OrderedTestShell* test);

private:
  static OrderedTestShell* ordered_tests_head_;
  OrderedTestShell* next_ordered_test_;

  int level_;
};

class OrderedTestInstaller
{
public:
  explicit OrderedTestInstaller(OrderedTestShell& test,
      const char* group_name,
      const char* test_name,
      const char* file_name,
      size_t line_number,
      int level);
  virtual ~OrderedTestInstaller();

private:
  void add_ordered_test_in_order(OrderedTestShell* test);
  void add_ordered_test_in_order_not_at_head_position(OrderedTestShell* test);
};

} // namespace cppmu

#define TEST_ORDERED(testGroup, testName, testLevel)                           \
  /* declarations for compilers */                                             \
  class TEST_##testGroup##_##testName##_TestShell;                             \
  extern TEST_##testGroup##_##testName##_TestShell                             \
      TEST_##testGroup##_##testName##_Instance;                                \
  class TEST_##testGroup##_##testName##_Test                                   \
    : public TEST_GROUP_##CppMuGroup##testGroup                                \
  {                                                                            \
  public:                                                                      \
    TEST_##testGroup##_##testName##_Test()                                     \
      : TEST_GROUP_##CppMuGroup##testGroup()                                   \
    {                                                                          \
    }                                                                          \
    void test_body() override;                                                 \
  };                                                                           \
  class TEST_##testGroup##_##testName##_TestShell                              \
    : public cppmu::OrderedTestShell                                           \
  {                                                                            \
    virtual cppmu::Test* create_test() override                                \
    {                                                                          \
      return new TEST_##testGroup##_##testName##_Test;                         \
    }                                                                          \
  } TEST_##testGroup##_##testName##_Instance;                                  \
  namespace {                                                                  \
  cppmu::OrderedTestInstaller TEST_##testGroup##_##testName##_Installer(       \
      TEST_##testGroup##_##testName##_Instance,                                \
      #testGroup,                                                              \
      #testName,                                                               \
      __FILE__,                                                                \
      __LINE__,                                                                \
      testLevel);                                                              \
  } /* namespace */                                                            \
  void TEST_##testGroup##_##testName##_Test::test_body()

#define TEST_ORDERED_C_WRAPPER(group_name, test_name, testLevel)               \
  extern "C" void test_##group_name##_##test_name##_wrapper_c(void);           \
  TEST_ORDERED(group_name, test_name, testLevel)                               \
  {                                                                            \
    test_##group_name##_##test_name##_wrapper_c();                             \
  }

#endif
