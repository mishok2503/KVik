#ifndef KVIK_KVIK_H
#define KVIK_KVIK_H

#include <future>

#include "KVik/KeyValue.h"

class KVikBuilder;

class KVik {
public:
    virtual std::future<void> remove(Key) = 0;
    virtual std::future<void> add(Key, Value) = 0;
    virtual std::future<Value> get(Key) = 0;
    virtual std::future<void> clear() = 0;
};


#endif //KVIK_KVIK_H
