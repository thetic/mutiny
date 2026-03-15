#include "CppUTest/SetPointerPlugin.hpp"

#include "CppUTest/StringBufferTestOutput.hpp"
#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestOutput.hpp"
#include "CppUTest/TestRegistry.hpp"

namespace {
void
orig_func1()
{
}

void
stub_func1()
{
}

void
orig_func2()
{
}

void
stub_func2()
{
}

void (*fp1)();
void (*fp2)();

double orig_double = 3.0;
double* orig_double_ptr = &orig_double;
double stub_double = 4.0;

class SetDoublePointerUtest : public cpputest::Test
{
public:
  void setup() override { UT_PTR_SET(orig_double_ptr, &stub_double); }
  void testBody() override { CHECK(orig_double_ptr == &stub_double); }
};

class SetDoublePointerUtestShell : public cpputest::TestShell
{
public:
  cpputest::Test* createTest() override { return new SetDoublePointerUtest(); }
};
}

TEST_GROUP(SetPointerPlugin)
{
  cpputest::SetPointerPlugin* plugin_;
  cpputest::TestRegistry* myRegistry_;
  cpputest::StringBufferTestOutput* output_;
  cpputest::TestResult* result_;

  void setup() override
  {
    myRegistry_ = new cpputest::TestRegistry();
    plugin_ = new cpputest::SetPointerPlugin("TestSetPlugin");
    myRegistry_->setCurrentRegistry(myRegistry_);
    myRegistry_->installPlugin(plugin_);
    output_ = new cpputest::StringBufferTestOutput();
    result_ = new cpputest::TestResult(*output_);
  }

  void teardown() override
  {
    myRegistry_->setCurrentRegistry(nullptr);
    delete myRegistry_;
    delete plugin_;
    delete output_;
    delete result_;
  }
};

class FunctionPointerUtest : public cpputest::Test
{
public:
  void setup() override
  {
    UT_PTR_SET(fp1, stub_func1);
    UT_PTR_SET(fp2, stub_func2);
    UT_PTR_SET(fp2, stub_func2);
  }
  void testBody() override
  {
    CHECK(fp1 == stub_func1);
    CHECK(fp2 == stub_func2);
  }
};

class FunctionPointerUtestShell : public cpputest::TestShell
{
public:
  virtual cpputest::Test* createTest() override
  {
    return new FunctionPointerUtest();
  }
};

TEST(SetPointerPlugin, installTwoFunctionPointer)
{
  auto* tst = new FunctionPointerUtestShell();

  fp1 = orig_func1;
  fp2 = orig_func2;
  myRegistry_->addTest(tst);
  myRegistry_->runAllTests(*result_);
  CHECK(fp1 == orig_func1);
  CHECK(fp2 == orig_func2);
  LONGS_EQUAL(0, result_->getFailureCount());
  LONGS_EQUAL(2, result_->getCheckCount());
  delete tst;
}

class MaxFunctionPointerUtest : public cpputest::Test
{
public:
  int numOfFpSets;
  MaxFunctionPointerUtest(int num)
    : numOfFpSets(num)
  {
  }

  void setup() override
  {
    for (int i = 0; i < numOfFpSets; ++i) {
      UT_PTR_SET(fp1, stub_func1);
    }
  }
};

class MaxFunctionPointerUtestShell : public cpputest::TestShell
{
public:
  int numOfFpSets;
  MaxFunctionPointerUtestShell(int num)
    : numOfFpSets(num)
  {
  }

  virtual cpputest::Test* createTest() override
  {
    return new MaxFunctionPointerUtest(numOfFpSets);
  }
};

TEST(SetPointerPlugin, installTooMuchFunctionPointer)
{
  auto* tst =
      new MaxFunctionPointerUtestShell(cpputest::SetPointerPlugin::MAX_SET + 1);
  myRegistry_->addTest(tst);

  myRegistry_->runAllTests(*result_);

  LONGS_EQUAL(1, result_->getFailureCount());
  delete tst;
}

EXPECT_FAIL_TEST(SetPointerPlugin, tooManyPtrSets)
{
  for (int i = 0; i <= cpputest::SetPointerPlugin::MAX_SET; ++i) {
    UT_PTR_SET(fp1, stub_func1);
  }
}

TEST(SetPointerPlugin, doublePointer)
{
  auto* doubletst = new SetDoublePointerUtestShell();
  myRegistry_->addTest(doubletst);
  myRegistry_->runAllTests(*result_);

  CHECK(orig_double_ptr == &orig_double);
  LONGS_EQUAL(1, result_->getCheckCount());
  delete doubletst;
}
