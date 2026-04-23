#include "mu/tiny/test/Ordered.hpp"

#include "mu/tiny/test/Registry.hpp"

namespace mu {
namespace tiny {
namespace test {

namespace {
void add_ordered_test_in_order_not_at_head_position(OrderedShell* test);

void add_ordered_test_in_order(OrderedShell* test)
{
  if (test->get_level() < OrderedShell::get_ordered_test_head()->get_level()) {
    OrderedShell::add_ordered_test_to_head(test);
  } else {
    add_ordered_test_in_order_not_at_head_position(test);
  }
}

void add_ordered_test_in_order_not_at_head_position(OrderedShell* test)
{
  OrderedShell* current = OrderedShell::get_ordered_test_head();
  while (current->get_next_ordered_test() != nullptr) {

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
} // namespace

OrderedShell* OrderedShell::ordered_tests_head_ = nullptr;

bool OrderedShell::is_ordered() const
{
  return true;
}

int OrderedShell::get_level() const
{
  return level_;
}

void OrderedShell::set_level(int level)
{
  level_ = level;
}

void OrderedShell::set_ordered_test_head(OrderedShell* test)
{
  ordered_tests_head_ = test;
}

OrderedShell* OrderedShell::get_ordered_test_head()
{
  return ordered_tests_head_;
}

bool OrderedShell::first_ordered_test()
{
  return (get_ordered_test_head() == nullptr);
}

OrderedShell* OrderedShell::add_ordered_test(OrderedShell* test)
{
  Shell::add_test(test);
  next_ordered_test_ = test;
  return this;
}

void OrderedShell::add_ordered_test_to_head(OrderedShell* test)
{
  Registry* reg = Registry::get_current_registry();
  Shell* head = get_ordered_test_head();

  if (nullptr == reg->get_first_test() || head == reg->get_first_test()) {
    reg->add_test(test);
  } else {
    reg->get_test_with_next(head)->add_test(test);
    test->add_test(head);
  }

  test->next_ordered_test_ = get_ordered_test_head();
  set_ordered_test_head(test);
}

OrderedShell* OrderedShell::get_next_ordered_test()
{
  return next_ordered_test_;
}

OrderedInstaller::OrderedInstaller(
    OrderedShell& test,
    const char* group_name,
    const char* test_name,
    const char* file_name,
    int line_number,
    int level
) noexcept
{
  test.set_test_name(test_name);
  test.set_group_name(group_name);
  test.set_file_name(file_name);
  test.set_line_number(line_number);
  test.set_level(level);

  if (OrderedShell::first_ordered_test()) {
    OrderedShell::add_ordered_test_to_head(&test);
  } else {
    add_ordered_test_in_order(&test);
  }
}

} // namespace test
} // namespace tiny
} // namespace mu
