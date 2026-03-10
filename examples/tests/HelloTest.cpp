#include "hello.h"

#include "CppUTest/TestHarness.hpp"

#include <stdarg.h>
#include <stdio.h>

static cpputest::String* buffer;

TEST_GROUP(HelloWorld)
{
  static int output_method(const char* output, ...)
  {
    va_list arguments;
    va_start(arguments, output);
    *buffer = cpputest::VStringFromFormat(output, arguments);
    va_end(arguments);
    return 1;
  }

  void setup() override
  {
    buffer = new cpputest::String();
    UT_PTR_SET(PrintFormated, &output_method);
  }

  void teardown() override { delete buffer; }
};

TEST(HelloWorld, PrintOk)
{
  printHelloWorld();
  STRCMP_EQUAL("Hello World!\n", buffer->c_str());
}
