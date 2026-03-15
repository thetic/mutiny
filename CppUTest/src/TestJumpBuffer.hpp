#ifndef INCLUDED_TESTJUMPBUFFER_HPP
#define INCLUDED_TESTJUMPBUFFER_HPP

/* Internal interface for the setjmp/longjmp test-exception mechanism.
 * Not part of the public API. */

int
test_set_jmp(void (*function)(void*), void* data);
void
test_long_jmp();
void
test_restore_jump_buffer();

#endif /* INCLUDED_TESTJUMPBUFFER_HPP */
