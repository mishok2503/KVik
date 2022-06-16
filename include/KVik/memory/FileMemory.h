#ifndef KVIK_FILE_MEMORY_H
#define KVIK_FILE_MEMORY_H

#include <cstdio>
#include <unistd.h>
#include <sys/types.h>

#include "Memory.h"
#include "DirectoryFileMemoryAllocator.h"

struct FixedFileMemory : Memory {

private:

    void changeFilePosition(Offset offset);

public:

    explicit FixedFileMemory(FILE *file, Size size, std::string filename);

    void write(Offset offset, Size count, void *data) override;

    void write(Offset offset, Size count, void const *data) override;

    void read(Offset offset, Size count, void *data) override;

    Size size() override;

protected:

    FILE *_file;

    Size _size;

    std::string _filename;

    template<typename T>
    friend
    struct DirectoryFileMemoryAllocator;

};

struct ExtendableFileMemory : FixedFileMemory {

    explicit ExtendableFileMemory(FILE *file, Size size, std::string filename);

    void write(Offset offset, Size count, void *data) override;

    void write(Offset offset, Size count, void const *data) override;

};

#endif