#ifndef KVIK_FILE_MEMORY_H
#define KVIK_FILE_MEMORY_H

#include <cstdio>
#include <unistd.h>
#include <sys/types.h>

#include "Memory.hpp"

struct FixedFileMemory : Memory {
private:
    void changeFilePosition(Offset offset);

public:
    FixedFileMemory(FILE *file);

    virtual void write(Offset offset, Size count, void *data) override;

    virtual void read(Offset offset, Size count, void *data) override;

    virtual Size size() override;

protected:
    FILE *_file;
};

struct ExtendableFileMemory : FixedFileMemory {
    ExtendableFileMemory(FILE *file);

    virtual void write(Offset offset, Size count, void *data) override;
};

#endif