<img src="docs/_static/logo.png" alt="mutiny" width="300">

[![Basic builds](https://github.com/thetic/mutiny/actions/workflows/basic.yml/badge.svg)](https://github.com/thetic/mutiny/actions/workflows/basic.yml)
[![Coverage Status](https://coveralls.io/repos/github/thetic/mutiny/badge.svg?branch=main)](https://coveralls.io/github/thetic/mutiny?branch=main)

---

mutiny is a C/C++ unit testing and mocking framework suited for embedded and
low-resource targets.

## Quick Start

Build and install:

```bash
cmake -B build
cmake --build build
```

Add to your project with FetchContent:

```cmake
include(FetchContent)
FetchContent_Declare(mutiny GIT_REPOSITORY https://github.com/thetic/mutiny.git GIT_TAG main)
FetchContent_MakeAvailable(mutiny)
target_link_libraries(my_tests PRIVATE mu::tiny)
```

Write a test:

```cpp
#include "mutiny/test.hpp"

TEST_GROUP(MyGroup) {};

TEST(MyGroup, Passes) { CHECK_EQUAL(42, compute()); }
```

Run it:

```bash
./my_tests -v
```

## Attribution

mutiny is descended from [CppUTest](https://github.com/cpputest/cpputest),
a unit testing framework originally created by Michael Feathers and maintained
by the CppUTest community. The test API, assertion macros, mock framework, and
core architecture are all derived from that project.
