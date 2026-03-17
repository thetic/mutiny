#include "IEEE754ExceptionsPlugin.h"

static volatile float f;
static volatile float zero_float = 0.0f;

void set_divisionbyzero_c(void)
{
  f = 1.0f;
  f /= zero_float;
}

void set_overflow_c(void)
{
  f = 1e38f;
  f *= f;
}

void set_underflow_c(void)
{
  f = 1e-38f;
  f *= f;
}

void set_inexact_c(void)
{
  f = 10.0f;
  f /= 3.0f;
}

void set_nothing_c(void) {}

void set_everything_c(void)
{
  set_divisionbyzero_c();
  set_overflow_c();
  set_underflow_c();
  set_inexact_c();
}
