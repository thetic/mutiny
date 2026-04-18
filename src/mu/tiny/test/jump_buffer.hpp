#ifndef INCLUDED_MU_TINY_TEST_JUMP_BUFFER_HPP
#define INCLUDED_MU_TINY_TEST_JUMP_BUFFER_HPP

/* Internal interface for the setjmp/longjmp test-exception mechanism.
 * Not part of the public API. */

namespace mu {
namespace tiny {
namespace test {

bool set_jump(void (*function)(void*), void* data);

[[noreturn]] void long_jump(void);

void restore_jump_buffer(void);

} // namespace test
} // namespace tiny
} // namespace mu

#endif /* INCLUDED_MU_TINY_TEST_JUMP_BUFFER_HPP */
