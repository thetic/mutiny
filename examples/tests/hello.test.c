#include "hello.h"

#include "CppUTest/TestHarness.h"

static const char* output_ptr;
static int (*saved_print)(const char*, ...);

static int
capture_output(const char* format, ...)
{
  output_ptr = format;
  return 1;
}

TEST_GROUP_C_SETUP(hello)
{
  saved_print = PrintFormated;
  PrintFormated = capture_output;
  output_ptr = NULL;
}

TEST_GROUP_C_TEARDOWN(hello)
{
  PrintFormated = saved_print;
}

TEST_C(hello, PrintsHelloWorld)
{
  printHelloWorld();
  CHECK_EQUAL_C_STRING("Hello World!\n", output_ptr);
}
