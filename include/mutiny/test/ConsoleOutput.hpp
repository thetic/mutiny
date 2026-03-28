#ifndef INCLUDED_MUTINY_TEST_CONSOLEOUTPUT_HPP
#define INCLUDED_MUTINY_TEST_CONSOLEOUTPUT_HPP

#include "mutiny/test/Output.hpp"

#include "mutiny/export.h"

namespace mu {
namespace tiny {
namespace test {

///////////////////////////////////////////////////////////////////////////////
//
//  Printf Based Solution
//
///////////////////////////////////////////////////////////////////////////////

class MUTINY_EXPORT ConsoleOutput : public Output
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
