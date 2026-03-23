#ifndef INCLUDED_MUTINY_TEST_TIME_HPP
#define INCLUDED_MUTINY_TEST_TIME_HPP

namespace mu {
namespace tiny {
namespace test {

extern unsigned long (*get_time_in_millis)();
extern const char* (*get_time_string)();

} // namespace test
} // namespace tiny
} // namespace mu

#endif /* INCLUDED_MUTINY_TEST_TIME_HPP */
