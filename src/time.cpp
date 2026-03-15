#include "CppMu/time.hpp"

#include <time.h>

#if defined(_WIN32)
#include <windows.h>
#elif defined(CPPMU_HAVE_GETTIMEOFDAY)
#include <sys/time.h>
#endif

namespace {

unsigned long
get_time_in_millis_impl()
{
#if defined(_WIN32)
  static LARGE_INTEGER s_frequency;
  static const BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
  if (s_use_qpc) {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return static_cast<unsigned long>(
        now.QuadPart * 1000 / s_frequency.QuadPart);
  } else {
    return static_cast<unsigned long>(GetTickCount64());
  }
#elif defined(CPPMU_HAVE_GETTIMEOFDAY)
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (static_cast<unsigned long>(tv.tv_sec) * 1000) +
         (static_cast<unsigned long>(tv.tv_usec) / 1000);
#else
  return static_cast<unsigned long>(clock()) * 1000 /
         static_cast<unsigned long>(CLOCKS_PER_SEC);
#endif
}

const char*
get_time_string_impl()
{
  time_t the_time = time(nullptr);
  static char date_time[80];
#if defined(__STDC_LIB_EXT1__) || defined(__STDC_SECURE_LIB__)
  static struct tm lastlocaltime;
  localtime_s(&lastlocaltime, &theTime);
  struct tm* tmp = &lastlocaltime;
#else
  struct tm* tmp = localtime(&the_time);
#endif
  strftime(date_time, 80, "%Y-%m-%dT%H:%M:%S", tmp);
  return date_time;
}

} // namespace

namespace cppmu {

unsigned long (*get_time_in_millis)() = get_time_in_millis_impl;
const char* (*get_time_string)() = get_time_string_impl;

} // namespace cppmu
