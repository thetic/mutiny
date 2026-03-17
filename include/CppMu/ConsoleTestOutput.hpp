#ifndef INCLUDED_CPPMU_CONSOLETESTOUTPUT_HPP
#define INCLUDED_CPPMU_CONSOLETESTOUTPUT_HPP

#include "CppMu/TestOutput.hpp"

namespace cppmu {

///////////////////////////////////////////////////////////////////////////////
//
//  Printf Based Solution
//
///////////////////////////////////////////////////////////////////////////////

class ConsoleTestOutput : public TestOutput
{
public:
  explicit ConsoleTestOutput() = default;
  ~ConsoleTestOutput() override = default;

  void print_buffer(const char* s) override;
  void flush() override;

  ConsoleTestOutput(const ConsoleTestOutput&) = delete;
  ConsoleTestOutput& operator=(const ConsoleTestOutput&) = delete;
};

} // namespace cppmu

#endif
