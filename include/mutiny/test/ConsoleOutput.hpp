#ifndef INCLUDED_MUTINY_TEST_CONSOLEOUTPUT_HPP
#define INCLUDED_MUTINY_TEST_CONSOLEOUTPUT_HPP

#include "mutiny/test/Output.hpp"

namespace mu {
namespace tiny {
namespace test {

///////////////////////////////////////////////////////////////////////////////
//
//  Printf Based Solution
//
///////////////////////////////////////////////////////////////////////////////

class ConsoleOutput : public Output
{
public:
  explicit ConsoleOutput() = default;
  ~ConsoleOutput() override = default;

  void print_buffer(const char* s) override;
  void flush() override;

  ConsoleOutput(const ConsoleOutput&) = delete;
  ConsoleOutput& operator=(const ConsoleOutput&) = delete;
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
