#include "mu/tiny/test/jump_buffer.hpp"

#include <setjmp.h>
#include <stddef.h>

namespace mu {
namespace tiny {
namespace test {

namespace {
constexpr size_t max_jmp_buf_depth{ 10 };
jmp_buf mutiny_jmp_buf[max_jmp_buf_depth];
int mutiny_jmp_buf_depth = 0;
} // namespace

bool set_jump(void (*function)(void*), void* data)
{
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4611)
#endif
  if (0 == setjmp(mutiny_jmp_buf[mutiny_jmp_buf_depth])) {
#ifdef _MSC_VER
#pragma warning(pop)
#endif
    mutiny_jmp_buf_depth++;
    function(data);
    mutiny_jmp_buf_depth--;
    return true;
  }
  return false;
}

void long_jump(void)
{
  mutiny_jmp_buf_depth--;
  longjmp(mutiny_jmp_buf[mutiny_jmp_buf_depth], 1);
}

void restore_jump_buffer(void)
{
  mutiny_jmp_buf_depth--;
}

} // namespace test
} // namespace tiny
} // namespace mu
