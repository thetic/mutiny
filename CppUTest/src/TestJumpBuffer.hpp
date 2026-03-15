#ifndef INCLUDED_TESTJUMPBUFFER_HPP
#define INCLUDED_TESTJUMPBUFFER_HPP

/* Internal interface for the setjmp/longjmp test-exception mechanism.
 * Not part of the public API. */

int
TestSetJmp(void (*function)(void*), void* data);
void
TestLongJmp();
void
TestRestoreJumpBuffer();

#endif /* INCLUDED_TESTJUMPBUFFER_HPP */
