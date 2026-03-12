#include "CppUTest/TestMemoryAllocator.hpp"

#include "CppUTest/PlatformSpecificFunctions.h"
#include "CppUTest/TestHarness.hpp"

namespace cpputest {

static char*
checkedMalloc(size_t size)
{
  char* mem = static_cast<char*>(PlatformSpecificMalloc(size));
  if (mem == nullptr)
    FAIL("malloc returned null pointer");
  return mem;
}

static TestMemoryAllocator* currentNewAllocator = nullptr;
static TestMemoryAllocator* currentNewArrayAllocator = nullptr;
static TestMemoryAllocator* currentMallocAllocator = nullptr;

void
setCurrentNewAllocator(TestMemoryAllocator* allocator)
{
  currentNewAllocator = allocator;
}

TestMemoryAllocator*
getCurrentNewAllocator()
{
  if (currentNewAllocator == nullptr)
    setCurrentNewAllocatorToDefault();
  return currentNewAllocator;
}

void
setCurrentNewAllocatorToDefault()
{
  currentNewAllocator = defaultNewAllocator();
}

TestMemoryAllocator*
defaultNewAllocator()
{
  static TestMemoryAllocator allocator(
      "Standard New Allocator", "new", "delete");
  return &allocator;
}

void
setCurrentNewArrayAllocator(TestMemoryAllocator* allocator)
{
  currentNewArrayAllocator = allocator;
}

TestMemoryAllocator*
getCurrentNewArrayAllocator()
{
  if (currentNewArrayAllocator == nullptr)
    setCurrentNewArrayAllocatorToDefault();
  return currentNewArrayAllocator;
}

void
setCurrentNewArrayAllocatorToDefault()
{
  currentNewArrayAllocator = defaultNewArrayAllocator();
}

TestMemoryAllocator*
defaultNewArrayAllocator()
{
  static TestMemoryAllocator allocator(
      "Standard New [] Allocator", "new []", "delete []");
  return &allocator;
}

void
setCurrentMallocAllocator(TestMemoryAllocator* allocator)
{
  currentMallocAllocator = allocator;
}

TestMemoryAllocator*
getCurrentMallocAllocator()
{
  if (currentMallocAllocator == nullptr)
    setCurrentMallocAllocatorToDefault();
  return currentMallocAllocator;
}

void
setCurrentMallocAllocatorToDefault()
{
  currentMallocAllocator = defaultMallocAllocator();
}

TestMemoryAllocator*
defaultMallocAllocator()
{
  static TestMemoryAllocator allocator(
      "Standard Malloc Allocator", "malloc", "free");
  return &allocator;
}

TestMemoryAllocator::TestMemoryAllocator(const char* name_str,
    const char* alloc_name_str,
    const char* free_name_str)
  : name_(name_str)
  , alloc_name_(alloc_name_str)
  , free_name_(free_name_str)
{
}

TestMemoryAllocator::~TestMemoryAllocator() {}

char*
TestMemoryAllocator::alloc_memory(size_t size, const char*, size_t)
{
  return checkedMalloc(size);
}

void
TestMemoryAllocator::free_memory(char* memory, size_t, const char*, size_t)
{
  PlatformSpecificFree(memory);
}

const char*
TestMemoryAllocator::name() const
{
  return name_;
}

const char*
TestMemoryAllocator::alloc_name() const
{
  return alloc_name_;
}

const char*
TestMemoryAllocator::free_name() const
{
  return free_name_;
}

TestMemoryAllocator*
TestMemoryAllocator::actualAllocator()
{
  return this;
}

} // namespace cpputest
