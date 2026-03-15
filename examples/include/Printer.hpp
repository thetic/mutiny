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
  explicit Printer();
  virtual ~Printer();

  virtual void print(const char*);
  virtual void print(long int);

private:
  Printer(const Printer&);
  Printer& operator=(const Printer&);
};

Printer&
operator<<(Printer&, const char*);
Printer&
operator<<(Printer&, long int);

#endif // INCLUDED_PRINTER_HPP
