#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestOutput.hpp"

#include <math.h>

#if defined(NAN) && defined(INFINITY)

namespace {
const int failLineNumber = 2;
const char* failFileName = "fail.cpp";
}

TEST_GROUP(TestFailureNaN)
{
  cpputest::TestShell* test;

  void setup() override
  {
    test = new cpputest::TestShell(
        "groupname", "testname", failFileName, failLineNumber - 1);
  }
  void teardown() override { delete test; }
};
#define FAILURE_EQUAL(a, b)                                                    \
  STRCMP_EQUAL_LOCATION(a, (b).getMessage().c_str(), "", __FILE__, __LINE__)

TEST(TestFailureNaN, DoublesEqualExpectedIsNaN)
{
  cpputest::DoublesEqualFailure f(test,
      failFileName,
      failLineNumber,
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
  cpputest::DoublesEqualFailure f(test,
      failFileName,
      failLineNumber,
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
  cpputest::DoublesEqualFailure f(test,
      failFileName,
      failLineNumber,
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
  cpputest::DoublesEqualFailure f(test,
      failFileName,
      failLineNumber,
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
  cpputest::DoublesEqualFailure f(test,
      failFileName,
      failLineNumber,
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
  cpputest::DoublesEqualFailure f(test,
      failFileName,
      failLineNumber,
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
