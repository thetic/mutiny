#ifndef INCLUDED_CPPMU_CPPMU_HPP
#define INCLUDED_CPPMU_CPPMU_HPP

#include "CppMu/ExpectFailTestShell.hpp"
#include "CppMu/IgnoredTestShell.hpp"
#include "CppMu/SetPointerPlugin.hpp"
#include "CppMu/String.hpp"
#include "CppMu/Test.hpp"
#include "CppMu/TestFailure.hpp"
#include "CppMu/TestInstaller.hpp"
#include "CppMu/TestPlugin.hpp"
#include "CppMu/TestResult.hpp"
#include "CppMu/TestShell.hpp"
#include "CppMu/TestShellPointerArray.hpp"

/*! \brief Define a group of tests
 *
 * All tests in a TEST_GROUP share the same setup()
 * and teardown().  setup() is run before the opening
 * curly brace of each TEST and teardown() is
 * called after the closing curly brace of TEST.
 *
 */

#define TEST_GROUP_BASE(testGroup, baseclass)                                  \
  extern int externTestGroup##testGroup;                                       \
  int externTestGroup##testGroup = 0;                                          \
  struct TEST_GROUP_##CppMuGroup##testGroup : public baseclass

#define TEST_BASE(testBaseClass) struct testBaseClass : public cppmu::Test

#define TEST_GROUP(testGroup) TEST_GROUP_BASE(testGroup, cppmu::Test)

#define TEST_SETUP() virtual void setup() override

#define TEST_TEARDOWN() virtual void teardown() override

#define TEST(testGroup, testName)                                              \
  /* External declarations for strict compilers */                             \
  class TEST_##testGroup##_##testName##_TestShell;                             \
  extern TEST_##testGroup##_##testName##_TestShell                             \
      TEST_##testGroup##_##testName##_TestShell_instance;                      \
                                                                               \
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
  class TEST_##testGroup##_##testName##_TestShell : public cppmu::TestShell    \
  {                                                                            \
    virtual cppmu::Test* create_test() override                                \
    {                                                                          \
      return new TEST_##testGroup##_##testName##_Test;                         \
    }                                                                          \
  } TEST_##testGroup##_##testName##_TestShell_instance;                        \
  namespace {                                                                  \
  cppmu::TestInstaller TEST_##testGroup##_##testName##_Installer(              \
      TEST_##testGroup##_##testName##_TestShell_instance,                      \
      #testGroup,                                                              \
      #testName,                                                               \
      __FILE__,                                                                \
      __LINE__);                                                               \
  } /* namespace */                                                            \
  void TEST_##testGroup##_##testName##_Test::test_body()

#define IGNORE_TEST(testGroup, testName)                                       \
  /* External declarations for strict compilers */                             \
  class IGNORE##testGroup##_##testName##_TestShell;                            \
  extern IGNORE##testGroup##_##testName##_TestShell                            \
      IGNORE##testGroup##_##testName##_TestShell_instance;                     \
                                                                               \
  class IGNORE##testGroup##_##testName##_Test                                  \
    : public TEST_GROUP_##CppMuGroup##testGroup                                \
  {                                                                            \
  public:                                                                      \
    IGNORE##testGroup##_##testName##_Test()                                    \
      : TEST_GROUP_##CppMuGroup##testGroup()                                   \
    {                                                                          \
    }                                                                          \
                                                                               \
  public:                                                                      \
    void test_body() override;                                                 \
  };                                                                           \
  class IGNORE##testGroup##_##testName##_TestShell                             \
    : public cppmu::IgnoredTestShell                                           \
  {                                                                            \
    virtual cppmu::Test* create_test() override                                \
    {                                                                          \
      return new IGNORE##testGroup##_##testName##_Test;                        \
    }                                                                          \
  } IGNORE##testGroup##_##testName##_TestShell_instance;                       \
  namespace {                                                                  \
  cppmu::TestInstaller TEST_##testGroup##testName##_Installer(                 \
      IGNORE##testGroup##_##testName##_TestShell_instance,                     \
      #testGroup,                                                              \
      #testName,                                                               \
      __FILE__,                                                                \
      __LINE__);                                                               \
  } /* namespace */                                                            \
  void IGNORE##testGroup##_##testName##_Test::test_body()

#define EXPECT_FAIL_TEST(testGroup, testName)                                  \
  /* External declarations for strict compilers */                             \
  class EXPECT_FAIL##testGroup##_##testName##_TestShell;                       \
  extern EXPECT_FAIL##testGroup##_##testName##_TestShell                       \
      EXPECT_FAIL##testGroup##_##testName##_TestShell_instance;                \
                                                                               \
  class EXPECT_FAIL##testGroup##_##testName##_Test                             \
    : public TEST_GROUP_##CppMuGroup##testGroup                                \
  {                                                                            \
  public:                                                                      \
    EXPECT_FAIL##testGroup##_##testName##_Test()                               \
      : TEST_GROUP_##CppMuGroup##testGroup()                                   \
    {                                                                          \
    }                                                                          \
    void test_body() override;                                                 \
  };                                                                           \
  class EXPECT_FAIL##testGroup##_##testName##_TestShell                        \
    : public cppmu::ExpectFailTestShell                                        \
  {                                                                            \
    virtual cppmu::Test* create_test() override                                \
    {                                                                          \
      return new EXPECT_FAIL##testGroup##_##testName##_Test;                   \
    }                                                                          \
  } EXPECT_FAIL##testGroup##_##testName##_TestShell_instance;                  \
  namespace {                                                                  \
  cppmu::TestInstaller EXPECT_FAIL##testGroup##_##testName##_Installer(        \
      EXPECT_FAIL##testGroup##_##testName##_TestShell_instance,                \
      #testGroup,                                                              \
      #testName,                                                               \
      __FILE__,                                                                \
      __LINE__);                                                               \
  } /* namespace */                                                            \
  void EXPECT_FAIL##testGroup##_##testName##_Test::test_body()

#endif
