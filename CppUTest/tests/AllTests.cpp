#include "CppUTest/CommandLineTestRunner.hpp"

int
main(int ac, char** av)
{
  int returnValue = 0;

  {
    /* These checks are here to make sure assertions outside test runs don't
     * crash */
    CHECK(true);
    LONGS_EQUAL(1, 1);

    returnValue = cpputest::CommandLineTestRunner::RunAllTests(
        ac, av); /* cover alternate method */
  }

  return returnValue;
}
