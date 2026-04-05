# Mu::tiny

[![Basic builds](https://github.com/thetic/mutiny/actions/workflows/basic.yml/badge.svg)](https://github.com/thetic/mutiny/actions/workflows/basic.yml)
[![Coverage Status](https://coveralls.io/repos/github/thetic/mutiny/badge.svg)](https://coveralls.io/github/thetic/mutiny)

*Mu::tiny* is a C/C++ unit testing and mocking framework suited for embedded and
low-resource targets.

## Building Mu::tiny

*Mu::tiny* uses CMake. Clone the repository and configure:

```sh
cmake -B build
cmake --build build
cmake --install build  # optional system install
```

## Adding Mu::tiny to Your Project

The recommended approach uses CMake
[FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html)
with `FIND_PACKAGE_ARGS`. This first attempts
[`find_package`](https://cmake.org/cmake/help/latest/command/find_package.html);
if _Mu::tiny_ is not installed, CMake fetches and builds it from source
automatically:

```cmake
cmake_minimum_required(VERSION 3.24)
project(my_tests)

include(FetchContent)
FetchContent_Declare(
    mutiny
    GIT_REPOSITORY    https://github.com/thetic/mutiny.git
    GIT_TAG           v0.1.0
    FIND_PACKAGE_ARGS 0.1
)
FetchContent_MakeAvailable(mutiny)

add_executable(my_tests main.cpp widget.test.cpp)
target_link_libraries(my_tests PRIVATE mu::tiny)

include(CTest)
include(mutiny)
mutiny_discover_tests(my_tests)
```

If _Mu::tiny_ is already installed and you prefer not to use
[FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html), use
[`find_package`](https://cmake.org/cmake/help/latest/command/find_package.html) directly:

```cmake
find_package(mutiny 0.1 REQUIRED)
```

## Headers

All public headers live under `include/mutiny/`.
The main headers you'll use:

| Header                              | Purpose                                                                         |
| ----------------------------------- | ------------------------------------------------------------------------------- |
| `mutiny/test.hpp`                   | Test and assertion macros (`TEST_GROUP`, `TEST`, `CHECK`, etc.)                 |
| `mutiny/mock.hpp`                   | Mock framework (`mu::tiny::mock::mock`, `mu::tiny::mock::Support`)              |
| `mutiny/test.h`                     | C interface                                                                     |
| `mutiny/test/CommandLineRunner.hpp` | `mu::tiny::test::CommandLineRunner` (`main()` runner)                           |
| `mutiny/test/Ordered.hpp`           | `TEST_ORDERED` macro                                                            |

## Writing `main`

Every test executable needs a `main()`.
The simplest form uses `mu::tiny::test::CommandLineRunner`:

```cpp
#include "mutiny/test/CommandLineRunner.hpp"

int main(int argc, char** argv)
{
    return mu::tiny::test::CommandLineRunner::run_all_tests(argc, argv);
}
```

To add plugins (e.g. `mu::tiny::test::JUnitOutputPlugin`,
`mu::tiny::test::SetPointerPlugin`,
`mu::tiny::mock::SupportPlugin`), install them via
`mu::tiny::test::Registry`:

```cpp
#include "mutiny/test/CommandLineRunner.hpp"
#include "mutiny/test/JUnitOutputPlugin.hpp"
#include "mutiny/mock/SupportPlugin.hpp"
#include "mutiny/test/Registry.hpp"

int main(int argc, char** argv)
{
    mu::tiny::test::JUnitOutputPlugin junit;
    mu::tiny::mock::SupportPlugin mock_plugin;
    auto* reg = mu::tiny::test::Registry::get_current_registry();
    reg->install_plugin(&junit);
    reg->install_plugin(&mock_plugin);
    return mu::tiny::test::CommandLineRunner::run_all_tests(argc, argv);
}
```

## Your First Test

See [examples/tests/CheatSheet.test.cpp](examples/tests/CheatSheet.test.cpp):

- `TEST_GROUP` declares a group;
  the `struct` implicitly inherits from `mu::tiny::test::Test`.
  `setup()` runs before each test body;
  `teardown()` runs after.
- `TEST(group, name)` defines a single test.
- A failing assertion immediately exits the test body.

## Running Tests

Run the binary directly:

```sh
./build/GNU/tests/my_tests                  # run all
./build/GNU/tests/my_tests -v               # verbose: print each test name
./build/GNU/tests/my_tests -g CheatSheet    # only group "CheatSheet"
./build/GNU/tests/my_tests -n TestName      # only tests whose name contains this
```

Via CTest (after `mutiny_discover_tests` in CMakeLists):

```sh
ctest --preset GNU
ctest --preset GNU -R CheatSheet
```

See [docs/command-line-reference.rst](docs/command-line-reference.rst) for all flags.

## Examples

| File                                                                                     | Demonstrates                                                                         |
| ---------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------ |
| [CheatSheet.test.cpp](examples/tests/CheatSheet.test.cpp)                               | Minimal `TEST_GROUP` + `TEST` with `setup()`, `MUTINY_PTR_SET`, and assertion macros |
| [CircularBuffer.test.cpp](examples/tests/CircularBuffer.test.cpp)                       | Full group with setup/teardown, helper methods, and multiple assertion styles        |

## Attribution

*Mu::tiny* is descended from [CppUTest](https://github.com/cpputest/cpputest).
Users looking for more robust support of older language standards, or legacy compilers and platforms should consider using *CppUTest* instead.
