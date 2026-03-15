#include "CppUTest/time.hpp"

#include <time.h>

#if defined(_WIN32)
#include <windows.h>
#elif defined(CPPUTEST_HAVE_GETTIMEOFDAY)
#include <sys/time.h>
#endif

namespace {

unsigned long
GetTimeInMillisImpl()
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
#elif defined(CPPUTEST_HAVE_GETTIMEOFDAY)
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
GetTimeStringImpl()
{
  time_t theTime = time(nullptr);
  static char dateTime[80];
#if defined(__STDC_LIB_EXT1__) || defined(__STDC_SECURE_LIB__)
  static struct tm lastlocaltime;
  localtime_s(&lastlocaltime, &theTime);
  struct tm* tmp = &lastlocaltime;
#else
  struct tm* tmp = localtime(&theTime);
#endif
  strftime(dateTime, 80, "%Y-%m-%dT%H:%M:%S", tmp);
  return dateTime;
}

} // namespace

namespace cpputest {

unsigned long (*GetTimeInMillis)() = GetTimeInMillisImpl;
const char* (*GetTimeString)() = GetTimeStringImpl;

} // namespace cpputest
