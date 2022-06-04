#include <gtest/gtest.h>
#include "memory/DRAMMemory.hpp"

TEST(DRAMMemoryTest, EmptyBufferContructingWorks) {
    DRAMMemory memory(nullptr, 0);
    EXPECT_EQ(memory.size(), 0);
}

TEST(DRAMMemoryTest, WorkingWithBufferAllocatedByMallocIsCorrect) {
    size_t bufSize = 100;
    void *buf = malloc(100);
    ASSERT_NE(buf, nullptr);

    DRAMMemory memory(buf, bufSize);
    ASSERT_EQ(memory.size(), bufSize);

    char tmpBuf[1];
    char fillingSymbol = '1';
    tmpBuf[0] = fillingSymbol;

    for (Offset i = 0; i < bufSize; ++i) {
        memory.write(i, 1, tmpBuf);
    }

    for (Offset i = 0; i < bufSize; ++i) {
        memory.read(i, 1, tmpBuf);
        EXPECT_EQ(*tmpBuf, fillingSymbol);
    }
    free(buf);
}

TEST(DRAMMemoryTest, AfterIncorrectUsageOfEmptyMemoryExceptionIsThrown) {
    size_t bufSize = 0;
    void *buf = nullptr;

    DRAMMemory memory(buf, bufSize);

    char tmpBuf[1];
    try {
        memory.read(0, 1, tmpBuf);
    } catch (std::exception &e) {
        auto &me = dynamic_cast<MemoryException &>(e);
        // std::string constructor is called in order to not compare these values as pointers but as strings
        ASSERT_EQ(std::string(me.what()), std::string("error while working with DRAM memory, attempt to access memory that is out of bounds"));
    }
}