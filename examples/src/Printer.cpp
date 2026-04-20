#include "Printer.hpp"

#include <stdio.h>

void Printer::print(const char* s)
{
  for (const char* p = s; *p != 0; p++) {
    putchar(*p);
  }
}

void Printer::print(long int n)
{
  printf("%ld", n);
}

Printer& operator<<(Printer& p, const char* s)
{
  p.print(s);
  return p;
}

Printer& operator<<(Printer& p, long int i)
{
  p.print(i);
  return p;
}
