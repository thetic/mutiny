#include "CppMu/CppMu.hpp"
#include "CppMu/TestFilter.hpp"
#include "CppMu/TestShell.hpp"

TEST_BASE(MyOwnTest)
{
  MyOwnTest() = default;
  bool in_test{ false };

  void setup() override
  {
    CHECK(!in_test);
    in_test = true;
  }
  void teardown() override
  {
    CHECK(in_test);
    in_test = false;
  }
};

TEST_GROUP_BASE(TestShellCustomBase, MyOwnTest){};

TEST(TestShellCustomBase, test)
{
  CHECK(in_test);
}

class NullParameterTest : public cppmu::TestShell
{};

TEST(TestShellCustomBase, NullParameters)
{
  NullParameterTest null_test; /* Bug fix tests for creating a test without a
                                 name, fix in String */
  cppmu::TestFilter empty_filter;
  CHECK(null_test.should_run(&empty_filter, &empty_filter));
}
