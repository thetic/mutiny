#ifndef INCLUDED_CPPUTEST_TIME_HPP
#define INCLUDED_CPPUTEST_TIME_HPP

namespace cpputest {

extern unsigned long (*get_time_in_millis)();
extern const char* (*get_time_string)();

} // namespace cpputest

#endif /* INCLUDED_CPPUTEST_TIME_HPP */
