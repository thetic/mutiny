#include "mu/tiny/math.hpp"

#include <math.h>

#if defined(_WIN32)
#include <float.h>
#endif

namespace mu {
namespace tiny {
namespace {

int is_nan_impl(double d)
{
#if defined(_MSC_VER)
  return _isnan(d);
#else
  return isnan(d);
#endif
}

int is_inf_impl(double d)
{
#if defined(_MSC_VER)
  return !_finite(d);
#else
  return isinf(d);
#endif
}

} // namespace

int (*is_nan)(double) = is_nan_impl;
int (*is_inf)(double) = is_inf_impl;

} // namespace tiny
} // namespace mu
