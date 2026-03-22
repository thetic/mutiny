# Testing C Code

CppMu supports writing tests in pure C. Because the test runner itself is C++, you need two files per test group: a `.test.c` file containing the tests, and a `.test.cpp` wrapper that registers them with the C++ runner.

## The Two-File Pattern

### `.test.c` — the C test file

Include `"CppMu/CppMu.h"` and use the C macros:

```c
/* widget.test.c */
#include "widget.h"
#include "CppMu/CppMu.h"

TEST_GROUP_SETUP(Widget)
{
    widget_init();
}

TEST_GROUP_TEARDOWN(Widget)
{
    widget_reset();
}

TEST(Widget, ReturnsInitialValue)
{
    CHECK_EQUAL_INT(0, widget_get_value());
}

TEST(Widget, CanBeSet)
{
    widget_set_value(42);
    CHECK_EQUAL_INT(42, widget_get_value());
}
```

`TEST_GROUP_SETUP` and `TEST_GROUP_TEARDOWN` in C define wrapper functions that the `.test.cpp` file calls. `TEST` in C defines a wrapper function body.

### `.test.cpp` — the C++ registration wrapper

```cpp
/* widget.test.cpp */
#include "CppMu/CppMu.hpp"

TEST_GROUP_C_WRAPPER(Widget)
{
    TEST_GROUP_C_SETUP_WRAPPER(Widget)
    TEST_GROUP_C_TEARDOWN_WRAPPER(Widget)
};

TEST_C_WRAPPER(Widget, ReturnsInitialValue)
TEST_C_WRAPPER(Widget, CanBeSet)
```

Both files must be compiled together into the same test executable:

```cmake
add_executable(my_tests main.cpp widget.test.c widget.test.cpp)
target_link_libraries(my_tests PRIVATE CppMu::CppMu)
```

---

## C Assertion Macros (`CppMu/CppMu.h`)

The C header provides typed macros because C does not have overloaded functions:

| Macro | Type checked |
|-------|-------------|
| `CHECK(condition)` | any non-zero value |
| `CHECK_TEXT(condition, text)` | same with message |
| `CHECK_EQUAL_BOOL(expected, actual)` | `bool` |
| `CHECK_EQUAL_INT(expected, actual)` | `int` |
| `CHECK_EQUAL_UINT(expected, actual)` | `unsigned int` |
| `CHECK_EQUAL_LONG(expected, actual)` | `long` |
| `CHECK_EQUAL_ULONG(expected, actual)` | `unsigned long` |
| `CHECK_EQUAL_LONGLONG(expected, actual)` | `long long` |
| `CHECK_EQUAL_ULONGLONG(expected, actual)` | `unsigned long long` |
| `CHECK_EQUAL_DOUBLE(expected, actual, threshold)` | `double` within ±threshold |
| `CHECK_EQUAL_CHAR(expected, actual)` | `char` |
| `CHECK_EQUAL_UBYTE(expected, actual)` | `unsigned char` |
| `CHECK_EQUAL_SBYTE(expected, actual)` | `signed char` |
| `CHECK_EQUAL_STRING(expected, actual)` | `const char*` via `strcmp` |
| `CHECK_EQUAL_POINTER(expected, actual)` | `const void*` |
| `CHECK_EQUAL_MEMCMP(expected, actual, size)` | raw memory via `memcmp` |
| `FAIL()` | unconditional failure (no message) |
| `FAIL_TEXT(text)` | unconditional failure with message |
| `TEST_PROPERTY(name, value)` | attaches metadata to JUnit output |

Each macro also has a `_TEXT` variant that accepts an extra message string.

---

## C Test Definition Macros

| Macro | Purpose |
|-------|---------|
| `TEST_GROUP_SETUP(group)` | Defines the setup function for group |
| `TEST_GROUP_TEARDOWN(group)` | Defines the teardown function for group |
| `TEST(group, name)` | Defines a test function body |
| `IGNORE_TEST(group, name)` | Defines an ignored test body |
| `EXPECT_FAIL_TEST(group, name)` | Defines an expected-failure test body |

---

## C++ Wrapper Macros

Each C macro that defines a test body has a matching C++ wrapper:

| C++ wrapper | What it creates |
|-------------|----------------|
| `TEST_GROUP_C_WRAPPER(group)` | A `TEST_GROUP` with optional setup/teardown wiring |
| `TEST_GROUP_C_SETUP_WRAPPER(group)` | Calls the C setup function from C++ `setup()` |
| `TEST_GROUP_C_TEARDOWN_WRAPPER(group)` | Calls the C teardown function from C++ `teardown()` |
| `TEST_C_WRAPPER(group, name)` | A `TEST` that calls the C test function |
| `IGNORE_TEST_C_WRAPPER(group, name)` | An `IGNORE_TEST` that calls the C ignored function |
| `EXPECT_FAIL_TEST_C_WRAPPER(group, name)` | An `EXPECT_FAIL_TEST` that calls the C expected-fail function |

---

## C Mock Interface (`CppMu/MockSupport.h`)

The C mock API exposes `struct CppMuMockSupport` through a `cppmu_mock()` function that returns a pointer to a struct of function pointers. This allows chaining calls in C99:

```c
#include "CppMu/MockSupport.h"

/* expect */
cppmu_mock()->expect_one_call("send")
    ->with_int_parameters("fd", 3)
    ->and_return_int_value(8);

/* actual (in stub) */
int n = (int)cppmu_mock()
    ->actual_call("send")
    ->with_int_parameters("fd", fd)
    ->int_return_value();
```

For named scopes: `cppmu_mock_scope("name")` returns a pointer to the named `CppMuMockSupport`.

### Data store in C

```c
cppmu_mock()->set_int_data("retval", 42);
int v = cppmu_mock()->get_data("retval").value.int_value;
```

### Custom comparators in C

```c
static int my_equal(const void* a, const void* b) { return a == b; }
static const char* my_to_string(const void* a) { (void)a; return "obj"; }

cppmu_mock()->install_comparator("MyType", my_equal, my_to_string);
```

See `examples/tests/MockDocumentation.test.c` for a complete C mock example.

---

## Examples

| Files | Demonstrates |
|-------|-------------|
| [`examples/tests/hello.test.c`](../examples/tests/hello.test.c) + [`hello.test.cpp`](../examples/tests/hello.test.cpp) | Two-file pattern: stubs a function pointer with `CPPMU_PTR_SET`, checks output with `CHECK_EQUAL_STRING` |
| [`examples/tests/MockDocumentation.test.c`](../examples/tests/MockDocumentation.test.c) + [`MockDocumentation.test.cpp`](../examples/tests/MockDocumentation.test.cpp) | C mock interface: `cppmu_mock()`, typed parameters, custom comparator, data store |
