#ifndef INCLUDED_CPPMU_STRINGBUFFERTESTOUTPUT_HPP
#define INCLUDED_CPPMU_STRINGBUFFERTESTOUTPUT_HPP

#include "CppMu/TestOutput.hpp"

namespace cppmu {

///////////////////////////////////////////////////////////////////////////////
//
//  TestOutput for test purposes
//
///////////////////////////////////////////////////////////////////////////////

class StringBufferTestOutput : public TestOutput
{
public:
  explicit StringBufferTestOutput() {}

  virtual ~StringBufferTestOutput() override;

  void print_buffer(const char* s) override { output_ += s; }

  void flush() override { output_ = ""; }

  const String& get_output() { return output_; }

protected:
  String output_;

private:
  StringBufferTestOutput(const StringBufferTestOutput&);
  StringBufferTestOutput& operator=(const StringBufferTestOutput&);
};

} // namespace cppmu

#endif
