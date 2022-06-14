#include <gtest/gtest.h>

#include <memory>
#include <unistd.h>

#include "KVik/memory/MmapDRAMMemoryAllocator.h"
#include "KVik/memory/Memory.h"

TEST(MmapDRAMMemoryAllocatorTest, EmptyMemoryIsAllocatedCorrectly) {
    MmapDRAMMemoryAllocator allocator;
    std::unique_ptr<Memory> memory = allocator.alloc(0);
    ASSERT_EQ(memory->size(), 0);
    allocator.dealloc(std::move(memory));
}

TEST(MmapDRAMMemoryAllocatorTest, SmallUnalignedMemoryIsAllocatedAndDeallocatedCorrectly) {
    Size memorySize = 1000;
    MmapDRAMMemoryAllocator allocator;
    std::unique_ptr<Memory> memory = allocator.alloc(memorySize);
    ASSERT_EQ(memory->size(), memorySize);

    // memory is actually zeroed
    char byte;
    for (Offset i = 0; i < memorySize; ++i) {
        memory->read(i, 1, &byte);
        EXPECT_EQ(byte, 0);
    }

    allocator.dealloc(std::move(memory));
}

TEST(MmapDRAMMemoryAllocatorTest, SmallAlignedMemoryIsAllocatedAndDeallocatedCorrectly) {
    Size PAGE_SIZE = sysconf(_SC_PAGESIZE);
    Size PAGES_COUNT = 5;
    Size memorySize = PAGE_SIZE * PAGES_COUNT;

    MmapDRAMMemoryAllocator allocator;
    std::unique_ptr<Memory> memory = allocator.alloc(memorySize);
    ASSERT_EQ(memory->size(), memorySize);

    // memory is actually zeroed
    char byte;
    for (Offset i = 0; i < memorySize; ++i) {
        memory->read(i, 1, &byte);
        EXPECT_EQ(byte, 0);
    }

    allocator.dealloc(std::move(memory));
}

TEST(MmapDRAMMemoryAllocatorTest, ReallocatingEmptyToEmptyWorks) {
    MmapDRAMMemoryAllocator allocator;
    std::unique_ptr<Memory> memory = allocator.alloc(0);
    ASSERT_EQ(memory->size(), 0);
    memory = allocator.alloc(std::move(memory), 0);
    ASSERT_EQ(memory->size(), 0);
    allocator.dealloc(std::move(memory));
}

TEST(MmapDRAMMemoryAllocatorTest, ReallocatingEmptyToNonemptyWorks) {
    Size reallocatedSize = 1'000;
    MmapDRAMMemoryAllocator allocator;
    std::unique_ptr<Memory> memory = allocator.alloc(0);
    ASSERT_EQ(memory->size(), 0);
    memory = allocator.alloc(std::move(memory), reallocatedSize);
    ASSERT_EQ(memory->size(), reallocatedSize);

    char byte;
    for (Offset i = 0; i < reallocatedSize; ++i) {
        memory->read(i, 1, &byte);
        EXPECT_EQ(byte, 0);
    }

    allocator.dealloc(std::move(memory));
}

TEST(MmapDRAMMemoryAllocatorTest, ReallocatingNonemptyToEmptyWorks) {
    Size allocatedSize = 1'000;
    MmapDRAMMemoryAllocator allocator;
    std::unique_ptr<Memory> memory = allocator.alloc(allocatedSize);
    ASSERT_EQ(memory->size(), allocatedSize);

    char byte;
    for (Offset i = 0; i < allocatedSize; ++i) {
        memory->read(i, 1, &byte);
        EXPECT_EQ(byte, 0);
    }

    memory = allocator.alloc(std::move(memory), 0);
    ASSERT_EQ(memory->size(), 0);
    allocator.dealloc(std::move(memory));
}

TEST(MmapDRAMMemoryAllocatorTest, ReallocatingNonemptyToNonemptyToBiggerWorks) {
    Size allocatedSize = 1'000;
    Size reallocatedSize = 2'000;
    MmapDRAMMemoryAllocator allocator;
    std::unique_ptr<Memory> memory = allocator.alloc(allocatedSize);
    ASSERT_EQ(memory->size(), allocatedSize);

    char byte;

    for (Offset i = 0; i < allocatedSize; ++i) {
        memory->read(i, 1, &byte);
        EXPECT_EQ(byte, 0);
    }

    memory = allocator.alloc(std::move(memory), reallocatedSize);
    ASSERT_EQ(memory->size(), reallocatedSize);

    for (Offset i = 0; i < reallocatedSize; ++i) {
        memory->read(i, 1, &byte);
        EXPECT_EQ(byte, 0);
    }

    allocator.dealloc(std::move(memory));
}

TEST(MmapDRAMMemoryAllocatorTest, ReallocatingNonemptyToNonemptyToSmallerWorks) {
    Size allocatedSize = 2'000;
    Size reallocatedSize = 1'000;
    MmapDRAMMemoryAllocator allocator;
    std::unique_ptr<Memory> memory = allocator.alloc(allocatedSize);
    ASSERT_EQ(memory->size(), allocatedSize);

    char byte;

    for (Offset i = 0; i < allocatedSize; ++i) {
        memory->read(i, 1, &byte);
        EXPECT_EQ(byte, 0);
    }

    memory = allocator.alloc(std::move(memory), reallocatedSize);
    ASSERT_EQ(memory->size(), reallocatedSize);

    for (Offset i = 0; i < reallocatedSize; ++i) {
        memory->read(i, 1, &byte);
        EXPECT_EQ(byte, 0);
    }

    allocator.dealloc(std::move(memory));
}