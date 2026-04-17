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
  CHECK_APPROX(
      mu::tiny::mock::NamedValue::default_double_tolerance,
      value->get_double_tolerance(),
      0.0
  );
}

TEST(NamedValue, GivenToleranceUsed)
{
  value->set_value(0.2, 3.2);
  STRCMP_EQUAL("double", value->get_type().c_str());
  CHECK_APPROX(0.2, value->get_value<double>(), 0.0);
  CHECK_APPROX(3.2, value->get_double_tolerance(), 0.0);
}

TEST(NamedValue, DoublesEqualIfWithinTolerance)
{
  value->set_value(5.0, 0.4);
  mu::tiny::mock::NamedValue other("param2");
  other.set_value(5.3);

  CHECK(value->equals(other));
}

TEST(NamedValue, DoublesNotEqualIfOutsideTolerance)
{
  value->set_value(5.0, 0.4);
  mu::tiny::mock::NamedValue other("param2");
  other.set_value(5.5);

  CHECK(!value->equals(other));
}
