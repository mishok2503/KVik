#include <gtest/gtest.h>
#include <memory>

#include "KVik/memory/MallocDRAMMemoryAllocator.h"


TEST(MallocDRAMMemoryAllocatorTest, AllocatingEmptyMemoryWorks) {
    MallocDRAMMemoryAllocator allocator;
    std::unique_ptr<Memory> memory = allocator.alloc(0);
    ASSERT_EQ(memory->size(), 0);
    allocator.dealloc(std::move(memory));
}

TEST(MallocDRAMMemoryAllocatorTest, AllocatingNonemptyMemoryWorks) {
    Size allocSize = 1'000;
    MallocDRAMMemoryAllocator allocator;
    std::unique_ptr<Memory> memory = allocator.alloc(allocSize);
    ASSERT_EQ(memory->size(), allocSize);
    allocator.dealloc(std::move(memory));
}

TEST(MallocDRAMMemoryAllocatorTest, AllocatingHugeMemory1Works) {
    Size allocSize = 1'000'000;
    MallocDRAMMemoryAllocator allocator;
    std::unique_ptr<Memory> memory = allocator.alloc(allocSize);
    ASSERT_EQ(memory->size(), allocSize);
    allocator.dealloc(std::move(memory));
}

TEST(MallocDRAMMemoryAllocatorTest, AllocatingHugeMemory2Works) {
    Size allocSize = 10'000'000;
    MallocDRAMMemoryAllocator allocator;
    std::unique_ptr<Memory> memory = allocator.alloc(allocSize);
    ASSERT_EQ(memory->size(), allocSize);
    allocator.dealloc(std::move(memory));
}

TEST(MallocDRAMMemoryAllocatorTest, AllocatingHugeMemory3Works) {
    Size allocSize = 100'000'000;
    MallocDRAMMemoryAllocator allocator;
    std::unique_ptr<Memory> memory = allocator.alloc(allocSize);
    ASSERT_EQ(memory->size(), allocSize);
    allocator.dealloc(std::move(memory));
}