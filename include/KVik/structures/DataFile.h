#ifndef KVIK_DATAFILE_H
#define KVIK_DATAFILE_H

#include "KVik/memory/Memory.h"

#include <vector>
#include <memory>

#include "KVik/KeyValue.h"

class DataFile {
public:
    virtual void remove(Offset offset) = 0;
    virtual std::vector<Offset> write(std::unique_ptr<Memory> &memory, std::vector<Offset> const &offsets) = 0;
    virtual Offset write(std::unique_ptr<Memory> &memory, Offset offset) = 0;
    virtual Offset write(Value) = 0;
    virtual Value read(Offset offset) = 0;
    virtual Size size() = 0;
};

#endif //KVIK_DATAFILE_H
