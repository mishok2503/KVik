#ifndef KVIK_FILE_MEMORY_H
#define KVIK_FILE_MEMORY_H

#include <cstdio>
#include <unistd.h>
#include <sys/types.h>

#include "Memory.hpp"

struct FixedFileMemory : Memory {
private:
    void changeFilePosition(Offset offset) {
        if (std::fseek(_file, offset, SEEK_SET) < 0) {
            throw MemoryException(std::string("error during fseek, errno is ") + std::to_string(errno));
        }
    }

public:
    FixedFileMemory(FILE *file) : _file(file) {}

    virtual void write(Offset offset, Size count, void *data) override {
        changeFilePosition(offset);
        if (std::fwrite(data, 1, count, _file) != count) {
            throw MemoryException("error during std::fwrite");
        }
    }

    virtual void read(Offset offset, Size count, void *data) override {
        changeFilePosition(offset);
        if (std::fread(data, 1, count, _file)) {
            throw MemoryException("error during std::fread");
        }
    }

    virtual Size size() override {
        std::fseek(_file, 0L, SEEK_END);
        return std::ftell(_file);
    }

protected:
    FILE *_file;
};

struct ExtendableFileMemory : FixedFileMemory {
    ExtendableFileMemory(FILE *file) : FixedFileMemory(file) {}
    virtual void write(Offset offset, Size count, void *data) {
        if (offset + count > FixedFileMemory::size()) {
            if (ftruncate(fileno(_file), offset + count) < 0) {
                throw MemoryException("error during ftruncate");
            }
        }
        FixedFileMemory::write(offset, count, data);
    }
};

#endif