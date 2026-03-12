#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness.hpp"

TEST_GROUP_C_WRAPPER(Hello){ TEST_GROUP_C_SETUP_WRAPPER(Hello)
      TEST_GROUP_C_TEARDOWN_WRAPPER(Hello) };

TEST_C_WRAPPER(Hello, PrintsHelloWorld)
