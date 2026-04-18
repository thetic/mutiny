#include "mu/tiny/test/SetPointerPlugin.hpp"

#include "mu/tiny/test/Output.hpp"
#include "mu/tiny/test/Registry.hpp"
#include "mu/tiny/test/StringBufferOutput.hpp"

#include "mu/tiny/test.hpp"

namespace {
void orig_func1() {}

void stub_func1() {}

void orig_func2() {}

void stub_func2() {}

void (*fp1)();
void (*fp2)();

double orig_double = 3.0;
double* orig_double_ptr = &orig_double;
double stub_double = 4.0;

class SetDoublePointerGroup : public mu::tiny::test::Test
{
public:
  void setup() override { MUTINY_PTR_SET(orig_double_ptr, &stub_double); }
  void test_body() override { CHECK(orig_double_ptr == &stub_double); }
};

class SetDoublePointerShell : public mu::tiny::test::Shell
{
public:
  mu::tiny::test::Test* create_test() override
  {
    return new SetDoublePointerGroup();
  }
};
} // namespace

TEST_GROUP(SetPointerPlugin)
{
  mu::tiny::test::SetPointerPlugin* plugin;
  mu::tiny::test::Registry* my_registry;
  mu::tiny::test::StringBufferOutput* output;
  mu::tiny::test::Result* result;

  void setup() override
  {
    my_registry = new mu::tiny::test::Registry();
    plugin = new mu::tiny::test::SetPointerPlugin;
    my_registry->set_current_registry(my_registry);
    my_registry->install_plugin(plugin);
    output = new mu::tiny::test::StringBufferOutput();
    result = new mu::tiny::test::Result(*output);
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

class FunctionPointerGroup : public mu::tiny::test::Test
{
public:
  void setup() override
  {
    MUTINY_PTR_SET(fp1, stub_func1);
    MUTINY_PTR_SET(fp2, stub_func2);
    MUTINY_PTR_SET(fp2, stub_func2);
  }
  void test_body() override
  {
    CHECK(fp1 == stub_func1);
    CHECK(fp2 == stub_func2);
  }
};

class FunctionPointerShell : public mu::tiny::test::Shell
{
public:
  mu::tiny::test::Test* create_test() override
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
  CHECK_EQUAL(size_t{ 0 }, result->get_failure_count());
  CHECK_EQUAL(size_t{ 2 }, result->get_check_count());
  delete tst;
}

class MaxFunctionPointerGroup : public mu::tiny::test::Test
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
      MUTINY_PTR_SET(fp1, stub_func1);
    }
  }
};

class MaxFunctionPointerShell : public mu::tiny::test::Shell
{
public:
  int num_of_fp_sets;
  MaxFunctionPointerShell(int num)
    : num_of_fp_sets(num)
  {
  }

  mu::tiny::test::Test* create_test() override
  {
    return new MaxFunctionPointerGroup(num_of_fp_sets);
  }
};

TEST(SetPointerPlugin, installTooMuchFunctionPointer)
{
  auto* tst = new MaxFunctionPointerShell(
      mu::tiny::test::SetPointerPlugin::max_set + 1
  );
  my_registry->add_test(tst);

  my_registry->run_all_tests(*result);

  CHECK_EQUAL(size_t{ 1 }, result->get_failure_count());
  delete tst;
}

XFAIL_TEST(SetPointerPlugin, tooManyPtrSets)
{
  for (size_t i{ 0 }; i <= mu::tiny::test::SetPointerPlugin::max_set; ++i) {
    MUTINY_PTR_SET(fp1, stub_func1);
  }
}

TEST(SetPointerPlugin, doublePointer)
{
  auto* doubletst = new SetDoublePointerShell();
  my_registry->add_test(doubletst);
  my_registry->run_all_tests(*result);

  CHECK(orig_double_ptr == &orig_double);
  CHECK_EQUAL(size_t{ 1 }, result->get_check_count());
  delete doubletst;
}
