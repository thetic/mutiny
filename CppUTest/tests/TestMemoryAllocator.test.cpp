#include "CppUTest/TestMemoryAllocator.hpp"

#include "CppUTest/TestHarness.hpp"

TEST_GROUP(TestMemoryAllocatorTest)
{
  cpputest::TestMemoryAllocator* allocator;
  cpputest::TestMemoryAllocator* originalMallocAllocator;
  cpputest::TestMemoryAllocator* originalNewAllocator;
  cpputest::TestMemoryAllocator* originalNewArrayAllocator;

  void setup() override
  {
    allocator = nullptr;
    originalMallocAllocator = cpputest::getCurrentMallocAllocator();
    originalNewAllocator = cpputest::getCurrentNewAllocator();
    originalNewArrayAllocator = cpputest::getCurrentNewArrayAllocator();
  }

  void teardown() override
  {
    setCurrentMallocAllocator(originalMallocAllocator);
    setCurrentNewAllocator(originalNewAllocator);
    setCurrentNewArrayAllocator(originalNewArrayAllocator);
    delete allocator;
  }
};

TEST(TestMemoryAllocatorTest, SetCurrentNewAllocator)
{
  allocator = new cpputest::TestMemoryAllocator("new allocator for test");
  setCurrentNewAllocator(allocator);
  POINTERS_EQUAL(allocator, cpputest::getCurrentNewAllocator());
}

TEST(TestMemoryAllocatorTest, SetCurrentNewAllocatorToDefault)
{
  cpputest::TestMemoryAllocator* originalAllocator =
      cpputest::getCurrentNewAllocator();

  cpputest::setCurrentNewAllocatorToDefault();
  POINTERS_EQUAL(
      cpputest::defaultNewAllocator(), cpputest::getCurrentNewAllocator());

  setCurrentNewAllocator(originalAllocator);
}

TEST(TestMemoryAllocatorTest, SetCurrentNewArrayAllocator)
{
  allocator = new cpputest::TestMemoryAllocator("new array allocator for test");
  setCurrentNewArrayAllocator(allocator);
  POINTERS_EQUAL(allocator, cpputest::getCurrentNewArrayAllocator());
  cpputest::setCurrentNewArrayAllocatorToDefault();
  POINTERS_EQUAL(cpputest::defaultNewArrayAllocator(),
      cpputest::getCurrentNewArrayAllocator());
}

TEST(TestMemoryAllocatorTest, SetCurrentMallocAllocator)
{
  allocator = new cpputest::TestMemoryAllocator("malloc_allocator");
  setCurrentMallocAllocator(allocator);
  POINTERS_EQUAL(allocator, cpputest::getCurrentMallocAllocator());
  cpputest::setCurrentMallocAllocatorToDefault();
  POINTERS_EQUAL(cpputest::defaultMallocAllocator(),
      cpputest::getCurrentMallocAllocator());
}

TEST(TestMemoryAllocatorTest, MemoryAllocation)
{
  allocator = new cpputest::TestMemoryAllocator();
  allocator->free_memory(
      allocator->alloc_memory(100, "file", 1), 100, "file", 1);
}

TEST(TestMemoryAllocatorTest, MallocNames)
{
  STRCMP_EQUAL(
      "Standard Malloc Allocator", cpputest::defaultMallocAllocator()->name());
  STRCMP_EQUAL("malloc", cpputest::defaultMallocAllocator()->alloc_name());
  STRCMP_EQUAL("free", cpputest::defaultMallocAllocator()->free_name());
}

TEST(TestMemoryAllocatorTest, NewNames)
{
  STRCMP_EQUAL(
      "Standard New Allocator", cpputest::defaultNewAllocator()->name());
  STRCMP_EQUAL("new", cpputest::defaultNewAllocator()->alloc_name());
  STRCMP_EQUAL("delete", cpputest::defaultNewAllocator()->free_name());
}

TEST(TestMemoryAllocatorTest, NewArrayNames)
{
  STRCMP_EQUAL("Standard New [] Allocator",
      cpputest::defaultNewArrayAllocator()->name());
  STRCMP_EQUAL("new []", cpputest::defaultNewArrayAllocator()->alloc_name());
  STRCMP_EQUAL("delete []", cpputest::defaultNewArrayAllocator()->free_name());
}
