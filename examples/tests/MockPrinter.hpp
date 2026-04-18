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

#include "mu/tiny/String.hpp"

#include <stdlib.h>

class MockPrinter : public Printer
{
public:
  explicit MockPrinter() = default;
  ~MockPrinter() override = default;

  MockPrinter(const MockPrinter&) = delete;
  MockPrinter& operator=(const MockPrinter&) = delete;

  void print(const char* s) override { saved_output_ += s; }

  void print(long int value) override
  {
    mu::tiny::String buffer;
    buffer = mu::tiny::string_from_format("%ld", value);
    saved_output_ += buffer.c_str();
  }

  mu::tiny::String get_output() const { return saved_output_; }

private:
  mu::tiny::String saved_output_;
};

#endif // D_MockPrinter_H
