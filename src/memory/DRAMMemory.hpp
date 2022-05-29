#ifndef KVIK_DRAM_MEMORY_H
#define KVIK_DRAM_MEMORY_H

#include <memory>
#include <cstring>

#include "Memory.hpp"

struct DRAMMemory : Memory {
private:
    void checkRange(Offset offset, Size count);

public:

    DRAMMemory(void *buf, Size bufSize);

    virtual void write(Offset offset, Size count, void *data) override;

    virtual void read(Offset offset, Size count, void *data) override;

    virtual Size size() noexcept override;

private:
    void *_buf;
    Size _bufSize;
};

#endif