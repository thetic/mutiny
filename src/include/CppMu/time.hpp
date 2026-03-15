#ifndef INCLUDED_CPPMU_TIME_HPP
#define INCLUDED_CPPMU_TIME_HPP

namespace cppmu {

extern unsigned long (*get_time_in_millis)();
extern const char* (*get_time_string)();

} // namespace cppmu

#endif /* INCLUDED_CPPMU_TIME_HPP */
