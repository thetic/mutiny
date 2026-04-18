#include "OrderedTest.h"

#include "mu/tiny/test/Ordered.hpp"
#include "mu/tiny/test/Registry.hpp"

#include "mu/tiny/test.hpp"

namespace {
class OrderedTestTestingFixture
{
public:
  static void check_run(int run)
  {
    if (run != run_) {
      run_ = run;
      count_ = 0;
    }
  }
  static int count(void) { return count_++; }

private:
  static int run_;
  static int count_;
};

int OrderedTestTestingFixture::run_ = 0;
int OrderedTestTestingFixture::count_ = 0;
} // namespace

int mutiny_ordered_test_fixture_c_wrapper(void)
{
  return OrderedTestTestingFixture::count();
}

TEST_GROUP(OrderedTestC)
{
  void setup() override
  {
    OrderedTestTestingFixture::check_run(
        mu::tiny::test::Registry::get_current_registry()
            ->get_current_repetition()
    );
  }
};

TEST(OrderedTestC, NormalTest)
{
  CHECK(OrderedTestTestingFixture::count() == 0);
}

TEST_ORDERED(OrderedTestC, Test1, 1)
{
  CHECK(OrderedTestTestingFixture::count() == 1);
}

TEST_ORDERED(OrderedTestC, Test2, 2)
{
  CHECK(OrderedTestTestingFixture::count() == 2);
}

TEST_ORDERED(OrderedTestC, Test3, 3)
{
  CHECK(OrderedTestTestingFixture::count() == 3);
}

TEST_ORDERED(OrderedTestC, Test4, 4)
{
  CHECK(OrderedTestTestingFixture::count() == 4);
}

// Test with same level
TEST_ORDERED(OrderedTestC, Test5_1, 5)
{
  CHECK(OrderedTestTestingFixture::count() == 5);
}

TEST_ORDERED(OrderedTestC, Test5_2, 5)
{
  CHECK(OrderedTestTestingFixture::count() == 6);
}

TEST_ORDERED(OrderedTestC, Test6_1, 6)
{
  CHECK(OrderedTestTestingFixture::count() == 7);
}

TEST_ORDERED_C_WRAPPER(OrderedTestC, Test7, 7)

TEST_ORDERED(OrderedTestC, Test6_2, 6)
{
  CHECK(OrderedTestTestingFixture::count() == 8);
}

TEST_ORDERED(OrderedTestC, Test8, 8)
{
  CHECK(OrderedTestTestingFixture::count() == 10);
}

TEST_ORDERED_C_WRAPPER(OrderedTestC, Test9, 9)

TEST_ORDERED(OrderedTestC, Test10, 10)
{
  CHECK(OrderedTestTestingFixture::count() == 12);
}

TEST_ORDERED_C_WRAPPER(OrderedTestC, Test11, 11)
