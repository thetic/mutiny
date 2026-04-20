#include "mu/tiny/test/SkippedShell.hpp"

#include "mu/tiny/test.hpp"

TEST_GROUP(SkippedShell){};

TEST(SkippedShell, getFormattedName_whenRunSkipped_showsTEST)
{
  mu::tiny::test::SkippedShell shell;
  shell.set_group_name("TestGroup");
  shell.set_test_name("TestName");
  shell.set_run_skipped();
  STRCMP_EQUAL("TEST(TestGroup, TestName)", shell.get_formatted_name().c_str());
}
