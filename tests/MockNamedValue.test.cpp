#include "mutiny/mock/NamedValue.hpp"
#include "mutiny/test.hpp"

TEST_GROUP(MockNamedValue)
{
  mu::tiny::mock::MockNamedValue* value;
  void setup() override { value = new mu::tiny::mock::MockNamedValue("param"); }

  void teardown() override { delete value; }
};

TEST(MockNamedValue, DefaultToleranceUsedWhenNoToleranceGiven)
{
  value->set_value(0.2);
  DOUBLES_EQUAL(
      mu::tiny::mock::MockNamedValue::default_double_tolerance,
      value->get_double_tolerance(),
      0.0
  );
}

TEST(MockNamedValue, GivenToleranceUsed)
{
  value->set_value(0.2, 3.2);
  STRCMP_EQUAL("double", value->get_type().c_str());
  DOUBLES_EQUAL(0.2, value->get_double_value(), 0.0);
  DOUBLES_EQUAL(3.2, value->get_double_tolerance(), 0.0);
}

TEST(MockNamedValue, DoublesEqualIfWithinTolerance)
{
  value->set_value(5.0, 0.4);
  mu::tiny::mock::MockNamedValue other("param2");
  other.set_value(5.3);

  CHECK_TRUE(value->equals(other));
}

TEST(MockNamedValue, DoublesNotEqualIfOutsideTolerance)
{
  value->set_value(5.0, 0.4);
  mu::tiny::mock::MockNamedValue other("param2");
  other.set_value(5.5);

  CHECK_FALSE(value->equals(other));
}
