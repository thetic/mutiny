#include "CppUTest/Extensions/MockSupportPlugin.hpp"

#include "CppUTest/CommandLineTestRunner.hpp"
#include "CppUTest/TestRegistry.hpp"

int
main(int argc, const char* const* argv)
{
  int result = 0;

  {
    cpputest::extensions::MockSupportPlugin mockPlugin;
    cpputest::TestRegistry::getCurrentRegistry()->installPlugin(&mockPlugin);

    result = cpputest::CommandLineTestRunner::RunAllTests(argc, argv);
  }

  return result;
}
