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