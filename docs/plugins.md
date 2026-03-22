# Plugins

Plugins let you run code before and after every test, parse custom command-line flags, and create additional output formatters. CppMu ships several built-in plugins; you can also write your own.

## `TestPlugin` Base Class

```cpp
// include/CppMu/TestPlugin.hpp
class TestPlugin
{
public:
    TestPlugin(const String& name);

    virtual void pre_test_action(TestShell&, TestResult&)  {}
    virtual void post_test_action(TestShell&, TestResult&) {}

    virtual bool parse_arguments(int argc, const char* const* argv, int index)
    { return false; }

    virtual String get_help() const { return ""; }
    virtual TestOutput* create_output() { return nullptr; }
};
```

Override the methods you need:

| Method | When called | Typical use |
|--------|-------------|-------------|
| `pre_test_action` | Before each test body | Reset global state, start timers |
| `post_test_action` | After each test body | Restore pointers, verify mock expectations |
| `parse_arguments` | During argument parsing | Activate plugin via a `-p<flag>` argument |
| `get_help` | When `-h` is printed | Show plugin's flag description |
| `create_output` | At startup | Return a `TestOutput*` for additional output format |

## Installing Plugins

Install plugins in `main()` before calling `run_all_tests`:

```cpp
#include "CppMu/CommandLineTestRunner.hpp"
#include "CppMu/TestRegistry.hpp"
#include "MyPlugin.hpp"

int main(int argc, char** argv)
{
    MyPlugin my_plugin;
    cppmu::TestRegistry::get_current_registry()->install_plugin(&my_plugin);
    return cppmu::CommandLineTestRunner::run_all_tests(argc, argv);
}
```

Plugins form a linked list; they run in installation order.

## Writing a Custom Plugin

Example: a plugin that logs the name of every test that runs.

```cpp
// LogPlugin.hpp
#include "CppMu/TestPlugin.hpp"
#include "CppMu/TestShell.hpp"
#include <cstdio>

class LogPlugin : public cppmu::TestPlugin
{
public:
    LogPlugin() : cppmu::TestPlugin("LogPlugin") {}

    void pre_test_action(cppmu::TestShell& test, cppmu::TestResult&) override
    {
        std::printf("[LOG] running %s::%s\n",
                    test.get_group(), test.get_name());
    }
};
```

Use it in `main()`:

```cpp
LogPlugin log;
cppmu::TestRegistry::get_current_registry()->install_plugin(&log);
```

## Built-in Plugins

| Plugin | Header | Purpose | Docs |
|--------|--------|---------|------|
| `SetPointerPlugin` | `CppMu/SetPointerPlugin.hpp` | Restore overwritten pointers after each test | [set-pointer-plugin.md](set-pointer-plugin.md) |
| `MockSupportPlugin` | `CppMu/MockSupportPlugin.hpp` | Auto-verify and clear mock expectations | [mocking.md](mocking.md) |
| `JUnitOutputPlugin` | `CppMu/JUnitOutputPlugin.hpp` | Write JUnit XML output via `-pjunit` | [junit-output.md](junit-output.md) |

`SetPointerPlugin` is automatically created and available through the `CPPMU_PTR_SET` macro — you do not need to install it manually if you only use the macro.
