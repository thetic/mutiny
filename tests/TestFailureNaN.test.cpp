#include "CppMu/CppMu.hpp"
#include "CppMu/TestOutput.hpp"

#include <math.h>

#if defined(NAN) && defined(INFINITY)

namespace {
const int fail_line_number = 2;
const char* fail_file_name = "fail.cpp";
}

TEST_GROUP(TestFailureNaN)
{
  cppmu::TestShell* test;

  void setup() override
  {
    test = new cppmu::TestShell(
        "groupname", "testname", fail_file_name, fail_line_number - 1);
  }
  void teardown() override { delete test; }
};
#define FAILURE_EQUAL(a, b)                                                    \
  STRCMP_EQUAL_LOCATION(a, (b).get_message().c_str(), "", __FILE__, __LINE__)

TEST(TestFailureNaN, DoublesEqualExpectedIsNaN)
{
  cppmu::DoublesEqualFailure f(test,
      fail_file_name,
      fail_line_number,
      static_cast<double>(NAN),
      2.0,
      3.0,
      "");
  FAILURE_EQUAL("expected <Nan - Not a number>\n"
                "\tbut was  <2> threshold used was <3>\n"
                "\tCannot make comparisons with Nan",
      f);
}

TEST(TestFailureNaN, DoublesEqualActualIsNaN)
{
  cppmu::DoublesEqualFailure f(test,
      fail_file_name,
      fail_line_number,
      1.0,
      static_cast<double>(NAN),
      3.0,
      "");
  FAILURE_EQUAL("expected <1>\n"
                "\tbut was  <Nan - Not a number> threshold used was <3>\n"
                "\tCannot make comparisons with Nan",
      f);
}

TEST(TestFailureNaN, DoublesEqualThresholdIsNaN)
{
  cppmu::DoublesEqualFailure f(test,
      fail_file_name,
      fail_line_number,
      1.0,
      2.0,
      static_cast<double>(NAN),
      "");
  FAILURE_EQUAL("expected <1>\n"
                "\tbut was  <2> threshold used was <Nan - Not a number>\n"
                "\tCannot make comparisons with Nan",
      f);
}

TEST(TestFailureNaN, DoublesEqualExpectedIsInf)
{
  cppmu::DoublesEqualFailure f(test,
      fail_file_name,
      fail_line_number,
      static_cast<double>(INFINITY),
      2.0,
      3.0,
      "");
  FAILURE_EQUAL("expected <Inf - Infinity>\n"
                "\tbut was  <2> threshold used was <3>",
      f);
}

TEST(TestFailureNaN, DoublesEqualActualIsInf)
{
  cppmu::DoublesEqualFailure f(test,
      fail_file_name,
      fail_line_number,
      1.0,
      static_cast<double>(INFINITY),
      3.0,
      "");
  FAILURE_EQUAL("expected <1>\n"
                "\tbut was  <Inf - Infinity> threshold used was <3>",
      f);
}

TEST(TestFailureNaN, DoublesEqualThresholdIsInf)
{
  cppmu::DoublesEqualFailure f(test,
      fail_file_name,
      fail_line_number,
      1.0,
      static_cast<double>(NAN),
      static_cast<double>(INFINITY),
      "");
  FAILURE_EQUAL(
      "expected <1>\n"
      "\tbut was  <Nan - Not a number> threshold used was <Inf - Infinity>\n"
      "\tCannot make comparisons with Nan",
      f);
}

#endif
