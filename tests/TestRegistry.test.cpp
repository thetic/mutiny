#include "CppMu/TestRegistry.hpp"

#include "CppMu/CppMu.hpp"
#include "CppMu/StringBufferTestOutput.hpp"
#include "CppMu/TestOutput.hpp"
#include "CppMu/time.hpp"

namespace {
const int test_line_number = 1;

int
get_zero()
{
  return 0;
}

class MockTest : public cppmu::TestShell
{
public:
  MockTest(const char* group = "Group")
    : TestShell(group, "Name", "File", test_line_number)
    , has_run(false)
  {
  }
  void run_one_test(cppmu::TestPlugin*, cppmu::TestResult&) override
  {
    has_run = true;
  }

  bool has_run;
};

class MockTestResult : public cppmu::TestResult
{
public:
  int count_tests_started;
  int count_tests_ended;
  int count_current_test_started;
  int count_current_test_ended;
  int count_current_group_started;
  int count_current_group_ended;

  MockTestResult(cppmu::TestOutput& p)
    : TestResult(p)
  {
    reset_count();
  }

  ~MockTestResult() override {}

  void reset_count()
  {
    count_tests_started = 0;
    count_tests_ended = 0;
    count_current_test_started = 0;
    count_current_test_ended = 0;
    count_current_group_started = 0;
    count_current_group_ended = 0;
  }

  void tests_started() override { count_tests_started++; }
  void tests_ended() override { count_tests_ended++; }
  void current_test_started(cppmu::TestShell* /*test*/) override
  {
    count_current_test_started++;
  }
  void current_test_ended(cppmu::TestShell* /*test*/) override
  {
    count_current_test_ended++;
  }
  void current_group_started(cppmu::TestShell* /*test*/) override
  {
    count_current_group_started++;
  }
  void current_group_ended(cppmu::TestShell* /*test*/) override
  {
    count_current_group_ended++;
  }
};

class MyTestPluginDummy : public cppmu::TestPlugin
{
public:
  MyTestPluginDummy(const cppmu::String& name)
    : TestPlugin(name)
  {
  }
  ~MyTestPluginDummy() override {}
  void run_all_pre_test_action(cppmu::TestShell&, cppmu::TestResult&) override
  {
  }
  void run_all_post_test_action(cppmu::TestShell&, cppmu::TestResult&) override
  {
  }
};

} // namespace

TEST_GROUP(TestRegistry)
{
  cppmu::TestRegistry* my_registry;
  cppmu::StringBufferTestOutput* output;
  MockTest* test1;
  MockTest* test2;
  MockTest* test3;
  MockTest* test4;
  cppmu::TestResult* result;
  MockTestResult* mock_result;
  void setup() override
  {
    output = new cppmu::StringBufferTestOutput();
    mock_result = new MockTestResult(*output);
    result = mock_result;
    test1 = new MockTest();
    test2 = new MockTest();
    test3 = new MockTest("group2");
    test4 = new MockTest();
    my_registry = new cppmu::TestRegistry();
    my_registry->set_current_registry(my_registry);
  }

  void teardown() override
  {
    my_registry->set_current_registry(nullptr);
    delete my_registry;
    delete test1;
    delete test2;
    delete test3;
    delete test4;
    delete result;
    delete output;
  }

  void add_and_run_all_tests()
  {
    my_registry->add_test(test1);
    my_registry->add_test(test2);
    my_registry->add_test(test3);
    my_registry->run_all_tests(*result);
  }
};

TEST(TestRegistry, registryMyRegistryAndReset)
{
  CHECK(my_registry->get_current_registry() == my_registry);
}

TEST(TestRegistry, emptyRegistryIsEmpty)
{
  CHECK(my_registry->count_tests() == 0);
}

TEST(TestRegistry, addOneTestIsNotEmpty)
{
  my_registry->add_test(test1);
  CHECK(my_registry->count_tests() == 1);
}

TEST(TestRegistry, addOneTwoTests)
{
  my_registry->add_test(test1);
  my_registry->add_test(test2);
  CHECK(my_registry->count_tests() == 2);
}

TEST(TestRegistry, runTwoTests)
{
  my_registry->add_test(test1);
  my_registry->add_test(test2);
  CHECK(!test1->has_run);
  CHECK(!test2->has_run);
  my_registry->run_all_tests(*result);
  CHECK(test1->has_run);
  CHECK(test2->has_run);
}

TEST(TestRegistry, runTwoTestsCheckResultFunctionsCalled)
{
  my_registry->add_test(test1);
  my_registry->add_test(test2);
  my_registry->run_all_tests(*result);
  LONGS_EQUAL(1, mock_result->count_tests_started);
  LONGS_EQUAL(1, mock_result->count_tests_ended);
  LONGS_EQUAL(1, mock_result->count_current_group_started);
  LONGS_EQUAL(1, mock_result->count_current_group_ended);
  LONGS_EQUAL(2, mock_result->count_current_test_started);
  LONGS_EQUAL(2, mock_result->count_current_test_ended);
}

TEST(TestRegistry, runThreeTestsandTwoGroupsCheckResultFunctionsCalled)
{
  add_and_run_all_tests();
  LONGS_EQUAL(2, mock_result->count_current_group_started);
  LONGS_EQUAL(2, mock_result->count_current_group_ended);
  LONGS_EQUAL(3, mock_result->count_current_test_started);
  LONGS_EQUAL(3, mock_result->count_current_test_ended);
}

TEST(TestRegistry, unDoTest)
{
  my_registry->add_test(test1);
  CHECK(my_registry->count_tests() == 1);
  my_registry->un_do_last_add_test();
  CHECK(my_registry->count_tests() == 0);
}

TEST(TestRegistry, unDoButNoTest)
{
  CHECK(my_registry->count_tests() == 0);
  my_registry->un_do_last_add_test();
  CHECK(my_registry->count_tests() == 0);
}

TEST(TestRegistry, reallyUndoLastTest)
{
  my_registry->add_test(test1);
  my_registry->add_test(test2);
  CHECK(my_registry->count_tests() == 2);
  my_registry->un_do_last_add_test();
  CHECK(my_registry->count_tests() == 1);
  my_registry->run_all_tests(*result);
  CHECK(test1->has_run);
  CHECK(!test2->has_run);
}

TEST(TestRegistry, findTestWithNameDoesntExist)
{
  CHECK(my_registry->find_test_with_name("ThisTestDoesntExists") == nullptr);
}

TEST(TestRegistry, findTestWithName)
{
  test1->set_test_name("NameOfATestThatDoesExist");
  test2->set_test_name("SomeOtherTest");
  my_registry->add_test(test1);
  my_registry->add_test(test2);
  CHECK(
      my_registry->find_test_with_name("NameOfATestThatDoesExist") != nullptr);
}

TEST(TestRegistry, findTestWithGroupDoesntExist)
{
  CHECK(my_registry->find_test_with_group("ThisTestGroupDoesntExists") ==
        nullptr);
}

TEST(TestRegistry, findTestWithGroup)
{
  test1->set_group_name("GroupOfATestThatDoesExist");
  test2->set_group_name("SomeOtherGroup");
  my_registry->add_test(test1);
  my_registry->add_test(test2);
  CHECK(my_registry->find_test_with_group("GroupOfATestThatDoesExist") !=
        nullptr);
}

TEST(TestRegistry, nameFilterWorks)
{
  test1->set_test_name("testname");
  test2->set_test_name("noname");
  cppmu::TestFilter name_filter("testname");
  my_registry->set_name_filters(&name_filter);
  add_and_run_all_tests();
  CHECK(test1->has_run);
  CHECK(!test2->has_run);
}

TEST(TestRegistry, groupFilterWorks)
{
  test1->set_group_name("groupname");
  test2->set_group_name("noname");
  cppmu::TestFilter group_filter("groupname");
  my_registry->set_group_filters(&group_filter);
  add_and_run_all_tests();
  CHECK(test1->has_run);
  CHECK(!test2->has_run);
}

TEST(TestRegistry, CurrentRepetitionIsCorrectNone)
{
  CHECK(0 == my_registry->get_current_repetition());
  my_registry->run_all_tests(*result);
  LONGS_EQUAL(1, my_registry->get_current_repetition());
}

TEST(TestRegistry, CurrentRepetitionIsCorrectTwo)
{
  CHECK(0 == my_registry->get_current_repetition());
  my_registry->run_all_tests(*result);
  my_registry->run_all_tests(*result);
  LONGS_EQUAL(2, my_registry->get_current_repetition());
}

TEST(TestRegistry, ResetPluginsWorks)
{
  MyTestPluginDummy plugin1("Plugin-1");
  MyTestPluginDummy plugin2("Plugin-2");
  my_registry->install_plugin(&plugin1);
  my_registry->install_plugin(&plugin2);
  LONGS_EQUAL(2, my_registry->count_plugins());
  my_registry->reset_plugins();
  LONGS_EQUAL(0, my_registry->count_plugins());
}

TEST(TestRegistry,
    listTestGroupNames_shouldListBackwardsGroup1AfterGroup11AndGroup2OnlyOnce)
{
  test1->set_group_name("GROUP_1");
  my_registry->add_test(test1);
  test2->set_group_name("GROUP_2");
  my_registry->add_test(test2);
  test3->set_group_name("GROUP_11");
  my_registry->add_test(test3);
  test4->set_group_name("GROUP_2");
  my_registry->add_test(test4);

  my_registry->list_test_group_names(*result);
  cppmu::String s = output->get_output();
  STRCMP_EQUAL("GROUP_2 GROUP_11 GROUP_1", s.c_str());
}

TEST(TestRegistry,
    listTestGroupAndCaseNames_shouldListBackwardsGroupATestaAfterGroupAtestaa)
{
  test1->set_group_name("GROUP_A");
  test1->set_test_name("test_a");
  my_registry->add_test(test1);
  test2->set_group_name("GROUP_B");
  test2->set_test_name("test_b");
  my_registry->add_test(test2);
  test3->set_group_name("GROUP_A");
  test3->set_test_name("test_aa");
  my_registry->add_test(test3);

  my_registry->list_test_group_and_case_names(*result);
  cppmu::String s = output->get_output();
  STRCMP_EQUAL("GROUP_A.test_aa GROUP_B.test_b GROUP_A.test_a", s.c_str());
}

TEST(TestRegistry,
    listTestLocations_shouldListBackwardsGroupATestaAfterGroupAtestaa)
{
  test1->set_group_name("GROUP_A");
  test1->set_test_name("test_a");
  test1->set_file_name("cpptest_simple/my_tests/testa.cpp");
  test1->set_line_number(100);
  my_registry->add_test(test1);
  test2->set_group_name("GROUP_B");
  test2->set_test_name("test_b");
  test2->set_file_name("cpptest_simple/my tests/testb.cpp");
  test2->set_line_number(200);
  my_registry->add_test(test2);
  test3->set_group_name("GROUP_A");
  test3->set_test_name("test_aa");
  test3->set_file_name("cpptest_simple/my_tests/testaa.cpp");
  test3->set_line_number(300);
  my_registry->add_test(test3);

  my_registry->list_test_locations(*result);
  cppmu::String s = output->get_output();
  STRCMP_EQUAL("GROUP_A.test_aa.cpptest_simple/my_tests/"
               "testaa.cpp.300\nGROUP_B.test_b.cpptest_simple/my "
               "tests/testb.cpp.200\nGROUP_A.test_a.cpptest_simple/my_tests/"
               "testa.cpp.100\n",
      s.c_str());
}

TEST(TestRegistry, shuffleEmptyListIsNoOp)
{
  CHECK_TRUE(my_registry->get_first_test() == nullptr);
  my_registry->shuffle_tests(0);
  CHECK_TRUE(my_registry->get_first_test() == nullptr);
}

TEST(TestRegistry, shuffleSingleTestIsNoOp)
{
  my_registry->add_test(test1);
  my_registry->shuffle_tests(0);
  CHECK_TRUE(my_registry->get_first_test() == test1);
}

IGNORE_TEST(TestRegistry, shuffleTestList)
{
  CPPMU_PTR_SET(cppmu::rand, get_zero);
  my_registry->add_test(test3);
  my_registry->add_test(test2);
  my_registry->add_test(test1);

  cppmu::TestShell* first_before = my_registry->get_first_test();
  cppmu::TestShell* second_before = first_before->get_next();
  cppmu::TestShell* third_before = second_before->get_next();

  CHECK_TRUE(first_before == test1);
  CHECK_TRUE(second_before == test2);
  CHECK_TRUE(third_before == test3);
  CHECK_TRUE(third_before->get_next() == nullptr);

  // shuffle always with element at index 0: [1] 2 [3] --> [3] [2] 1 --> 2 3 1
  my_registry->shuffle_tests(0);

  cppmu::TestShell* first_after = my_registry->get_first_test();
  cppmu::TestShell* second_after = first_after->get_next();
  cppmu::TestShell* third_after = second_after->get_next();

  CHECK_TRUE(first_after == test2);
  CHECK_TRUE(second_after == test3);
  CHECK_TRUE(third_after == test1);
  CHECK_TRUE(third_after->get_next() == nullptr);
}

TEST(TestRegistry, reverseTests)
{
  my_registry->add_test(test1);
  my_registry->add_test(test2);

  my_registry->reverse_tests();

  CHECK(test1 == my_registry->get_first_test());
}

TEST(TestRegistry, reverseZeroTests)
{
  my_registry->reverse_tests();

  CHECK(nullptr == my_registry->get_first_test());
}
