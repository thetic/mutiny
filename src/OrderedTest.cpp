#include "CppMu/OrderedTest.hpp"

#include "CppMu/TestRegistry.hpp"

namespace cppmu {

OrderedTestShell* OrderedTestShell::ordered_tests_head_ = nullptr;

bool OrderedTestShell::is_ordered() const
{
  return true;
}

int OrderedTestShell::get_level()
{
  return level_;
}

void OrderedTestShell::set_level(int level)
{
  level_ = level;
}

void OrderedTestShell::set_ordered_test_head(OrderedTestShell* test)
{
  ordered_tests_head_ = test;
}

OrderedTestShell* OrderedTestShell::get_ordered_test_head()
{
  return ordered_tests_head_;
}

bool OrderedTestShell::first_ordered_test()
{
  return (get_ordered_test_head() == nullptr);
}

OrderedTestShell* OrderedTestShell::add_ordered_test(OrderedTestShell* test)
{
  cppmu::TestShell::add_test(test);
  next_ordered_test_ = test;
  return this;
}

void OrderedTestShell::add_ordered_test_to_head(OrderedTestShell* test)
{
  TestRegistry* reg = TestRegistry::get_current_registry();
  cppmu::TestShell* head = get_ordered_test_head();

  if (nullptr == reg->get_first_test() || head == reg->get_first_test()) {
    reg->add_test(test);
  } else {
    reg->get_test_with_next(head)->add_test(test);
    test->add_test(head);
  }

  test->next_ordered_test_ = get_ordered_test_head();
  set_ordered_test_head(test);
}

OrderedTestShell* OrderedTestShell::get_next_ordered_test()
{
  return next_ordered_test_;
}

OrderedTestInstaller::OrderedTestInstaller(
    OrderedTestShell& test,
    const char* group_name,
    const char* test_name,
    const char* file_name,
    size_t line_number,
    int level
)
{
  test.set_test_name(test_name);
  test.set_group_name(group_name);
  test.set_file_name(file_name);
  test.set_line_number(line_number);
  test.set_level(level);

  if (OrderedTestShell::first_ordered_test())
    OrderedTestShell::add_ordered_test_to_head(&test);
  else
    add_ordered_test_in_order(&test);
}

void OrderedTestInstaller::add_ordered_test_in_order(OrderedTestShell* test)
{
  if (test->get_level() <
      OrderedTestShell::get_ordered_test_head()->get_level())
    OrderedTestShell::add_ordered_test_to_head(test);
  else
    add_ordered_test_in_order_not_at_head_position(test);
}

void OrderedTestInstaller::add_ordered_test_in_order_not_at_head_position(
    OrderedTestShell* test
)
{
  OrderedTestShell* current = OrderedTestShell::get_ordered_test_head();
  while (current->get_next_ordered_test()) {

    if (current->get_next_ordered_test()->get_level() > test->get_level()) {
      test->add_ordered_test(current->get_next_ordered_test());
      current->add_ordered_test(test);
      return;
    }
    current = current->get_next_ordered_test();
  }
  test->add_ordered_test(current->get_next_ordered_test());
  current->add_ordered_test(test);
}

} // namespace cppmu
