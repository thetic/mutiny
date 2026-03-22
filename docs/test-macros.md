# Test Definition Macros

Include `"CppMu/CppMu.hpp"` for all macros on this page.

## Test Group and Lifecycle

### `TEST_GROUP(group)`

Declares a test group. Expands to a struct definition; add `setup()` and `teardown()` inside it.

```cpp
TEST_GROUP(MyGroup)
{
    MyObject* obj;

    void setup() override    { obj = new MyObject; }
    void teardown() override { delete obj; }
};
```

All tests in the group share a fresh instance of the struct (constructed before `setup()`, destroyed after `teardown()`).

### `TEST_SETUP()` / `TEST_TEARDOWN()`

Convenience aliases for declaring `setup()` and `teardown()` inside a `TEST_GROUP` body. Equivalent to writing `virtual void setup() override`.

```cpp
TEST_GROUP(Counter)
{
    int count;
    TEST_SETUP()    { count = 0; }
    TEST_TEARDOWN() { /* cleanup */ }
};
```

### `TEST(group, name)`

Defines a test belonging to `group`. The `{ ... }` block that follows is the test body.

```cpp
TEST(MyGroup, DoesTheThing)
{
    CHECK(obj->do_thing());
}
```

## Custom Base Classes

### `TEST_BASE(ClassName)`

Defines a custom base class struct that inherits from `cppmu::Test`. Use when you want a reusable base that multiple groups can share.

```cpp
TEST_BASE(LoggingTestBase)
{
    void setup() override   { start_log(); }
    void teardown() override { end_log(); }
};
```

### `TEST_GROUP_BASE(group, baseclass)`

Like `TEST_GROUP`, but inherits from `baseclass` instead of `cppmu::Test`.

```cpp
TEST_GROUP_BASE(Networking, LoggingTestBase)
{
    // inherits setup/teardown from LoggingTestBase
};
```

## Skipping and Expected Failures

### `IGNORE_TEST(group, name)`

Marks a test as ignored. It is registered but skipped during normal runs. Use `-ri` on the command line to run ignored tests anyway.

```cpp
IGNORE_TEST(MyGroup, NotImplementedYet)
{
    // will not run unless -ri is passed
    CHECK(false);
}
```

Ignored tests appear in the summary count as "ignored".

### `EXPECT_FAIL_TEST(group, name)`

Declares a test that is expected to fail. The test runner treats a failure as a pass and a pass as a failure. Useful for documenting known bugs that cannot yet be fixed.

```cpp
EXPECT_FAIL_TEST(MyGroup, KnownBug)
{
    CHECK_EQUAL(expected, buggy_function()); // currently produces wrong result
}
```

## Metadata

### `TEST_PROPERTY(name, value)`

Attaches a key/value metadata pair to the currently running test. Emitted in JUnit XML output as `<property>` elements. Useful for CI tagging (e.g. requirement IDs).

```cpp
TEST(MyGroup, Requirement42)
{
    TEST_PROPERTY("requirement", "REQ-42");
    CHECK(meets_requirement_42());
}
```

## C Wrapper Macros

These bridge C test files into the C++ test runner. See [testing-in-c.md](testing-in-c.md) for full details.

| Macro | Purpose |
|-------|---------|
| `TEST_GROUP_C_WRAPPER(group)` | Declares a C++ `TEST_GROUP` that calls C setup/teardown |
| `TEST_GROUP_C_SETUP_WRAPPER(group)` | Wires C setup into the C++ group's `setup()` |
| `TEST_GROUP_C_TEARDOWN_WRAPPER(group)` | Wires C teardown into the C++ group's `teardown()` |
| `TEST_C_WRAPPER(group, name)` | Wires a C test function into a C++ `TEST` |
| `IGNORE_TEST_C_WRAPPER(group, name)` | Same as above but ignored |
| `EXPECT_FAIL_TEST_C_WRAPPER(group, name)` | Same as above but expected to fail |

## Examples

| File | Demonstrates |
|------|-------------|
| [`examples/tests/CheatSheet.test.cpp`](../examples/tests/CheatSheet.test.cpp) | `TEST_GROUP`, `TEST`, `setup()`, `teardown()`, and `CPPMU_PTR_SET` |
| [`examples/tests/hello.test.c`](../examples/tests/hello.test.c) + [`hello.test.cpp`](../examples/tests/hello.test.cpp) | C-wrapper two-file pattern with `TEST_GROUP_SETUP`, `TEST`, and `TEST_C_WRAPPER` |
