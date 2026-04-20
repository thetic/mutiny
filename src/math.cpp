#include "mu/tiny/math.hpp"

#include <math.h>

#ifdef _WIN32
#include <float.h>
#endif

namespace mu {
namespace tiny {
namespace {

int is_nan_impl(double d)
{
#ifdef _MSC_VER
  return _isnan(d);
#else
  return static_cast<int>(isnan(d));
#endif
}

int is_inf_impl(double d)
{
#ifdef _MSC_VER
  return !_finite(d);
#else
  return static_cast<int>(isinf(d));
#endif
}

} // namespace

int (*is_nan)(double) = is_nan_impl;
int (*is_inf)(double) = is_inf_impl;

} // namespace tiny
} // namespace mu
