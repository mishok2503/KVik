#ifndef KVIK_HOT_FILES_H
#define KVIK_HOT_FILES_H

#include <optional>

#include "KVik/memory/Memory.h"
#include "KVik/KeyValue.h"

struct HotFiles {
    virtual Offset add(KeyValue const &keyValue) = 0;
    virtual void remove(Offset offset) = 0;
    virtual std::optional<Value> get(Offset) = 0;
};

#endif //KVIK_HOT_FILES_H