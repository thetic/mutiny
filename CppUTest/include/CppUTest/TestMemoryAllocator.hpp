#ifndef D_TestMemoryAllocator_h
#define D_TestMemoryAllocator_h

#include "CppUTest/String.hpp"

namespace cpputest {

class TestMemoryAllocator;

void
setCurrentNewAllocator(TestMemoryAllocator* allocator);
TestMemoryAllocator*
getCurrentNewAllocator();
void
setCurrentNewAllocatorToDefault();
TestMemoryAllocator*
defaultNewAllocator();

void
setCurrentNewArrayAllocator(TestMemoryAllocator* allocator);
TestMemoryAllocator*
getCurrentNewArrayAllocator();
void
setCurrentNewArrayAllocatorToDefault();
TestMemoryAllocator*
defaultNewArrayAllocator();

void
setCurrentMallocAllocator(TestMemoryAllocator* allocator);
TestMemoryAllocator*
getCurrentMallocAllocator();
void
setCurrentMallocAllocatorToDefault();
TestMemoryAllocator*
defaultMallocAllocator();

class TestMemoryAllocator
{
public:
  TestMemoryAllocator(const char* name_str = "generic",
      const char* alloc_name_str = "alloc",
      const char* free_name_str = "free");
  virtual ~TestMemoryAllocator();

  virtual char* alloc_memory(size_t size, const char* file, size_t line);
  virtual void free_memory(char* memory,
      size_t size,
      const char* file,
      size_t line);

  virtual const char* name() const;
  virtual const char* alloc_name() const;
  virtual const char* free_name() const;

  virtual TestMemoryAllocator* actualAllocator();

protected:
  const char* name_;
  const char* alloc_name_;
  const char* free_name_;
};

} // namespace cpputest

#endif
