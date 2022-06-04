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

    explicit FixedFileMemory(FILE *file);

    void write(Offset offset, Size count, void *data) override;

    void read(Offset offset, Size count, void *data) override;

    Size size() override;

protected:

    FILE *_file;

};

struct ExtendableFileMemory : FixedFileMemory {

    explicit ExtendableFileMemory(FILE *file);

    void write(Offset offset, Size count, void *data) override;

};

#endif