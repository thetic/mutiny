#include "hello.h"

#include "mu/tiny/test.h"

static const char* output_ptr;
static int (*saved_print)(const char*, ...);

static int capture_output(const char* format, ...)
{
  output_ptr = format;
  return 1;
}

TEST_GROUP_SETUP(hello)
{
  saved_print = print_formated;
  print_formated = capture_output;
  output_ptr = NULL;
}

TEST_GROUP_TEARDOWN(hello)
{
  print_formated = saved_print;
}

TEST(hello, PrintsHelloWorld)
{
  print_hello_world();
  CHECK_EQUAL_STRING("Hello World!\n", output_ptr);
}
