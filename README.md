<img src="docs/_static/logo.png" alt="mutiny" width="300">

---

mutiny is a C/C++ unit testing and mocking framework suited for embedded and
low-resource targets.

## Attribution

mutiny is descended from [CppUTest](https://github.com/cpputest/cpputest),
a unit testing framework originally created by Michael Feathers and maintained
by the CppUTest community. The test API, assertion macros, mock framework, and
core architecture are all derived from that project.

---

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
target_link_libraries(my_tests PRIVATE mutiny::mutiny)
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

---

## Documentation

| Topic | File |
|-------|------|
| Building, project setup, first test | [docs/getting-started.md](docs/getting-started.md) |
| `TEST_GROUP`, `TEST`, `IGNORE_TEST`, lifecycle | [docs/test-macros.md](docs/test-macros.md) |
| `CHECK_EQUAL`, `LONGS_EQUAL`, `STRCMP_EQUAL`, … | [docs/assertions.md](docs/assertions.md) |
| `mock()`, expectations, return values, custom types | [docs/mocking.md](docs/mocking.md) |
| Writing tests in pure C | [docs/testing-in-c.md](docs/testing-in-c.md) |
| `TestPlugin` base class, writing custom plugins | [docs/plugins.md](docs/plugins.md) |
| `MUTINY_PTR_SET` — automatic pointer restore | [docs/set-pointer-plugin.md](docs/set-pointer-plugin.md) |
| `-pjunit`, XML format, CI integration | [docs/junit-output.md](docs/junit-output.md) |
| `TEST_ORDERED`, level-based ordering | [docs/ordered-tests.md](docs/ordered-tests.md) |
| All command-line flags | [docs/command-line-reference.md](docs/command-line-reference.md) |
