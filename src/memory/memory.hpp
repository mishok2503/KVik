#ifndef KVIK_MEMORY_H
#define KVIK_MEMORY_H

#include <stddef.h>

using Offset = size_t;
using Size = size_t;

template<typename RecordType>
struct Memory {
    virtual void write(Offset offset, Size count, RecordType *data) = 0;

    // it was possible to return RecordType[] instead, however the problem is that someone should free this memory,
    // it either leads to inconvenient API with function like "void freeBuffer(RecordType[] buffer)" so that memory
    // will be deallocated compatibly with method that was used for its allocation
    // we decided that it is better to make buffer allocation responsibility of calling code
    virtual void read(Offset offset, Size count, RecordType *buffer) = 0;

    virtual void size() = 0;
};

#endif