#include "mu/tiny/test/CommandLineRunner.hpp"

using mu::tiny::test::CommandLineRunner;

int main(int argc, char** argv)
{
  return CommandLineRunner::run_all_tests(argc, argv);
}
