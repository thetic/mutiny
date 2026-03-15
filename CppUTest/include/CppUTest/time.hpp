#ifndef INCLUDED_CPPUTEST_TIME_HPP
#define INCLUDED_CPPUTEST_TIME_HPP

namespace cpputest {

extern unsigned long (*GetTimeInMillis)();
extern const char* (*GetTimeString)();

} // namespace cpputest

#endif /* INCLUDED_CPPUTEST_TIME_HPP */
