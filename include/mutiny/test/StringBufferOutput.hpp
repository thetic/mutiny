/**
 * @file
 * @brief In-memory @ref mu::tiny::test::Output implementation for use in tests.
 *
 * Used by @ref mu::tiny::test::TestingFixture to capture output for assertions.
 */

#ifndef INCLUDED_MUTINY_TEST_STRINGBUFFEROUTPUT_HPP
#define INCLUDED_MUTINY_TEST_STRINGBUFFEROUTPUT_HPP

#include "mutiny/test/Output.hpp"

#include "mutiny/export.h"

namespace mu {
namespace tiny {
namespace test {

/**
 * @brief Captures all output into an in-memory String for later inspection.
 *
 * Used by @ref TestingFixture to collect the output from a test run so that
 * tests can assert on what was printed.
 */
class MUTINY_EXPORT StringBufferOutput : public Output
{
public:
  explicit StringBufferOutput() = default;

  ~StringBufferOutput() override = default;

  StringBufferOutput(const StringBufferOutput&) = delete;
  StringBufferOutput& operator=(const StringBufferOutput&) = delete;

  /**
   * @brief Append @p s to the internal output buffer.
   * @param s  Null-terminated string to append.
   */
  void print_buffer(const char* s) override { output_ += s; }

  /** @brief Clear the internal output buffer. */
  void flush() override { output_ = ""; }

  /**
   * @return A reference to the accumulated output string.
   */
  const String& get_output() { return output_; }

protected:
  String output_;
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
