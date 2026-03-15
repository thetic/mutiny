# CppMu

CppMu is a C/C++ unit testing and mocking framework suited for embedded and
low-resource targets.

## Attribution

CppMu is descended from [CppUTest](https://github.com/cpputest/cpputest),
a unit testing framework originally created by Michael Feathers and maintained
by the CppUTest community. The test API, assertion macros, mock framework, and
core architecture are all derived from that project.

---

## Getting Started

Building from source:

```bash
cmake -B build
cmake --build build
```

You can use `cmake --install build` to install CppMu system-wide.

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

## Command line switches

- `-h` help, shows the latest help, including the parameters we've implemented after updating this README page.
- `-v` verbose, print each test name as it runs
- `-r#` repeat the tests some number of times, default is one, default if # is not specified is 2.
- `-s#` random shuffle the test execution order. # is an integer used for seeding the random number generator. # is optional, and if omitted, the seed value is chosen automatically, which results in a different order every time. The seed value is printed to console to make it possible to reproduce a previously generated execution order. Handy for detecting problems related to dependencies between tests.
- `-ri` run ignored tests as if they are not ignored.
- `-g` group only run test whose group contains the substring group
- `-n` name only run test whose name contains the substring name
- `-f` crash on fail, run the tests as normal but, when a test fails, crash rather than report the failure in the normal way

## Test Macros

- `TEST(group, name)` - define a test
- `IGNORE_TEST(group, name)` - turn off the execution of a test
- `TEST_GROUP(group)` - Declare a test group to which certain tests belong. This will also create the link needed from another library.
- `TEST_GROUP_BASE(group, base)` - Same as `TEST_GROUP`, just use a different base class than `Test`
- `TEST_SETUP()` - Declare a void setup method in a `TEST_GROUP` - this is the same as declaring void `setup()`
- `TEST_TEARDOWN()` - Declare a void setup method in a `TEST_GROUP`
- `IMPORT_TEST_GROUP(group)` - Export the name of a test group so it can be linked in from a library. Needs to be done in `main`.

## Set up and tear down support

- Each `TEST_GROUP` may contain a `setup` and/or a `teardown` method.
- `setup()` is called prior to each `TEST` body and `teardown()` is called after the test body.

## Assertion Macros

The failure of one of these macros causes the current test to immediately exit

- `CHECK(boolean condition)` - checks any boolean result
- `CHECK_TRUE(boolean condition)` - checks for true
- `CHECK_FALSE(boolean condition)` - checks for false
- `CHECK_EQUAL(expected, actual)` - checks for equality between entities using `==`. So if you have a class that supports `operator==()` you can use this macro to compare two instances.
- `STRCMP_EQUAL(expected, actual)` - check const `char*` strings for equality using `strcmp`
- `LONGS_EQUAL(expected, actual)` - Compares two numbers
- `BYTES_EQUAL(expected, actual)` - Compares two numbers, eight bits wide
- `POINTERS_EQUAL(expected, actual)` - Compares two `const void *`
- `DOUBLES_EQUAL(expected, actual, tolerance)` - Compares two doubles within some tolerance
- `ENUMS_EQUAL_INT(excepted, actual)` - Compares two enums which their underlying type is `int`
- `ENUMS_EQUAL_TYPE(underlying_type, excepted, actual)` - Compares two enums which they have the same underlying type
- `FAIL(text)` - always fails
- `TEST_EXIT` - Exit the test without failure - useful for contract testing (implementing an assert fake)

Customize `CHECK_EQUAL` to work with your types that support `operator==()`

- Create the function: `String StringFrom(const yourType&)`

## Building default checks with TestPlugin

- CppMu can support extra checking functionality by inserting TestPlugins
- TestPlugin is derived from the TestPlugin class and can be inserted in the TestRegistry via the installPlugin method.
- TestPlugins can be used for, for example, system stability and resource handling like files, memory or network connection clean-up.

Example of a main with a TestPlugin:

```cpp
int main(int argc, char** argv)
{
   LogPlugin logPlugin;
   TestRegistry::getCurrentRegistry()->installPlugin(&logPlugin);
   int result = CommandLineTestRunner::RunAllTests(argc, argv);
   TestRegistry::getCurrentRegistry()->resetPlugins();
   return result;
}
```

## Example Main

```cpp
#include "CppMu/CommandLineTestRunner.h"

int main(int argc, char** argv)
{
  return cppmu::CommandLineTestRunner::RunAllTests(argc, argv);
}
```

## Example Test

```cpp
#include "CppMu/CppMu.h"
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

## CMake Integration

### FetchContent

CMake can automatically download and integrate CppMu with
[`FetchContent`](https://cmake.org/cmake/help/latest/module/FetchContent.html).
This is the recommended strategy for CMake integration.

```cmake
cmake_minimum_required(VERSION 3.11)
project(trying_CppMu)

include(FetchContent)
FetchContent_Declare(
    CppMu
    GIT_REPOSITORY https://github.com/your-org/cppmu.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(CppMu)

add_executable(trying_CppMu main.cpp)
target_link_libraries(trying_CppMu PRIVATE CppMu::CppMu)

include(CTest)
include(CppMu)
cppmu_discover_tests(trying_CppMu)
```

### System install

If CppMu is installed on the system, use
[`find_package`](https://cmake.org/cmake/help/latest/command/find_package.html):

```cmake
cmake_minimum_required(VERSION 3.10)
project(trying_CppMu)

find_package(CppMu REQUIRED)

add_executable(trying_CppMu main.cpp)
target_link_libraries(trying_CppMu PRIVATE CppMu::CppMu)

include(CTest)
include(CppMu)
cppmu_discover_tests(trying_CppMu)
```

### Subdirectory

To integrate CppMu from a local directory (e.g. when building for a
different target platform):

```cmake
cmake_minimum_required(VERSION 3.10)
project(trying_CppMu)

set(CPPMU_PLATFORM OFF CACHE BOOL "Platform implementation")
add_subdirectory(/path/to/cppmu CppMu)
target_sources(CppMu PRIVATE ${PROJECT_SOURCE_DIR}/UtestPlatform.cpp)

add_executable(trying_CppMu main.cpp)
target_link_libraries(trying_CppMu PRIVATE CppMu::CppMu)

include(CTest)
include(CppMu)
cppmu_discover_tests(trying_CppMu)
```

## Testing C Code

CppMu supports writing tests in pure C using a two-file pattern: a `.test.c`
file for the C tests and a `.test.cpp` wrapper that wires them into the C++ runner.

### The `.test.c` file

Include `"CppMu/CppMu.h"` and use the C-specific macros:

```c
#include "CppMu/CppMu.h"
#include "mymodule.h"

TEST_GROUP_C_SETUP(MyGroup)
{
  /* runs before each test */
}

TEST_GROUP_C_TEARDOWN(MyGroup)
{
  /* runs after each test */
}

TEST_C(MyGroup, SomeTest)
{
  CHECK_EQUAL_C_INT(42, mymodule_compute());
}
```

Available typed assertion macros: `CHECK_C()`, `CHECK_EQUAL_C_INT()`,
`CHECK_EQUAL_C_UINT()`, `CHECK_EQUAL_C_LONG()`, `CHECK_EQUAL_C_STRING()`,
`CHECK_EQUAL_C_REAL()`, `CHECK_EQUAL_C_POINTER()`, `CHECK_EQUAL_C_MEMCMP()`,
and more (see `CppMu/CppMu.h`).

### The companion `.test.cpp` wrapper

Create a `.test.cpp` file that bridges the C functions into the C++ test runner:

```cpp
#include "CppMu/CppMu.h"

TEST_GROUP_C_WRAPPER(MyGroup)
{
  TEST_GROUP_C_SETUP_WRAPPER(MyGroup)
  TEST_GROUP_C_TEARDOWN_WRAPPER(MyGroup)
};

TEST_C_WRAPPER(MyGroup, SomeTest)
```

Both files must be compiled together into the same test executable. CMake
handles this automatically — just list both in `add_executable()`.

See `examples/` for a complete working example (`hello.test.c` /
`hello.test.cpp`).
