
#include "CppUTest/CppUTest.h"

#include "CppUTest.test.h"

TEST_GROUP_C_SETUP(CppUTestC)
{
  setup_teardown_was_called_in_test_group_in_c++;
}

TEST_GROUP_C_TEARDOWN(CppUTestC)
{
  setup_teardown_was_called_in_test_group_in_c--;
  CHECK_C(test_was_called_in_test_group_in_c == 1);
  test_was_called_in_test_group_in_c--;
}

TEST_C(CppUTestC, checkThatTheTestHasRun)
{
  test_was_called_in_test_group_in_c++;
}

IGNORE_TEST_C(CppUTestC, ignoreMacroForCFile)
{
  test_was_called_in_test_group_in_c++;
}
