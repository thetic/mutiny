---
name: Bug Report
about: Report a technical issue or unexpected behavior in the *Mu::tiny* framework.
labels: bug
assignees: ''

---

## Description
A clear and concise description of the bug.

## Environment Context
- **Framework Version/Commit:** (e.g., v1.0.0 or commit hash)
- **Target OS:** [e.g. Bare-metal, FreeRTOS, Linux, Windows]
- **Architecture:** [e.g. ARM Cortex-M4, x86_64]
- **Compiler:** [e.g. arm-none-eabi-gcc 12.2, Clang 17, MSVC 2022]
- **C++ Standard:** [e.g. C++11, C++17, C++23]

## Framework Configuration
Please specify your CMake options or relevant preprocessor defines:
- `MUTINY_USE_STD_CPP_LIB`: [ON/OFF]
- `MUTINY_USE_STD_STRING`: [ON/OFF]
- Using C or C++ interface?
- Are you using `mutiny_discover_tests()` in CMake?

## Reproduction Steps
Provide a minimal reproduction case. If possible, provide a snippet of a `TEST` or `TEST_GROUP` that demonstrates the failure.

```cpp
// Example reproduction test case
#include "mutiny/test.hpp"

TEST_GROUP(Reproduction)
{
};

TEST(Reproduction, DemonstratesBug)
{
    // Your code here
    // CHECK_EQUAL(expected, actual);
}
```

## Expected Behavior
What did you expect to happen?

## Actual Behavior
What actually happened? (Include logs, console output, or debugger traces).

## Additional Context
Add any other context about the problem here.
