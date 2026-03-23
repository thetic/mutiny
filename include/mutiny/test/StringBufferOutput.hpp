#ifndef INCLUDED_MUTINY_STRINGBUFFERTESTOUTPUT_HPP
#define INCLUDED_MUTINY_STRINGBUFFERTESTOUTPUT_HPP

#include "mutiny/test/Output.hpp"

namespace mu {
namespace tiny {
namespace test {

///////////////////////////////////////////////////////////////////////////////
//
//  TestOutput for test purposes
//
///////////////////////////////////////////////////////////////////////////////

class StringBufferTestOutput : public TestOutput
{
public:
  explicit StringBufferTestOutput() = default;

  ~StringBufferTestOutput() override = default;

  StringBufferTestOutput(const StringBufferTestOutput&) = delete;
  StringBufferTestOutput& operator=(const StringBufferTestOutput&) = delete;

  void print_buffer(const char* s) override { output_ += s; }

  void flush() override { output_ = ""; }

  const String& get_output() { return output_; }

protected:
  String output_;
};

}
}
} // namespace mu::tiny::test

#endif
