#include <gtest/gtest.h>
#include "KVik/memory/DirectoryFixedFileMemoryAllocator.h"
#include "KVik/memory/DirectoryExtandableFileMemoryAllocator.h"

TEST(DirectoryFixedFileMemoryAllocatorTest, EmptyFileAllocationAndDeallocationWorks) {
    DirectoryFixedFileMemoryAllocator allocator(".", "test-empty");
    auto firstFile = allocator.alloc(0);
    auto secondFile = allocator.alloc(0);
    EXPECT_EQ(firstFile->size(), 0);
    EXPECT_EQ(secondFile->size(), 0);
    allocator.dealloc(std::move(firstFile));
    allocator.dealloc(std::move(secondFile));
}

TEST(DirectoryFixedFileMemoryAllocatorTest, NonemptyFileAllocationAndDeallocationWorks) {
    DirectoryFixedFileMemoryAllocator allocator(".", "test-nonempty");
    Size firstFileSize = 1'000;
    Size secondFileSize = 1'000'000;
    auto firstFile = allocator.alloc(firstFileSize);
    auto secondFile = allocator.alloc(secondFileSize);
    EXPECT_EQ(firstFile->size(), firstFileSize);
    EXPECT_EQ(secondFile->size(), secondFileSize);
    allocator.dealloc(std::move(firstFile));
    allocator.dealloc(std::move(secondFile));
}

TEST(DirectoryFixedFileMemoryAllocatorTest, WhenFileIsAllocatedItIsZeroed) {
    DirectoryFixedFileMemoryAllocator allocator(".", "test-is-zeroed");
    Size fileSize = 1'000;
    auto memory = allocator.alloc(fileSize);
    EXPECT_EQ(memory->size(), fileSize);

    char byte;
    for (Offset i = 0; i < fileSize; ++i) {
        memory->read(i, 1, &byte);
        EXPECT_EQ(byte, 0);
    }
    allocator.dealloc(std::move(memory));
}

TEST(DirectoryExtandableFileMemoryAllocatorTest, WhenFileIsAllocatedItIsZeroed) {
    DirectoryExtandableFileMemoryAllocator allocator(".", "test-is-extandable-zeroed");
    Size fileSize = 1'000;
    auto memory = allocator.alloc(fileSize);
    EXPECT_EQ(memory->size(), fileSize);

    char byte;
    for (Offset i = 0; i < fileSize; ++i) {
        memory->read(i, 1, &byte);
        EXPECT_EQ(byte, 0);
    }

    byte = 'k';
    memory->write(fileSize, 1, &byte);
    EXPECT_EQ(memory->size(), fileSize + 1);
    memory->read(fileSize, 1, &byte);
    EXPECT_EQ(byte, 'k');

    allocator.dealloc(std::move(memory));
}