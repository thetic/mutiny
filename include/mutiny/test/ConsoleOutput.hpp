/**
 * @file
 * @brief printf-based @ref mu::tiny::test::Output implementation for console
 * logging.
 *
 * The default @ref mu::tiny::test::Output used by @ref
 * mu::tiny::test::CommandLineRunner.
 */

#ifndef INCLUDED_MUTINY_TEST_CONSOLEOUTPUT_HPP
#define INCLUDED_MUTINY_TEST_CONSOLEOUTPUT_HPP

#include "mutiny/test/Output.hpp"

#include "mutiny/export.h"

namespace mu {
namespace tiny {
namespace test {

/**
 * @brief Writes test output to stdout using printf.
 *
 * This is the default @ref Output used by @ref CommandLineRunner. It relies on
 * the replaceable @ref Output::stdout_, @ref Output::fopen_,
 * @ref Output::fputs_, and @ref Output::fclose_ function pointers, allowing
 * I/O to be redirected in tests.
 */
class MUTINY_EXPORT ConsoleOutput : public Output
{
public:
  explicit ConsoleOutput() = default;
  ~ConsoleOutput() override = default;

  /**
   * @brief Write @p s to the console via the fputs_ function pointer.
   * @param s  Null-terminated string to write.
   */
  void print_buffer(const char* s) override;

  /** @brief Flush all pending output to the console. */
  void flush() override;

  ConsoleOutput(const ConsoleOutput&) = delete;
  ConsoleOutput& operator=(const ConsoleOutput&) = delete;
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
