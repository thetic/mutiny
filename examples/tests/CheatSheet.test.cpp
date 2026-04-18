#include "mu/tiny/test.hpp"

namespace {
void (*real_one)();
void stub() {}
} // namespace

/* Declare TestGroup with name CheatSheet */
TEST_GROUP(CheatSheet)
{
  /* declare a setup method for the test group. Optional. */
  void setup() override
  {
    /* Set method real_one to stub. Automatically restore in teardown */
    MUTINY_PTR_SET(real_one, stub);
  }

  /* Declare a teardown method for the test group. Optional */
  void teardown() override {}
}; /* Do not forget semicolumn */

/* Declare one test within the test group */
TEST(CheatSheet, TestName)
{
  /* Check two longs are equal */
  CHECK_EQUAL(1L, 1L);

  /* Check a condition */
  CHECK(true == true);

  /* Check a string */
  STRCMP_EQUAL("HelloWorld", "HelloWorld");
}
