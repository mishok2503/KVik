#include <gtest/gtest.h>

#include "KVik/structures/SsdData.h"

#include "KVik/memory/DirectoryFixedFileMemoryAllocator.h"

TEST(SsdDataTest, EmptyWorks) {
    auto allocator = std::make_unique<DirectoryFixedFileMemoryAllocator>(".", "ssd-data");
    SsdData data(std::move(allocator));
    EXPECT_EQ(data.size(), 0);
}