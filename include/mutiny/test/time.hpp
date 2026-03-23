#ifndef INCLUDED_MUTINY_TIME_HPP
#define INCLUDED_MUTINY_TIME_HPP

namespace mu {
namespace tiny {
namespace test {

extern unsigned long (*get_time_in_millis)();
extern const char* (*get_time_string)();

}
}
} // namespace mu::tiny::test

#endif /* INCLUDED_MUTINY_TIME_HPP */
