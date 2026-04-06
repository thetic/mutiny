# Contributing to *Mu::tiny*

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
./build/GNU/tests/unit/mutiny_unit -v
./build/GNU/tests/integration/mutiny_integration -g String -n Contains
```

## Code Formatting

All C/C++ files must be formatted with `clang-format` before committing. Format all staged files:

```bash
git ls-files '*.cpp' '*.h' '*.c' | xargs clang-format -i
```

The style is `BasedOnStyle: Mozilla` with include-regroup and a few overrides — see `.clang-format`. **Do not run `clang-format` on `CMakeLists.txt` files** — it has no CMake parser and will mangle the syntax.

## C++ Conventions

These are enforced by CI via `clang-tidy` and code review:

- **Anonymous namespaces** instead of file-scope `static` in `.cpp` files ([SF.22](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rs-unnamed2)).
- **`using`** instead of `typedef` ([T.43](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rt-using)). Function pointer form: `using F = void (*)()`.
- **`auto`** to avoid repeating the type when the LHS type equals the RHS constructed type ([ES.11](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#res-auto)). Use `auto*` for pointer types; bare `auto` for function pointer casts. Skip the polymorphic base-pointer pattern (`Base* p = new Derived(...)`) — that is intentional.
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

All new code must have test coverage. Tests live in `tests/unit/` for testing internal framework classes, or `tests/integration/` for testing the public API. Each test file maps 1-to-1 with the header it tests (`foo.test.cpp` tests `foo.hpp`). One `TEST_GROUP` per file, named after the file's basename.

Note: Unit tests are typically disabled when building `mutiny` as a shared library (`BUILD_SHARED_LIBS=ON`) because they require access to private symbols. New features should prioritize integration tests that exercise the public API.

When testing framework internals (behaviors that are supposed to fail), use `TestingFixture` to run a nested test scenario rather than making your outer test fail.

## File and Header Organization

- C++ headers: `.hpp`; C interface headers: `.h`
- Test files: `X.test.cpp` / `X.test.c`
- One class per header unless the class is very small
- A single `.cpp` may implement both the C++ and C headers for the same module

## Documentation

User-facing features must be documented in `docs/`. The docs are written in reStructuredText (`.rst`) and built with Sphinx. Run `uv run --project docs sphinx-build docs docs/_build` to preview locally.

## Submitting Changes

1. Make sure all existing builds pass.
2. Make sure new code is covered by tests.
3. Document any user-facing changes in `docs/`.
4. Format all changed files with `clang-format` (see above).
5. Keep commits focused — one logical change per commit.
6. Commit messages should describe _why_, not just _what_.

## Semantic Versioning

This project follows [Semantic Versioning 2.0.0](https://semver.org/).
The version is defined in the `VERSION` file in the root directory and is used to generate the version header `mutiny/test/version.hpp`.

### Incrementing the Version

- **MAJOR** version: Incompatible source API changes.
  - Examples: Removing or renaming public headers/functions/classes, changing function signatures in `include/mutiny/`, dropping support for a C++ standard version (e.g., C++11 to C++14).
- **MINOR** version: Backwards compatible functionality.
  - Examples: Adding new classes/functions/macros to the public API.
- **PATCH** version: Backwards compatible bug fixes.
  - Examples: Internal logic fixes, performance improvements, documentation.

### Enforcement

Before submitting a pull request, please consider if your changes require a version bump.
Reviewers will check for breaking changes to ensure the MAJOR version is incremented if necessary.

## Creating a Release

1. Update the `VERSION` file with the new version number.
2. Merge to `main`.
3. Create a GitHub release tagged `v<version>` (e.g. `v1.2.3`).

CI will automatically generate a source archive and attach it to the release.
