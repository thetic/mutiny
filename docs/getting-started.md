# Getting Started with CppMu

## Building CppMu

CppMu uses CMake. Clone the repository and configure:

```bash
cmake -B build
cmake --build build
cmake --install build  # optional system install
```

## Adding CppMu to Your Project

The recommended approach is CMake `FetchContent`:

```cmake
cmake_minimum_required(VERSION 3.11)
project(my_tests)

include(FetchContent)
FetchContent_Declare(
    CppMu
    GIT_REPOSITORY https://github.com/thetic/cppmu.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(CppMu)

add_executable(my_tests main.cpp widget.test.cpp)
target_link_libraries(my_tests PRIVATE CppMu::CppMu)

include(CTest)
include(CppMu)
cppmu_discover_tests(my_tests)
```

If CppMu is already installed system-wide, use `find_package(CppMu REQUIRED)` instead of `FetchContent`.

### Headers

All public headers live under `include/CppMu/`. The main headers you'll use:

| Header | Purpose |
|--------|---------|
| `CppMu/CppMu.hpp` | Test definition macros (`TEST_GROUP`, `TEST`, etc.) |
| `CppMu/TestShell.hpp` | Assertion macros (`CHECK`, `CHECK_EQUAL`, etc.) |
| `CppMu/MockSupport.hpp` | Mock framework (`mock()`, `MockSupport`) |
| `CppMu/CppMu.h` | C interface (include in `.test.c` files) |
| `CppMu/CommandLineTestRunner.hpp` | `main()` runner |
| `CppMu/OrderedTest.hpp` | `TEST_ORDERED` macro |

## Writing `main()`

Every test executable needs a `main()`. The simplest form:

```cpp
#include "CppMu/CommandLineTestRunner.hpp"

int main(int argc, char** argv)
{
    return cppmu::CommandLineTestRunner::run_all_tests(argc, argv);
}
```

To add plugins (e.g. JUnit output, SetPointer, MockSupportPlugin):

```cpp
#include "CppMu/CommandLineTestRunner.hpp"
#include "CppMu/JUnitOutputPlugin.hpp"
#include "CppMu/MockSupportPlugin.hpp"
#include "CppMu/TestRegistry.hpp"

int main(int argc, char** argv)
{
    cppmu::JUnitOutputPlugin junit;
    cppmu::MockSupportPlugin mock_plugin;
    auto* reg = cppmu::TestRegistry::get_current_registry();
    reg->install_plugin(&junit);
    reg->install_plugin(&mock_plugin);
    return cppmu::CommandLineTestRunner::run_all_tests(argc, argv);
}
```

## Your First Test

```cpp
// widget.test.cpp
#include "CppMu/CppMu.hpp"
#include "widget.h"

TEST_GROUP(Widget)
{
    Widget* w;

    void setup() override
    {
        w = new Widget(42);
    }

    void teardown() override
    {
        delete w;
    }
};

TEST(Widget, ReturnsInitialValue)
{
    CHECK_EQUAL(42, w->value());
}

TEST(Widget, CanBeReset)
{
    w->reset();
    CHECK_EQUAL(0, w->value());
}
```

- `TEST_GROUP` declares a group and its shared state. `setup()` runs before each test body; `teardown()` runs after.
- `TEST(group, name)` defines a single test. The body is the `{ ... }` block that follows.
- A failing assertion immediately exits the test body (via `longjmp` or exception).

## Running Tests

Run the binary directly:

```bash
./build/GNU/tests/my_tests         # run all
./build/GNU/tests/my_tests -v      # verbose: print each test name
./build/GNU/tests/my_tests -g Widget          # only group "Widget"
./build/GNU/tests/my_tests -n ReturnsInitial  # only tests whose name contains this
```

Via CTest (after `cppmu_discover_tests` in CMakeLists):

```bash
ctest --preset GNU
ctest --preset GNU -R Widget
```

See [command-line-reference.md](command-line-reference.md) for all flags.

## Examples

| File | Demonstrates |
|------|-------------|
| [`examples/tests/CheatSheet.test.cpp`](../examples/tests/CheatSheet.test.cpp) | Minimal `TEST_GROUP` + `TEST` with `setup()`, `CPPMU_PTR_SET`, and a handful of assertion macros |
| [`examples/tests/CircularBuffer.test.cpp`](../examples/tests/CircularBuffer.test.cpp) | Full group with setup/teardown, helper methods, and multiple assertion styles |
