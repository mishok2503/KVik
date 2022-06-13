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

TEST(MallocDRAMMemoryAllocatorTest, ReallocatingEmptyToEmptyWorks) {
    MallocDRAMMemoryAllocator allocator;
    std::unique_ptr<Memory> memory = allocator.alloc(0);
    ASSERT_EQ(memory->size(), 0);
    memory = allocator.alloc(std::move(memory), 0);
    ASSERT_EQ(memory->size(), 0);
    allocator.dealloc(std::move(memory));
}

TEST(MallocDRAMMemoryAllocatorTest, ReallocatingEmptyToNonemptyWorks) {
    Size reallocatedSize = 1'000;
    MallocDRAMMemoryAllocator allocator;
    std::unique_ptr<Memory> memory = allocator.alloc(0);
    ASSERT_EQ(memory->size(), 0);
    memory = allocator.alloc(std::move(memory), reallocatedSize);
    ASSERT_EQ(memory->size(), reallocatedSize);
    allocator.dealloc(std::move(memory));
}

TEST(MallocDRAMMemoryAllocatorTest, ReallocatingNonemptyToEmptyWorks) {
    Size allocatedSize = 1'000;
    MallocDRAMMemoryAllocator allocator;
    std::unique_ptr<Memory> memory = allocator.alloc(allocatedSize);
    ASSERT_EQ(memory->size(), allocatedSize);
    memory = allocator.alloc(std::move(memory), 0);
    ASSERT_EQ(memory->size(), 0);
    allocator.dealloc(std::move(memory));
}

TEST(MallocDRAMMemoryAllocatorTest, ReallocatingNonemptyToNonemptyToBiggerWorks) {
    Size allocatedSize = 1'000;
    Size reallocatedSize = 2'000;
    MallocDRAMMemoryAllocator allocator;
    std::unique_ptr<Memory> memory = allocator.alloc(allocatedSize);
    ASSERT_EQ(memory->size(), allocatedSize);
    memory = allocator.alloc(std::move(memory), reallocatedSize);
    ASSERT_EQ(memory->size(), reallocatedSize);
    allocator.dealloc(std::move(memory));
}

TEST(MallocDRAMMemoryAllocatorTest, ReallocatingNonemptyToNonemptyToSmallerWorks) {
    Size allocatedSize = 2'000;
    Size reallocatedSize = 1'000;
    MallocDRAMMemoryAllocator allocator;
    std::unique_ptr<Memory> memory = allocator.alloc(allocatedSize);
    ASSERT_EQ(memory->size(), allocatedSize);
    memory = allocator.alloc(std::move(memory), reallocatedSize);
    ASSERT_EQ(memory->size(), reallocatedSize);
    allocator.dealloc(std::move(memory));
}