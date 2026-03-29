#ifndef INCLUDED_MUTINY_TEST_JUMP_BUFFER_HPP
#define INCLUDED_MUTINY_TEST_JUMP_BUFFER_HPP

/* Internal interface for the setjmp/longjmp test-exception mechanism.
 * Not part of the public API. */

namespace mu {
namespace tiny {
namespace test {

bool set_jump(void (*volatile function)(void*), void* volatile data);

[[noreturn]] void long_jump(void);

void restore_jump_buffer(void);

} // namespace test
} // namespace tiny
} // namespace mu

#endif /* INCLUDED_MUTINY_TEST_JUMP_BUFFER_HPP */
