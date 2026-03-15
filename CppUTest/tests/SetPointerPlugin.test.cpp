#include "CppUTest/SetPointerPlugin.hpp"

#include "CppUTest/CppUTest.hpp"
#include "CppUTest/StringBufferTestOutput.hpp"
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
  void test_body() override { CHECK(orig_double_ptr == &stub_double); }
};

class SetDoublePointerUtestShell : public cpputest::TestShell
{
public:
  cpputest::Test* create_test() override { return new SetDoublePointerUtest(); }
};
}

TEST_GROUP(SetPointerPlugin)
{
  cpputest::SetPointerPlugin* plugin;
  cpputest::TestRegistry* my_registry;
  cpputest::StringBufferTestOutput* output;
  cpputest::TestResult* result;

  void setup() override
  {
    my_registry = new cpputest::TestRegistry();
    plugin = new cpputest::SetPointerPlugin("TestSetPlugin");
    my_registry->set_current_registry(my_registry);
    my_registry->install_plugin(plugin);
    output = new cpputest::StringBufferTestOutput();
    result = new cpputest::TestResult(*output);
  }

  void teardown() override
  {
    my_registry->set_current_registry(nullptr);
    delete my_registry;
    delete plugin;
    delete output;
    delete result;
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
  void test_body() override
  {
    CHECK(fp1 == stub_func1);
    CHECK(fp2 == stub_func2);
  }
};

class FunctionPointerUtestShell : public cpputest::TestShell
{
public:
  virtual cpputest::Test* create_test() override
  {
    return new FunctionPointerUtest();
  }
};

TEST(SetPointerPlugin, installTwoFunctionPointer)
{
  auto* tst = new FunctionPointerUtestShell();

  fp1 = orig_func1;
  fp2 = orig_func2;
  my_registry->add_test(tst);
  my_registry->run_all_tests(*result);
  CHECK(fp1 == orig_func1);
  CHECK(fp2 == orig_func2);
  LONGS_EQUAL(0, result->get_failure_count());
  LONGS_EQUAL(2, result->get_check_count());
  delete tst;
}

class MaxFunctionPointerUtest : public cpputest::Test
{
public:
  int num_of_fp_sets;
  MaxFunctionPointerUtest(int num)
    : num_of_fp_sets(num)
  {
  }

  void setup() override
  {
    for (int i = 0; i < num_of_fp_sets; ++i) {
      UT_PTR_SET(fp1, stub_func1);
    }
  }
};

class MaxFunctionPointerUtestShell : public cpputest::TestShell
{
public:
  int num_of_fp_sets;
  MaxFunctionPointerUtestShell(int num)
    : num_of_fp_sets(num)
  {
  }

  virtual cpputest::Test* create_test() override
  {
    return new MaxFunctionPointerUtest(num_of_fp_sets);
  }
};

TEST(SetPointerPlugin, installTooMuchFunctionPointer)
{
  auto* tst =
      new MaxFunctionPointerUtestShell(cpputest::SetPointerPlugin::max_set + 1);
  my_registry->add_test(tst);

  my_registry->run_all_tests(*result);

  LONGS_EQUAL(1, result->get_failure_count());
  delete tst;
}

EXPECT_FAIL_TEST(SetPointerPlugin, tooManyPtrSets)
{
  for (int i = 0; i <= cpputest::SetPointerPlugin::max_set; ++i) {
    UT_PTR_SET(fp1, stub_func1);
  }
}

TEST(SetPointerPlugin, doublePointer)
{
  auto* doubletst = new SetDoublePointerUtestShell();
  my_registry->add_test(doubletst);
  my_registry->run_all_tests(*result);

  CHECK(orig_double_ptr == &orig_double);
  LONGS_EQUAL(1, result->get_check_count());
  delete doubletst;
}
