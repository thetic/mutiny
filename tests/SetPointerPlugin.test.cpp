#include "CppMu/SetPointerPlugin.hpp"

#include "CppMu/CppMu.hpp"
#include "CppMu/StringBufferTestOutput.hpp"
#include "CppMu/TestOutput.hpp"
#include "CppMu/TestRegistry.hpp"

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

class SetDoublePointerGroup : public cppmu::Test
{
public:
  void setup() override { CPPMU_PTR_SET(orig_double_ptr, &stub_double); }
  void test_body() override { CHECK(orig_double_ptr == &stub_double); }
};

class SetDoublePointerShell : public cppmu::TestShell
{
public:
  cppmu::Test* create_test() override { return new SetDoublePointerGroup(); }
};
}

TEST_GROUP(SetPointerPlugin)
{
  cppmu::SetPointerPlugin* plugin;
  cppmu::TestRegistry* my_registry;
  cppmu::StringBufferTestOutput* output;
  cppmu::TestResult* result;

  void setup() override
  {
    my_registry = new cppmu::TestRegistry();
    plugin = new cppmu::SetPointerPlugin("TestSetPlugin");
    my_registry->set_current_registry(my_registry);
    my_registry->install_plugin(plugin);
    output = new cppmu::StringBufferTestOutput();
    result = new cppmu::TestResult(*output);
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

class FunctionPointerGroup : public cppmu::Test
{
public:
  void setup() override
  {
    CPPMU_PTR_SET(fp1, stub_func1);
    CPPMU_PTR_SET(fp2, stub_func2);
    CPPMU_PTR_SET(fp2, stub_func2);
  }
  void test_body() override
  {
    CHECK(fp1 == stub_func1);
    CHECK(fp2 == stub_func2);
  }
};

class FunctionPointerShell : public cppmu::TestShell
{
public:
  virtual cppmu::Test* create_test() override
  {
    return new FunctionPointerGroup();
  }
};

TEST(SetPointerPlugin, installTwoFunctionPointer)
{
  auto* tst = new FunctionPointerShell();

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

class MaxFunctionPointerGroup : public cppmu::Test
{
public:
  int num_of_fp_sets;
  MaxFunctionPointerGroup(int num)
    : num_of_fp_sets(num)
  {
  }

  void setup() override
  {
    for (int i = 0; i < num_of_fp_sets; ++i) {
      CPPMU_PTR_SET(fp1, stub_func1);
    }
  }
};

class MaxFunctionPointerShell : public cppmu::TestShell
{
public:
  int num_of_fp_sets;
  MaxFunctionPointerShell(int num)
    : num_of_fp_sets(num)
  {
  }

  virtual cppmu::Test* create_test() override
  {
    return new MaxFunctionPointerGroup(num_of_fp_sets);
  }
};

TEST(SetPointerPlugin, installTooMuchFunctionPointer)
{
  auto* tst = new MaxFunctionPointerShell(cppmu::SetPointerPlugin::max_set + 1);
  my_registry->add_test(tst);

  my_registry->run_all_tests(*result);

  LONGS_EQUAL(1, result->get_failure_count());
  delete tst;
}

EXPECT_FAIL_TEST(SetPointerPlugin, tooManyPtrSets)
{
  for (int i = 0; i <= cppmu::SetPointerPlugin::max_set; ++i) {
    CPPMU_PTR_SET(fp1, stub_func1);
  }
}

TEST(SetPointerPlugin, doublePointer)
{
  auto* doubletst = new SetDoublePointerShell();
  my_registry->add_test(doubletst);
  my_registry->run_all_tests(*result);

  CHECK(orig_double_ptr == &orig_double);
  LONGS_EQUAL(1, result->get_check_count());
  delete doubletst;
}
