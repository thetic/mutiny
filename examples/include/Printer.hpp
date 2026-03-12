#ifndef D_Printer_H
#define D_Printer_H

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

  virtual void Print(const char*);
  virtual void Print(long int);

private:
  Printer(const Printer&);
  Printer& operator=(const Printer&);
};

Printer&
operator<<(Printer&, const char*);
Printer&
operator<<(Printer&, long int);

#endif // D_Printer_H
