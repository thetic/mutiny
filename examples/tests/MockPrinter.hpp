#ifndef D_MockPrinter_H
#define D_MOCK_PRINTER_H

///////////////////////////////////////////////////////////////////////////////
//
//  MockPrinter.h
//
//  MockPrinter is responsible for providing a test stub for Printer
//
///////////////////////////////////////////////////////////////////////////////
#include "Printer.hpp"

#include "CppMu/String.hpp"

#include <stdlib.h>

class MockPrinter : public Printer
{
public:
  explicit MockPrinter() {}
  virtual ~MockPrinter() override {}

  virtual void print(const char* s) override { saved_output_ += s; }

  virtual void print(long int value) override
  {
    cppmu::String buffer;
    buffer = cppmu::string_from_format("%ld", value);
    saved_output_ += buffer.c_str();
  }

  cppmu::String get_output() const { return saved_output_; }

private:
  cppmu::String saved_output_;

  MockPrinter(const MockPrinter&);
  MockPrinter& operator=(const MockPrinter&);
};

#endif // D_MockPrinter_H
