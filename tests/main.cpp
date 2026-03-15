#include "CppMu/CommandLineTestRunner.hpp"

int
main(int argc, char** argv)
{
  int return_value = 0;

  {
    /* These checks are here to make sure assertions outside test runs don't
     * crash */
    CHECK(true);
    LONGS_EQUAL(1, 1);

    return_value = cppmu::CommandLineTestRunner::run_all_tests(
        argc, argv); /* cover alternate method */
  }

  return return_value;
}
