#include "CppMu/jump_buffer.h"

#include <setjmp.h>

static jmp_buf test_exit_jmp_buf[10];
static int jmp_buf_index = 0;

bool cppmu_set_jmp(void (*function)(void*), void* data)
{
  if (0 == setjmp(test_exit_jmp_buf[jmp_buf_index])) {
    jmp_buf_index++;
    function(data);
    jmp_buf_index--;
    return true;
  }
  return false;
}

void cppmu_long_jmp(void)
{
  jmp_buf_index--;
  longjmp(test_exit_jmp_buf[jmp_buf_index], 1);
}

void cppmu_restore_jump_buffer(void)
{
  jmp_buf_index--;
}
