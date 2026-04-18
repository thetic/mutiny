#include "mu/tiny/test/CommandLineRunner.hpp"

int main(int argc, char** argv)
{
  return mu::tiny::test::CommandLineRunner::run_all_tests(argc, argv);
}
