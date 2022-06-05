#include <gtest/gtest.h>
#include "memory/FileMemory.hpp"


TEST(FileMemoryTest, EmptyFileWorks) {
    char filename[] = "./EmptyFileWorks";

    FILE *f = fopen(filename, "w+");
    ASSERT_NE(f, nullptr);

    FixedFileMemory memory(f, 0, filename);
    ASSERT_EQ(memory.size(), 0);

    fclose(f);
    ASSERT_EQ(remove(filename), 0);
}

TEST(FileMemoryTest, BasicFileFilledWithOneValueWorks) {
    char filename[] = "./BasicFileFilledWithOneValueWorks";
    size_t size = 1000;

    FILE *f = fopen(filename, "w+");
    ASSERT_NE(f, nullptr);
    ASSERT_EQ(ftruncate(fileno(f), size), 0);

    FixedFileMemory memory(f, size, filename);
    ASSERT_EQ(memory.size(), size);

    char tmpBuf[1];
    char fillingSymbol = '1';
    tmpBuf[0] = fillingSymbol;

    for (Offset i = 0; i < size; ++i) {
        memory.write(i, 1, tmpBuf);
    }

    for (Offset i = 0; i < size; ++i) {
        memory.read(i, 1, tmpBuf);
        ASSERT_EQ(*tmpBuf, fillingSymbol);
    }

    fclose(f);
    ASSERT_EQ(remove(filename), 0);
}

TEST(FileMemoryTest, DynamicallyExtendingFileWithOneValueWorks) {
    char filename[] = "./DynamicallyExtendingFileWithOneValueWorks";
    size_t size = 1000;

    FILE *file = fopen(filename, "w+");
    ASSERT_NE(file, nullptr);

    ExtendableFileMemory memory(file, 0, filename);
    ASSERT_EQ(memory.size(), 0);

    char tmpBuf[1];
    char fillingSymbol = '1';
    tmpBuf[0] = fillingSymbol;

    for (Offset i = 0; i < size; ++i) {
        memory.write(i, 1, tmpBuf);
        ASSERT_EQ(memory.size(), i + 1);
    }

    for (Offset i = 0; i < size; ++i) {
        memory.read(i, 1, tmpBuf);
        ASSERT_EQ(*tmpBuf, fillingSymbol);
    }

    ASSERT_EQ(memory.size(), size);

    fclose(file);
    ASSERT_EQ(remove(filename), 0);
}

TEST(FileMemoryTest, AfterIncorrectUsageOfFixedEmptyFileExceptionIsThrown) {
    char filename[] = "./AfterIncorrectUsageOfFixedEmptyFileExceptionIsThrown";

    FILE *f = fopen(filename, "w+");
    ASSERT_NE(f, nullptr);

    FixedFileMemory memory(f, 0, filename);
    ASSERT_EQ(memory.size(), 0);

    char tmpBuf[1];

    try {
        memory.read(0, 1, tmpBuf);
    } catch (std::exception &e) {
        auto &me = dynamic_cast<MemoryException &>(e);
        // std::string constructor is called in order to not compare these values as pointers but as strings
        EXPECT_EQ(std::string(me.what()), std::string("error during std::fread"));
    }

    fclose(f);
    ASSERT_EQ(remove(filename), 0);
}

TEST(FileMemoryTest, AfterIncorrectUsageOfFixedNonemptyFileExceptionIsThrown) {
    char filename[] = "./AfterIncorrectUsageOfFixedNonemptyFileExceptionIsThrown";
    size_t size = 1000;

    FILE *f = fopen(filename, "w+");
    ASSERT_NE(f, nullptr);

    FixedFileMemory memory(f, size, filename);

    char tmpBuf[] = "1";
    for (Offset i = 0; i < size; ++i) {
        memory.write(i, 1, tmpBuf);
    }
    ASSERT_EQ(memory.size(), size);

    try {
        memory.read(size, 1, tmpBuf);
    } catch (std::exception &e) {
        auto &me = dynamic_cast<MemoryException &>(e);
        // std::string constructor is called in order to not compare these values as pointers but as strings
        EXPECT_EQ(std::string(me.what()), std::string("error during std::fread"));
    }

    ASSERT_EQ(remove(filename), 0);
    fclose(f);
}

TEST(FileMemoryTest, NonSequentialWriteOperationToExtendableFileMemoryWorks) {
    char filename[] = "./AfterIncorrectUsageOfFixedNonemptyFileExceptionIsThrown";
    size_t size = 1000;

    FILE *f = fopen(filename, "w+");
    ASSERT_NE(f, nullptr);

    ExtendableFileMemory memory(f, size, filename);

    char tmpBuf[] = "1";
    for (Offset i = 0; i < size; ++i) {
        memory.write(i, 1, tmpBuf);
    }
    ASSERT_EQ(memory.size(), size);

    memory.write(size * 2, 1, tmpBuf);

    ASSERT_EQ(memory.size(), size * 2 + 1);

    ASSERT_EQ(remove(filename), 0);
    fclose(f);
}