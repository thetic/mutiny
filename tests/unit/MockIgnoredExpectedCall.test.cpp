#include "mutiny/mock/IgnoredExpectedCall.hpp"

#include "mutiny/test.hpp"

TEST_GROUP(IgnoredExpectedCall)
{
  mu::tiny::mock::IgnoredExpectedCall ignored;
};

TEST(IgnoredExpectedCall, worksAsItShould)
{
  ignored.with_name("func");
  ignored.with_call_order(1);
  ignored.with_call_order(1, 1);
  ignored.on_object(nullptr);
  ignored.with_parameter("umm", true);
  ignored.with_parameter("bla", static_cast<int>(1));
  ignored.with_parameter("foo", static_cast<unsigned int>(1));
  ignored.with_parameter("hey", static_cast<long int>(1));
  ignored.with_parameter("bah", static_cast<unsigned long int>(1));
  ignored.with_parameter("yo", static_cast<long long int>(1));
  ignored.with_parameter("grr", static_cast<unsigned long long int>(1));
  ignored.with_parameter("hah", static_cast<double>(1.1f));
  ignored.with_parameter("gah", 2.1, 0.3);
  ignored.with_parameter("goo", "hello");
  ignored.with_parameter("pie", static_cast<void*>(nullptr));
  ignored.with_parameter("woo", static_cast<const void*>(nullptr));
  ignored.with_parameter("fop", static_cast<void (*)()>(nullptr));
  ignored.with_parameter("waa", static_cast<const unsigned char*>(nullptr), 0);
  ignored.with_parameter_of_type(
      "mytype", "top", static_cast<const void*>(nullptr)
  );
  ignored.with_output_parameter_returning(
      "bar", static_cast<void*>(nullptr), 1
  );
  ignored.with_output_parameter_of_type_returning(
      "mytype", "bar", static_cast<const void*>(nullptr)
  );
  ignored.with_unmodified_output_parameter("unmod");
  ignored.ignore_other_parameters();
  ignored.and_return_value(true);
  ignored.and_return_value(1.0);
  ignored.and_return_value(1U);
  ignored.and_return_value(1);
  ignored.and_return_value(1UL);
  ignored.and_return_value(1L);
  ignored.and_return_value(1ULL);
  ignored.and_return_value(1LL);
  ignored.and_return_value("boo");
  ignored.and_return_value(static_cast<void*>(nullptr));
  ignored.and_return_value(static_cast<const void*>(nullptr));
  ignored.and_return_value(static_cast<void (*)()>(nullptr));
}
