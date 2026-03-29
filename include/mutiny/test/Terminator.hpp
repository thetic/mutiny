/**
 * @file
 * @brief Strategies for ending a failing test.
 *
 * Shell calls the active Terminator when a test must stop. The concrete
 * subclasses implement normal (exception-based), exception-free, and
 * crash-on-failure exit strategies.
 *
 * @see Shell::get_current_test_terminator()
 */

#ifndef INCLUDED_MUTINY_TEST_TERMINATOR_HPP
#define INCLUDED_MUTINY_TEST_TERMINATOR_HPP

#include "mutiny/export.h"

namespace mu {
namespace tiny {
namespace test {

/**
 * @brief Abstract strategy for exiting a failing test.
 *
 * Implement exit_current_test() to control how the framework stops the
 * currently running test (throw, longjmp, crash, etc.).
 */
class MUTINY_EXPORT Terminator
{
public:
  /** @brief Exit the currently running test immediately. */
  [[noreturn]] virtual void exit_current_test() const = 0;
  virtual ~Terminator() = default;
};

/**
 * @brief Standard terminator that exits via a thrown FailedException.
 *
 * Used by default when exceptions are available.
 *
 * @see Shell::get_current_test_terminator()
 */
class MUTINY_EXPORT NormalTerminator : public Terminator
{
public:
  [[noreturn]] void exit_current_test() const override;
  ~NormalTerminator() override = default;
};

/**
 * @brief Terminator for builds without exception support.
 *
 * Uses a platform-specific non-exception mechanism (e.g. `setjmp`/`longjmp`)
 * to unwind from a failing test.
 *
 * @see Shell::get_current_test_terminator_without_exceptions()
 */
class MUTINY_EXPORT TerminatorWithoutExceptions : public Terminator
{
public:
  [[noreturn]] void exit_current_test() const override;
  ~TerminatorWithoutExceptions() override = default;
};

/**
 * @brief Terminator that crashes the process on failure.
 *
 * Extends NormalTerminator so that test failures also trigger a hard crash
 * (e.g. for use with crash-dump or post-mortem debugging).
 *
 * @see Shell::set_crash_on_fail()
 */
class MUTINY_EXPORT CrashingTerminator : public NormalTerminator
{
public:
  [[noreturn]] void exit_current_test() const override;
  ~CrashingTerminator() override = default;
};

/**
 * @brief Exception-free variant of CrashingTerminator.
 *
 * Combines the no-exceptions unwinding strategy with a hard crash on failure.
 *
 * @see Shell::set_crash_on_fail()
 */
class MUTINY_EXPORT CrashingTerminatorWithoutExceptions
  : public TerminatorWithoutExceptions
{
public:
  [[noreturn]] void exit_current_test() const override;
  ~CrashingTerminatorWithoutExceptions() override = default;
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
