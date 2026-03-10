
#include "TestHarness_cTestCFile.h"

#include "CppUTest/PlatformSpecificFunctions.h"
#include "CppUTest/TestHarness.h"

TEST_GROUP_C_SETUP(TestGroupInC)
{
  setup_teardown_was_called_in_test_group_in_C++;
}

TEST_GROUP_C_TEARDOWN(TestGroupInC)
{
  setup_teardown_was_called_in_test_group_in_C--;
  CHECK_C(test_was_called_in_test_group_in_C == 1);
  test_was_called_in_test_group_in_C--;
}

TEST_C(TestGroupInC, checkThatTheTestHasRun)
{
  test_was_called_in_test_group_in_C++;
}

IGNORE_TEST_C(TestGroupInC, ignoreMacroForCFile)
{
  test_was_called_in_test_group_in_C++;
}
