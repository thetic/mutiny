# Contributing to mutiny

## Building for Development

Each preset writes to its own directory (`build/<preset>`), so you can have multiple build trees simultaneously:

```bash
cmake --preset GNU    # configure
cmake --build --preset GNU
ctest --preset GNU
```

Or run the full configure → build → test cycle in one command:

```bash
cmake --workflow --preset GNU
```

Other useful presets: `Clang`, `asan`, `coverage`, `clang-tidy`, `no-std-cpp`, `no-rtti`, `no-exceptions`.

Running a single test binary directly (supports `-v`, `-g`, `-n`):

```bash
./build/GNU/tests/CppUTestTests -v
./build/GNU/tests/CppUTestTests -g String -n Contains
```

## Code Formatting

All C/C++ files must be formatted with `clang-format` before committing. Format all staged files:

```bash
git ls-files '*.cpp' '*.h' '*.c' | xargs clang-format -i
```

The style is `BasedOnStyle: Mozilla` with include-regroup and a few overrides — see `.clang-format`. **Do not run `clang-format` on `CMakeLists.txt` files** — it has no CMake parser and will mangle the syntax.

## C++ Conventions

These are enforced by CI via `clang-tidy` and code review:

- **Anonymous namespaces** instead of file-scope `static` in `.cpp` files (SF.22).
- **`using`** instead of `typedef` (T.43). Function pointer form: `using F = void (*)()`.
- **`auto`** to avoid repeating the type when the LHS type equals the RHS constructed type (ES.11). Use `auto*` for pointer types; bare `auto` for function pointer casts. Skip the polymorphic base-pointer pattern (`Base* p = new Derived(...)`) — that is intentional.
- **Include guards**: `INCLUDED_MUTINY_<NAME>_HPP` / `INCLUDED_MUTINY_<NAME>_H`.

## C++ Standard

The minimum supported standard is **C++11**. Library code must compile cleanly under C++11 through C++23; do not rely on features introduced after C++11 without a version guard.

## Standard Library Policy

Library code (anything under `include/mutiny/` and `src/`) must compile with and without the C++ standard library.
The `no-std-cpp` preset enforces this.
This means `<type_traits>`, `<utility>`, `std::enable_if`, and similar are off-limits in library headers and source unless guarded by `MUTINY_USE_STD_CPP_LIB`.

## Warnings Policy

The CI presets enable `-Werror`. All warnings must be clean across all presets before a change is ready.
Warning flags are defined in `cmake/warnings.cmake`.
New warning flags are checked for compiler support before being applied.

## Adding Tests

Tests live in `tests/`. Each test file maps 1-to-1 with the header it tests (`foo.test.cpp` tests `foo.hpp`). One `TEST_GROUP` per file, named after the file's basename.

When testing framework internals (behaviors that are supposed to fail), use `TestTestingFixture` to run a nested test scenario rather than making your outer test fail.

## File and Header Organization

- C++ headers: `.hpp`; C interface headers: `.h`
- Test files: `X.test.cpp` / `X.test.c`
- One class per header unless the class is very small
- A single `.cpp` may implement both the C++ and C headers for the same module

## Submitting Changes

1. Make sure all existing builds pass.
2. Make sure new code is covered by tests.
3. Format all changed files with `clang-format` (see above).
4. Keep commits focused — one logical change per commit.
5. Commit messages should describe _why_, not just _what_.
