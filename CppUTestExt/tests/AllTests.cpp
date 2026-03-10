#include "CppUTestExt/MockSupportPlugin.hpp"

#include "CppUTest/CommandLineTestRunner.hpp"
#include "CppUTest/TestRegistry.hpp"

using namespace cpputest;
using namespace cpputest::extensions;

int
main(int ac, const char* const* av)
{
  int result = 0;

  {
    MockSupportPlugin mockPlugin;
    TestRegistry::getCurrentRegistry()->installPlugin(&mockPlugin);

    result = CommandLineTestRunner::RunAllTests(ac, av);
  }

  return result;
}
