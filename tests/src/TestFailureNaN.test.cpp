#include "mu/tiny/test/Output.hpp"

#include "mu/tiny/test.hpp"

#include <math.h>

#if defined(NAN) && defined(INFINITY)

namespace {
const int fail_line_number = 2;
const char* fail_file_name = "fail.cpp";
} // namespace

TEST_GROUP(TestFailureNaN)
{
  mu::tiny::test::Shell* test;

  void setup() override
  {
    test = new mu::tiny::test::Shell(
        "groupname", "testname", fail_file_name, fail_line_number - 1
    );
  }
  void teardown() override { delete test; }
};
#define FAILURE_EQUAL(a, b)                                                    \
  STRCMP_EQUAL_LOCATION(a, (b).get_message().c_str(), "", __FILE__, __LINE__)

TEST(TestFailureNaN, DoublesEqualExpectedIsNaN)
{
  mu::tiny::test::ApproxEqualFailure<double> f(
      test,
      fail_file_name,
      fail_line_number,
      static_cast<double>(NAN),
      2.0,
      3.0,
      ""
  );
  FAILURE_EQUAL(
      "expected <Nan - Not a number>\n"
      "\tbut was  <2> threshold used was <3>\n"
      "\tCannot make comparisons with Nan",
      f
  );
}

TEST(TestFailureNaN, DoublesEqualActualIsNaN)
{
  mu::tiny::test::ApproxEqualFailure<double> f(
      test,
      fail_file_name,
      fail_line_number,
      1.0,
      static_cast<double>(NAN),
      3.0,
      ""
  );
  FAILURE_EQUAL(
      "expected <1>\n"
      "\tbut was  <Nan - Not a number> threshold used was <3>\n"
      "\tCannot make comparisons with Nan",
      f
  );
}

TEST(TestFailureNaN, DoublesEqualThresholdIsNaN)
{
  mu::tiny::test::ApproxEqualFailure<double> f(
      test,
      fail_file_name,
      fail_line_number,
      1.0,
      2.0,
      static_cast<double>(NAN),
      ""
  );
  FAILURE_EQUAL(
      "expected <1>\n"
      "\tbut was  <2> threshold used was <Nan - Not a number>\n"
      "\tCannot make comparisons with Nan",
      f
  );
}

TEST(TestFailureNaN, DoublesEqualExpectedIsInf)
{
  mu::tiny::test::ApproxEqualFailure<double> f(
      test,
      fail_file_name,
      fail_line_number,
      static_cast<double>(INFINITY),
      2.0,
      3.0,
      ""
  );
  FAILURE_EQUAL(
      "expected <Inf - Infinity>\n"
      "\tbut was  <2> threshold used was <3>",
      f
  );
}

TEST(TestFailureNaN, DoublesEqualActualIsInf)
{
  mu::tiny::test::ApproxEqualFailure<double> f(
      test,
      fail_file_name,
      fail_line_number,
      1.0,
      static_cast<double>(INFINITY),
      3.0,
      ""
  );
  FAILURE_EQUAL(
      "expected <1>\n"
      "\tbut was  <Inf - Infinity> threshold used was <3>",
      f
  );
}

TEST(TestFailureNaN, DoublesEqualThresholdIsInf)
{
  mu::tiny::test::ApproxEqualFailure<double> f(
      test,
      fail_file_name,
      fail_line_number,
      1.0,
      static_cast<double>(NAN),
      static_cast<double>(INFINITY),
      ""
  );
  FAILURE_EQUAL(
      "expected <1>\n"
      "\tbut was  <Nan - Not a number> threshold used was <Inf - Infinity>\n"
      "\tCannot make comparisons with Nan",
      f
  );
}

TEST(TestFailureNaN, FloatsEqualExpectedIsNaN)
{
  mu::tiny::test::ApproxEqualFailure<float> f(
      test,
      fail_file_name,
      fail_line_number,
      static_cast<float>(NAN),
      2.0f,
      3.0f,
      ""
  );
  FAILURE_EQUAL(
      "expected <Nan - Not a number>\n"
      "\tbut was  <2> threshold used was <3>\n"
      "\tCannot make comparisons with Nan",
      f
  );
}

TEST(TestFailureNaN, FloatsEqualActualIsNaN)
{
  mu::tiny::test::ApproxEqualFailure<float> f(
      test,
      fail_file_name,
      fail_line_number,
      1.0f,
      static_cast<float>(NAN),
      3.0f,
      ""
  );
  FAILURE_EQUAL(
      "expected <1>\n"
      "\tbut was  <Nan - Not a number> threshold used was <3>\n"
      "\tCannot make comparisons with Nan",
      f
  );
}

TEST(TestFailureNaN, FloatsEqualThresholdIsNaN)
{
  mu::tiny::test::ApproxEqualFailure<float> f(
      test,
      fail_file_name,
      fail_line_number,
      1.0f,
      2.0f,
      static_cast<float>(NAN),
      ""
  );
  FAILURE_EQUAL(
      "expected <1>\n"
      "\tbut was  <2> threshold used was <Nan - Not a number>\n"
      "\tCannot make comparisons with Nan",
      f
  );
}

#endif
