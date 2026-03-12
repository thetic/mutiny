#include "Printer.hpp"

#include <stdio.h>

Printer::Printer() {}

Printer::~Printer() {}

void
Printer::Print(const char* s)
{
  for (const char* p = s; *p; p++)
    putchar(*p);
}

void
Printer::Print(long int n)
{
  printf("%ld", n);
}

Printer&
operator<<(Printer& p, const char* s)
{
  p.Print(s);
  return p;
}

Printer&
operator<<(Printer& p, long int i)
{
  p.Print(i);
  return p;
}
