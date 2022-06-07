#ifndef KVIK_MEMORY_H
#define KVIK_MEMORY_H

#include <cstddef>
#include <stdexcept>

using Offset = size_t;
using Size = size_t;

struct MemoryException : std::runtime_error {

    using std::runtime_error::runtime_error;

};

struct Memory {

    virtual void write(Offset offset, Size count, void *data) = 0;

    // it was possible to return void* instead, however the problem is that someone should free this memory,
    // it either leads to inconvenient API with function like "void freeBuffer(void* buffer)" so that memory
    // will be deallocated compatibly with method that was used for its allocation
    // we decided that it is better to make buffer allocation responsibility of calling code
    virtual void read(Offset offset, Size count, void *buffer) = 0;

    virtual Size size() = 0;

    virtual ~Memory() = default;

};

#endif