/**
 * @file
 * @brief Replaceable time-source function pointers.
 *
 * The framework queries elapsed time through these two function pointers so
 * that tests can substitute a deterministic time source by reassigning them.
 */

#ifndef INCLUDED_MUTINY_TEST_TIME_HPP
#define INCLUDED_MUTINY_TEST_TIME_HPP

#include "mutiny/export.h"

#include <stdint.h>

namespace mu {
namespace tiny {

/**
 * @brief Returns the current time in milliseconds.
 *
 * Defaults to a platform clock. Replace with a custom function to make
 * timing deterministic in tests.
 */
MUTINY_EXPORT extern uint_least64_t (*get_time_in_millis)();

/**
 * @brief Returns a human-readable current time string.
 *
 * Used by the output layer when printing timestamps. Replace with a custom
 * function to control the format or source of the timestamp.
 */
MUTINY_EXPORT extern const char* (*get_time_string)();

} // namespace tiny
} // namespace mu

#endif /* INCLUDED_MUTINY_TEST_TIME_HPP */
