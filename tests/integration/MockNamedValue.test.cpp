#include "mutiny/mock/NamedValue.hpp"

#include "mutiny/test.hpp"

TEST_GROUP(NamedValue)
{
  mu::tiny::mock::NamedValue* value;
  void setup() override { value = new mu::tiny::mock::NamedValue("param"); }

  void teardown() override { delete value; }
};

TEST(NamedValue, DefaultToleranceUsedWhenNoToleranceGiven)
{
  value->set_value(0.2);
  DOUBLES_EQUAL(
      mu::tiny::mock::NamedValue::default_double_tolerance,
      value->get_double_tolerance(),
      0.0
  );
}

TEST(NamedValue, GivenToleranceUsed)
{
  value->set_value(0.2, 3.2);
  STRCMP_EQUAL("double", value->get_type().c_str());
  DOUBLES_EQUAL(0.2, value->get_double_value(), 0.0);
  DOUBLES_EQUAL(3.2, value->get_double_tolerance(), 0.0);
}

TEST(NamedValue, DoublesEqualIfWithinTolerance)
{
  value->set_value(5.0, 0.4);
  mu::tiny::mock::NamedValue other("param2");
  other.set_value(5.3);

  CHECK_TRUE(value->equals(other));
}

TEST(NamedValue, DoublesNotEqualIfOutsideTolerance)
{
  value->set_value(5.0, 0.4);
  mu::tiny::mock::NamedValue other("param2");
  other.set_value(5.5);

  CHECK_FALSE(value->equals(other));
}
