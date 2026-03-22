#include "CppMu/jump_buffer.h"

#include <setjmp.h>

static jmp_buf cppmu_jmp_buf[10];
static int cppmu_jmp_buf_depth = 0;

bool cppmu_set_jmp(void (*volatile function)(void*), void* volatile data)
{
  if (0 == setjmp(cppmu_jmp_buf[cppmu_jmp_buf_depth])) {
    cppmu_jmp_buf_depth++;
    function(data);
    cppmu_jmp_buf_depth--;
    return true;
  }
  return false;
}

void cppmu_long_jmp(void)
{
  cppmu_jmp_buf_depth--;
  longjmp(cppmu_jmp_buf[cppmu_jmp_buf_depth], 1);
}

void cppmu_restore_jump_buffer(void)
{
  cppmu_jmp_buf_depth--;
}
