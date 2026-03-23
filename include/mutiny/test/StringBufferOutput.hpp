#ifndef INCLUDED_MUTINY_TEST_STRINGBUFFEROUTPUT_HPP
#define INCLUDED_MUTINY_TEST_STRINGBUFFEROUTPUT_HPP

#include "mutiny/test/Output.hpp"

namespace mu {
namespace tiny {
namespace test {

///////////////////////////////////////////////////////////////////////////////
//
//  Output for test purposes
//
///////////////////////////////////////////////////////////////////////////////

class StringBufferOutput : public Output
{
public:
  explicit StringBufferOutput() = default;

  ~StringBufferOutput() override = default;

  StringBufferOutput(const StringBufferOutput&) = delete;
  StringBufferOutput& operator=(const StringBufferOutput&) = delete;

  void print_buffer(const char* s) override { output_ += s; }

  void flush() override { output_ = ""; }

  const String& get_output() { return output_; }

protected:
  String output_;
};

} // namespace test
} // namespace tiny
} // namespace mu

#endif
