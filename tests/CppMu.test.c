
#include "CppMu/CppMu.h"

#include "CppMu.test.h"

TEST_GROUP_C_SETUP(CppMuC)
{
  setup_teardown_was_called_in_test_group_in_c++;
}

TEST_GROUP_C_TEARDOWN(CppMuC)
{
  setup_teardown_was_called_in_test_group_in_c--;
  CHECK_C(test_was_called_in_test_group_in_c == 1);
  test_was_called_in_test_group_in_c--;
}

TEST_C(CppMuC, checkThatTheTestHasRun)
{
  test_was_called_in_test_group_in_c++;
}

IGNORE_TEST_C(CppMuC, ignoreMacroForCFile)
{
  test_was_called_in_test_group_in_c++;
}
