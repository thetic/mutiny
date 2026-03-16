#ifndef INCLUDED_CPPMU_TEST_JUMP_BUFFER_H
#define INCLUDED_CPPMU_TEST_JUMP_BUFFER_H

/* Internal interface for the setjmp/longjmp test-exception mechanism.
 * Not part of the public API. */

#ifdef __cplusplus
extern "C"
{
#endif

  int test_set_jmp(void (*function)(void*), void* data);
  void test_long_jmp(void);
  void test_restore_jump_buffer(void);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_CPPMU_TEST_JUMP_BUFFER_H */
