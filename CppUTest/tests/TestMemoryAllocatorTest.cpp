/*
 * Copyright (c) 2007, Michael Feathers, James Grenning and Bas Vodde
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE EARLIER MENTIONED AUTHORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestMemoryAllocator.h"

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
    cpputest::TestMemoryAllocator* originalAllocator = cpputest::getCurrentNewAllocator();

    cpputest::setCurrentNewAllocatorToDefault();
    POINTERS_EQUAL(cpputest::defaultNewAllocator(), cpputest::getCurrentNewAllocator());

    setCurrentNewAllocator(originalAllocator);
}

TEST(TestMemoryAllocatorTest, SetCurrentNewArrayAllocator)
{
    allocator = new cpputest::TestMemoryAllocator("new array allocator for test");
    setCurrentNewArrayAllocator(allocator);
    POINTERS_EQUAL(allocator, cpputest::getCurrentNewArrayAllocator());
    cpputest::setCurrentNewArrayAllocatorToDefault();
    POINTERS_EQUAL(cpputest::defaultNewArrayAllocator(), cpputest::getCurrentNewArrayAllocator());
}

TEST(TestMemoryAllocatorTest, SetCurrentMallocAllocator)
{
    allocator = new cpputest::TestMemoryAllocator("malloc_allocator");
    setCurrentMallocAllocator(allocator);
    POINTERS_EQUAL(allocator, cpputest::getCurrentMallocAllocator());
    cpputest::setCurrentMallocAllocatorToDefault();
    POINTERS_EQUAL(cpputest::defaultMallocAllocator(), cpputest::getCurrentMallocAllocator());
}

TEST(TestMemoryAllocatorTest, MemoryAllocation)
{
    allocator = new cpputest::TestMemoryAllocator();
    allocator->free_memory(allocator->alloc_memory(100, "file", 1), 100, "file", 1);
}

TEST(TestMemoryAllocatorTest, MallocNames)
{
    STRCMP_EQUAL("Standard Malloc Allocator", cpputest::defaultMallocAllocator()->name());
    STRCMP_EQUAL("malloc", cpputest::defaultMallocAllocator()->alloc_name());
    STRCMP_EQUAL("free", cpputest::defaultMallocAllocator()->free_name());
}

TEST(TestMemoryAllocatorTest, NewNames)
{
    STRCMP_EQUAL("Standard New Allocator", cpputest::defaultNewAllocator()->name());
    STRCMP_EQUAL("new", cpputest::defaultNewAllocator()->alloc_name());
    STRCMP_EQUAL("delete", cpputest::defaultNewAllocator()->free_name());
}

TEST(TestMemoryAllocatorTest, NewArrayNames)
{
    STRCMP_EQUAL("Standard New [] Allocator", cpputest::defaultNewArrayAllocator()->name());
    STRCMP_EQUAL("new []", cpputest::defaultNewArrayAllocator()->alloc_name());
    STRCMP_EQUAL("delete []", cpputest::defaultNewArrayAllocator()->free_name());
}
