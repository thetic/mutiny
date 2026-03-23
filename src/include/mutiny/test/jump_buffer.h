#ifndef INCLUDED_MUTINY_JUMP_BUFFER_H
#define INCLUDED_MUTINY_JUMP_BUFFER_H

#include <stdbool.h>

/* Internal interface for the setjmp/longjmp test-exception mechanism.
 * Not part of the public API. */

#ifdef __cplusplus
extern "C"
{
#endif

  bool mutiny_set_jmp(void (*volatile function)(void*), void* volatile data);

#ifdef __cplusplus
  [[noreturn]]
#elif defined(_MSC_VER)
__declspec(noreturn)
#else
_Noreturn
#endif
  void mutiny_long_jmp(void);

  void mutiny_restore_jump_buffer(void);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_MUTINY_JUMP_BUFFER_H */
