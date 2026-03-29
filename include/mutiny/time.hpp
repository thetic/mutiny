#ifndef INCLUDED_MUTINY_TEST_TIME_HPP
#define INCLUDED_MUTINY_TEST_TIME_HPP

#include "mutiny/export.h"

namespace mu {
namespace tiny {

MUTINY_EXPORT extern unsigned long (*get_time_in_millis)();
MUTINY_EXPORT extern const char* (*get_time_string)();

} // namespace tiny
} // namespace mu

#endif /* INCLUDED_MUTINY_TEST_TIME_HPP */
