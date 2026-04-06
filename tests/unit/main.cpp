#include "mutiny/test/CommandLineRunner.hpp"
#include "mutiny/test/Shell.hpp"

int main(int argc, char** argv)
{
  /* These checks are here to make sure assertions outside test runs don't
   * crash */
  CHECK(true);
  CHECK_EQUAL(1, 1);

  return mu::tiny::test::CommandLineRunner::run_all_tests(argc, argv);
}
