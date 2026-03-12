#include "CppUTest/Extensions/MockSupportPlugin.hpp"

#include "CppUTest/CommandLineTestRunner.hpp"
#include "CppUTest/TestRegistry.hpp"

int
main(int ac, const char* const* av)
{
  int result = 0;

  {
    cpputest::extensions::MockSupportPlugin mockPlugin;
    cpputest::TestRegistry::getCurrentRegistry()->installPlugin(&mockPlugin);

    result = cpputest::CommandLineTestRunner::RunAllTests(ac, av);
  }

  return result;
}
