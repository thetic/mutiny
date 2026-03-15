#include "CppUTest/math.hpp"

#include <math.h>

#if defined(_WIN32)
#include <float.h>
#endif

namespace {

int
IsNanImpl(double d)
{
#if defined(_MSC_VER)
  return _isnan(d);
#else
  return isnan(d);
#endif
}

int
IsInfImpl(double d)
{
#if defined(_MSC_VER)
  return !_finite(d);
#else
  return isinf(d);
#endif
}

} // namespace

namespace cpputest {

int (*IsNan)(double) = IsNanImpl;
int (*IsInf)(double) = IsInfImpl;

} // namespace cpputest
