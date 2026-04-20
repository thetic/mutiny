#ifndef INCLUDED_PRINTER_HPP
#define INCLUDED_PRINTER_HPP

///////////////////////////////////////////////////////////////////////////////
//
//  Printer is responsible for ...
//
///////////////////////////////////////////////////////////////////////////////

class Printer
{
public:
  explicit Printer() = default;
  virtual ~Printer() = default;

  Printer(const Printer&) = delete;
  Printer& operator=(const Printer&) = delete;

  virtual void print(const char* s);
  virtual void print(long int n);
};

Printer& operator<<(Printer& p, const char* s);
Printer& operator<<(Printer& p, long int i);

#endif // INCLUDED_PRINTER_HPP
