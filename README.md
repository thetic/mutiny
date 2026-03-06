# CppUTest

[![GitHub Actions](https://github.com/cpputest/cpputest/actions/workflows/basic.yml/badge.svg)](https://github.com/cpputest/cpputest/actions/workflows/basic.yml)
[![Coverage Status](https://coveralls.io/repos/cpputest/cpputest/badge.svg?branch=master&service=github)](https://coveralls.io/github/cpputest/cpputest?branch=master)
[![ConanCenter package](https://repology.org/badge/version-for-repo/conancenter/cpputest.svg)](https://conan.io/center/cpputest)

CppUTest unit testing and mocking framework for C/C++

[More information on the project page](https://cpputest.github.io)

Slack channel:
[Join if link not expired](https://join.slack.com/t/cpputest/shared_invite/zt-epq97u9h-6yBQHHl2cvUADjEENtdASw)

## Getting Started

You'll need to do the following to get started:

Building from source:

```bash
git clone https://github.com/cpputest/cpputest.git
cd cpputest
cmake -B build
cmake --build build
```

You can use `cmake --install build` if you want to install CppUTest system-wide.

Then to get started, you'll need to do the following:

* Add the include path to the Makefile. Something like:
    * `CPPFLAGS += -I$(CPPUTEST_HOME)/include`
* Add the library linking to your Makefile. Something like:
    * `LD_LIBRARIES = -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt`

After this, you can write your first test:

```cpp
TEST_GROUP(FirstTestGroup)
{
};

TEST(FirstTestGroup, FirstTest)
{
   FAIL("Fail me!");
}
```

You can build and install cpputest using [vcpkg](https://github.com/Microsoft/vcpkg/) dependency manager:

```console
$ vcpkg install cpputest (More information: https://github.com/microsoft/vcpkg)
```

## Command line switches

* `-h` help, shows the latest help, including the parameters we've implemented after updating this README page.
* `-v` verbose, print each test name as it runs
* `-r#` repeat the tests some number of times, default is one, default if # is not specified is 2.
* `-s#` random shuffle the test execution order. # is an integer used for seeding the random number generator. # is optional, and if omitted, the seed value is chosen automatically, which results in a different order every time. The seed value is printed to console to make it possible to reproduce a previously generated execution order. Handy for detecting problems related to dependencies between tests.
* `-ri` run ignored tests as if they are not ignored.
* `-g` group only run test whose group contains the substring group
* `-n` name only run test whose name contains the substring name
* `-f` crash on fail, run the tests as normal but, when a test fails, crash rather than report the failure in the normal way

## Test Macros

* `TEST(group, name)` - define a test
* `IGNORE_TEST(group, name)` - turn off the execution of a test
* `TEST_GROUP(group)` - Declare a test group to which certain tests belong. This will also create the link needed from another library.
* `TEST_GROUP_BASE(group, base)` - Same as `TEST_GROUP`, just use a different base class than Utest
* `TEST_SETUP()` - Declare a void setup method in a `TEST_GROUP` - this is the same as declaring void `setup()`
* `TEST_TEARDOWN()` - Declare a void setup method in a `TEST_GROUP`
* `IMPORT_TEST_GROUP(group)` - Export the name of a test group so it can be linked in from a library. Needs to be done in `main`.

## Set up and tear down support

* Each `TEST_GROUP` may contain a `setup` and/or a `teardown` method.
* `setup()` is called prior to each `TEST` body and `teardown()` is called after the test body.

## Assertion Macros

The failure of one of these macros causes the current test to immediately exit

* `CHECK(boolean condition)` - checks any boolean result
* `CHECK_TRUE(boolean condition)` - checks for true
* `CHECK_FALSE(boolean condition)` - checks for false
* `CHECK_EQUAL(expected, actual)` - checks for equality between entities using `==`. So if you have a class that supports `operator==()` you can use this macro to compare two instances.
* `STRCMP_EQUAL(expected, actual)` - check const `char*` strings for equality using `strcmp`
* `LONGS_EQUAL(expected, actual)` - Compares two numbers
* `BYTES_EQUAL(expected, actual)` - Compares two numbers, eight bits wide
* `POINTERS_EQUAL(expected, actual)` - Compares two `const void *`
* `DOUBLES_EQUAL(expected, actual, tolerance)` - Compares two doubles within some tolerance
* `ENUMS_EQUAL_INT(excepted, actual)` - Compares two enums which their underlying type is `int`
* `ENUMS_EQUAL_TYPE(underlying_type, excepted, actual)` - Compares two enums which they have the same underlying type
* `FAIL(text)` - always fails
* `TEST_EXIT` - Exit the test without failure - useful for contract testing (implementing an assert fake)

Customize `CHECK_EQUAL` to work with your types that support `operator==()`

- Create the function: `SimpleString StringFrom(const yourType&)`

The Extensions directory has a few of these.

## Building default checks with TestPlugin

* CppUTest can support extra checking functionality by inserting TestPlugins
* TestPlugin is derived from the TestPlugin class and can be inserted in the TestRegistry via the installPlugin method.
* TestPlugins can be used for, for example, system stability and resource handling like files, memory or network connection clean-up.

Example of a main with a TestPlugin:

```cpp
int main(int ac, char** av)
{
   LogPlugin logPlugin;
   TestRegistry::getCurrentRegistry()->installPlugin(&logPlugin);
   int result = CommandLineTestRunner::RunAllTests(ac, av);
   TestRegistry::getCurrentRegistry()->resetPlugins();
   return result;
}
```

## Example Main

```cpp
#include "CppUTest/CommandLineTestRunner.h"

int main(int ac, char** av)
{
  return RUN_ALL_TESTS(ac, av);
}
```

## Example Test

```cpp
#include "CppUTest/TestHarness.h"
#include "ClassName.h"

TEST_GROUP(ClassName)
{
  ClassName* className;

  void setup()
  {
    className = new ClassName();
  }
  void teardown()
  {
    delete className;
  }
};

TEST(ClassName, Create)
{
  CHECK(0 != className);
  CHECK(true);
  CHECK_EQUAL(1,1);
  LONGS_EQUAL(1,1);
  DOUBLES_EQUAL(1.000, 1.001, .01);
  STRCMP_EQUAL("hello", "hello");
  FAIL("The prior tests pass, but this one doesn't");
}
```

## Conan

CppUTest is available through [conan-center][conan-center].

##### conanfile.txt

```ini
[requires]
cpputest/4.0

[generators]
cmake_find_package
cmake_paths
```

##### CMake

```cmake
find_package(CppUTest REQUIRED)

add_executable(example_test ExampleTest.cpp)

target_link_libraries(example_test PRIVATE
    CppUTest::CppUTest
    CppUTest::CppUTestExt)
```

## CMake Integration

### FetchContent

CMake can automatically download and integrate CppUTest with
[`FetchContent`](https://cmake.org/cmake/help/latest/module/FetchContent.html).
This is the recommended strategy for CMake integration.

```cmake
cmake_minimum_required(VERSION 3.11)
project(trying_CppUtest)

include(FetchContent)
FetchContent_Declare(
    CppUTest
    GIT_REPOSITORY https://github.com/cpputest/cpputest.git
    GIT_TAG        master # or use release tag, eg. v4.0
)
FetchContent_MakeAvailable(CppUTest)

add_executable(trying_CppUtest main.cpp)
target_link_libraries(trying_CppUtest PRIVATE CppUTest::CppUTest)

include(CTest)
include(CppUTest)
cpputest_discover_tests(trying_CppUtest)
```

### System install

If CppUTest is installed on the system, use
[`find_package`](https://cmake.org/cmake/help/latest/command/find_package.html):

```cmake
cmake_minimum_required(VERSION 3.10)
project(trying_CppUtest)

find_package(CppUTest REQUIRED)

add_executable(trying_CppUtest main.cpp)
target_link_libraries(trying_CppUtest PRIVATE CppUTest::CppUTest)

include(CTest)
include(CppUTest)
cpputest_discover_tests(trying_CppUtest)
```

### Subdirectory

To integrate CppUTest from a local directory (e.g. when building for a
different target platform):

```cmake
cmake_minimum_required(VERSION 3.10)
project(trying_CppUtest)

set(CPPUTEST_PLATFORM OFF CACHE BOOL "Platform implementation")
add_subdirectory(/path/to/cpputest CppUTest)
target_sources(CppUTest PRIVATE ${PROJECT_SOURCE_DIR}/UtestPlatform.cpp)

add_executable(trying_CppUtest main.cpp)
target_link_libraries(trying_CppUtest PRIVATE CppUTest::CppUTest)

include(CTest)
include(CppUTest)
cpputest_discover_tests(trying_CppUtest)
```

[conan-center]: https://conan.io/center/cpputest

## Testing C Code

CppUTest can be used to test C code. A few things to keep in mind:

### Using `extern "C"`

Wrap C headers and declarations in `extern "C"` in your `.cpp` test files:

```cpp
extern "C" {
    #include "hello.h"

    extern HelloWorldApi theRealHelloWorldApi;
}
```

Use `TestHarness_c.h` (not `TestHarness.h`) when writing tests in C — it
provides C-compatible versions of the `CHECK` macros.

### C++ keywords in C code

If a C file uses a C++ keyword (e.g. `bool`) as an identifier, `#define` it
away inside the `extern "C"` block:

```cpp
extern "C" {
#define bool helloBool
#include "hello.h"
#undef bool
}
```
