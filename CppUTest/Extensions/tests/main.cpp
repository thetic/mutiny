#include "CppUTest/CommandLineTestRunner.hpp"

int
main(int argc, const char* const* argv)
{
  return cpputest::CommandLineTestRunner::run_all_tests(argc, argv);
}
