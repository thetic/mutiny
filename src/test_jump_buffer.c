#include "CppMu/test_jump_buffer.h"

#include <setjmp.h>

static jmp_buf test_exit_jmp_buf[10];
static int jmp_buf_index = 0;

int test_set_jmp(void (*function)(void*), void* data)
{
  if (0 == setjmp(test_exit_jmp_buf[jmp_buf_index])) {
    jmp_buf_index++;
    function(data);
    jmp_buf_index--;
    return 1;
  }
  return 0;
}

void test_long_jmp(void)
{
  jmp_buf_index--;
  longjmp(test_exit_jmp_buf[jmp_buf_index], 1);
}

void test_restore_jump_buffer(void)
{
  jmp_buf_index--;
}
