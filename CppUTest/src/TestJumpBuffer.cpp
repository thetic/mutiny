#include "TestJumpBuffer.hpp"

#include <setjmp.h>

namespace {

jmp_buf test_exit_jmp_buf[10];
int jmp_buf_index = 0;

} // namespace

int
TestSetJmp(void (*function)(void*), void* data)
{
  if (0 == setjmp(test_exit_jmp_buf[jmp_buf_index])) {
    jmp_buf_index++;
    function(data);
    jmp_buf_index--;
    return 1;
  }
  return 0;
}

void
TestLongJmp()
{
  jmp_buf_index--;
  longjmp(test_exit_jmp_buf[jmp_buf_index], 1);
}

void
TestRestoreJumpBuffer()
{
  jmp_buf_index--;
}
