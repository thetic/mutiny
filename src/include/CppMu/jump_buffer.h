#ifndef INCLUDED_CPPMU_JUMP_BUFFER_H
#define INCLUDED_CPPMU_JUMP_BUFFER_H

#include <stdbool.h>

/* Internal interface for the setjmp/longjmp test-exception mechanism.
 * Not part of the public API. */

#ifdef __cplusplus
extern "C"
{
#endif

  bool cppmu_set_jmp(void (*volatile function)(void*), void* volatile data);

#ifdef __cplusplus
  [[noreturn]]
#elif defined(_MSC_VER)
__declspec(noreturn)
#else
_Noreturn
#endif
  void cppmu_long_jmp(void);

  void cppmu_restore_jump_buffer(void);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_CPPMU_JUMP_BUFFER_H */
