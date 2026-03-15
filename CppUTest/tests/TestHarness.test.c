
#include "CppUTest/TestHarness.h"

#include "TestHarness.test.h"

TEST_GROUP_C_SETUP(TestHarnessC)
{
  setup_teardown_was_called_in_test_group_in_C++;
}

TEST_GROUP_C_TEARDOWN(TestHarnessC)
{
  setup_teardown_was_called_in_test_group_in_C--;
  CHECK_C(test_was_called_in_test_group_in_C == 1);
  test_was_called_in_test_group_in_C--;
}

TEST_C(TestHarnessC, checkThatTheTestHasRun)
{
  test_was_called_in_test_group_in_C++;
}

IGNORE_TEST_C(TestHarnessC, ignoreMacroForCFile)
{
  test_was_called_in_test_group_in_C++;
}
