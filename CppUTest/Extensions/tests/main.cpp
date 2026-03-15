#include "CppUTest/Extensions/MockSupportPlugin.hpp"

#include "CppUTest/CommandLineTestRunner.hpp"
#include "CppUTest/TestRegistry.hpp"

int
main(int argc, const char* const* argv)
{
  int result = 0;

  {
    cpputest::extensions::MockSupportPlugin mock_plugin;
    cpputest::TestRegistry::get_current_registry()->install_plugin(
        &mock_plugin);

    result = cpputest::CommandLineTestRunner::run_all_tests(argc, argv);
  }

  return result;
}
