#include "mutiny/test/jump_buffer.h"

#include <setjmp.h>

static jmp_buf mutiny_jmp_buf[10];
static int mutiny_jmp_buf_depth = 0;

bool mutiny_set_jmp(void (*volatile function)(void*), void* volatile data)
{
  if (0 == setjmp(mutiny_jmp_buf[mutiny_jmp_buf_depth])) {
    mutiny_jmp_buf_depth++;
    function(data);
    mutiny_jmp_buf_depth--;
    return true;
  }
  return false;
}

void mutiny_long_jmp(void)
{
  mutiny_jmp_buf_depth--;
  longjmp(mutiny_jmp_buf[mutiny_jmp_buf_depth], 1);
}

void mutiny_restore_jump_buffer(void)
{
  mutiny_jmp_buf_depth--;
}
