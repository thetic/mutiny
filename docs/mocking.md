# Mocking

Include `"mutiny/mock.hpp"` for the C++ mock API.

## Accessing the Mock Object

```cpp
mu::tiny::mock::mock()           // global mock scope
mu::tiny::mock::mock("network")  // named scope — keeps expectations separate
```

Named scopes are independent: `mock("db")` and `mock("net")` have separate expectation lists.

## Basic Workflow

Every test that uses mocks should follow this pattern:

```cpp
TEST(MyGroup, CallsNetworkSend)
{
    // 1. Set expectations
    mock("net").expect_one_call("send")
               .with_parameter("data", buffer)
               .and_return_value(42);

    // 2. Exercise the code under test
    int result = subject.send_data(buffer);

    // 3. Verify
    CHECK_EQUAL(42, result);
    mock().check_expectations(); // fails if expected calls were not made
    mock().clear();              // reset for next test
}
```

`check_expectations()` + `clear()` in teardown is cleaner than per-test calls. The `MockSupportPlugin` does this automatically — see [plugins.md](plugins.md).

---

## Expecting Calls

| Method | Description |
|--------|-------------|
| `mock().expect_one_call("func")` | Expect exactly one call to `"func"` |
| `mock().expect_n_calls(n, "func")` | Expect exactly `n` calls |
| `mock().expect_no_call("func")` | Expect that `"func"` is never called |

---

## Parameter Matching

Chain `.with_parameter(name, value)` on an expected call to constrain parameter values. The overloaded `with_parameter` accepts: `bool`, `int`, `unsigned int`, `long`, `unsigned long`, `long long`, `unsigned long long`, `double`, `const char*`, `void*`, `const void*`, `void(*)()`, and memory buffers `(const unsigned char*, size_t)`.

```cpp
mock().expect_one_call("write")
      .with_parameter("fd", 1)
      .with_parameter("buf", data_ptr)
      .with_parameter("count", (size_t)8);
```

For `double` with a tolerance:
```cpp
mock().expect_one_call("scale")
      .with_parameter("factor", 1.0, 0.001); // value ± tolerance
```

### Custom Types

Use `with_parameter_of_type(type_name, param_name, ptr)` for non-native types. A comparator must be installed first — see [Custom Comparators](#custom-comparators) below.

```cpp
mock().expect_one_call("process")
      .with_parameter_of_type("Packet", "pkt", &expected_packet);
```

### Relaxing Parameter Checks

`.ignore_other_parameters()` on an expected call: any extra parameters the actual call provides beyond the matched set are ignored.

`mock().ignore_other_calls()` on the mock support: silently accept any call that has no matching expectation.

---

## Output Parameters

Sometimes the mock needs to write data back to the caller through a pointer parameter.

On the expected side:
```cpp
mock().expect_one_call("read")
      .with_output_parameter_returning("buf", &source_data, sizeof(source_data));
```

On the actual call side (in your mock implementation):
```cpp
mock().actual_call("read")
      .with_output_parameter("buf", output_buffer);
```

The framework copies `source_data` into `output_buffer` when the call is matched.

For custom types use `with_output_parameter_of_type_returning` + `with_output_parameter_of_type` with a registered copier.

---

## Return Values

Set the return value on the expected call with `and_return_value(value)`. The overload accepts all basic types plus `void*`, `const void*`, and `void(*)()`.

Retrieve the return value in your mock stub with the typed accessors on `MockActualCall` or `MockSupport`:

```cpp
// In the mock stub:
return mock().actual_call("compute")
             .with_parameter("x", x)
             .return_int_value();

// Or with a default if no expectation was set:
return mock().actual_call("compute")
             .with_parameter("x", x)
             .return_int_value_or_default(0);
```

Available typed getters: `return_bool_value`, `return_int_value`, `return_unsigned_int_value`, `return_long_int_value`, `return_unsigned_long_int_value`, `return_long_long_int_value`, `return_unsigned_long_long_int_value`, `return_double_value`, `return_string_value`, `return_pointer_value`, `return_const_pointer_value`, `return_function_pointer_value`. All have `_or_default` variants.

---

## Object Binding

Use `.on_object(ptr)` to scope an expectation to a specific object instance. Useful when multiple objects of the same type are in play.

```cpp
Widget w1, w2;
mock().expect_one_call("tick").on_object(&w1);
// only a call on &w1 satisfies this expectation
```

In the mock stub:
```cpp
mock().actual_call("tick").on_object(this);
```

---

## Call Ordering

By default calls can occur in any order. To enforce order:

```cpp
mock().strict_order();
mock().expect_one_call("first") .with_call_order(1);
mock().expect_one_call("second").with_call_order(2);
mock().expect_one_call("third") .with_call_order(3);
```

For a range of acceptable positions: `.with_call_order(low, high)`.

---

## Enable / Disable / Tracing

```cpp
mock().disable();  // all actual_call() calls are silently accepted
mock().enable();   // back to normal checking

mock().tracing(true);   // print each actual call as it happens
mock().crash_on_failure();  // crash (useful with a debugger) instead of reporting failure
```

---

## Data Store

The mock object doubles as a key/value store. Pass data between the test and stub code without extra globals:

```cpp
// In test setup:
mock().set_data("timeout_ms", 100);

// In mock stub:
int timeout = mock().get_data("timeout_ms").get_int_value();
```

`set_data` is overloaded for: `bool`, `int`, `unsigned int`, `long`, `unsigned long`, `const char*`, `double`, `void*`, `const void*`, `void(*)()`. For object types: `set_data_object` / `set_data_const_object`.

---

## Custom Comparators

Install a comparator to make `with_parameter_of_type` work for your type:

### Template comparator (simplest)

```cpp
TypedMockComparator<Packet> packet_cmp;
mock().install_comparator("Packet", packet_cmp);
// Requires: operator==(const Packet&, const Packet&)
// and: mu::tiny::test::String string_from(const Packet&)
```

### Function-based comparator

```cpp
MockFunctionComparator packet_cmp(
    [](const void* a, const void* b) {
        return *static_cast<const Packet*>(a) == *static_cast<const Packet*>(b);
    },
    [](const void* a) -> mu::tiny::test::String {
        return static_cast<const Packet*>(a)->to_string();
    }
);
mock().install_comparator("Packet", packet_cmp);
```

### Subclass comparator

```cpp
class PacketComparator : public mu::tiny::mock::MockNamedValueComparator
{
public:
    bool is_equal(const void* a, const void* b) override {
        return *static_cast<const Packet*>(a) == *static_cast<const Packet*>(b);
    }
    mu::tiny::test::String value_to_string(const void* a) override {
        return static_cast<const Packet*>(a)->to_string();
    }
};
```

Remove all installed comparators and copiers with `mock().remove_all_comparators_and_copiers()`.

---

## Custom Copiers

Copiers are needed when using output parameters of custom types:

```cpp
TypedMockCopier<Packet> packet_copier;
mock().install_copier("Packet", packet_copier);
// Requires: Packet::operator=
```

Or function-based:

```cpp
MockFunctionCopier packet_copier([](void* dst, const void* src) {
    *static_cast<Packet*>(dst) = *static_cast<const Packet*>(src);
});
mock().install_copier("Packet", packet_copier);
```

---

## MockSupportPlugin

`MockSupportPlugin` automatically calls `check_expectations()` and `clear()` after every test, and manages comparator/copier lifetime:

```cpp
// main.cpp
mu::tiny::mock::MockSupportPlugin mock_plugin;
mock_plugin.install_comparator("Packet", packet_cmp);
reg->install_plugin(&mock_plugin);
```

With the plugin installed, tests no longer need explicit `check_expectations()`/`clear()` calls. See [plugins.md](plugins.md).

## Examples

| File | Demonstrates |
|------|-------------|
| [`examples/tests/MockCheatSheet.test.cpp`](../examples/tests/MockCheatSheet.test.cpp) | `expect_one_call`, `expect_n_calls`, `with_parameter`, `and_return_value`, `ignore_other_parameters`, `check_expectations`, `clear` |
| [`examples/tests/MockDocumentation.test.cpp`](../examples/tests/MockDocumentation.test.cpp) | `on_object`, `set_data`, custom comparator, scoped mock, `crash_on_failure`, `disable`/`enable`, `ignore_other_calls` |
| [`examples/tests/EventDispatcher.test.cpp`](../examples/tests/EventDispatcher.test.cpp) | Real-world example: virtual mock class, custom comparator installed in `setup()`, `with_parameter_of_type` |
| [`examples/tests/main.cpp`](../examples/tests/main.cpp) | Installing `MockSupportPlugin` with a custom comparator alongside other plugins |
