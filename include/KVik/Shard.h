#ifndef KVIK_SHARD_H
#define KVIK_SHARD_H

#include <memory>

#include "KVik/structures/Filter.h"
#include "KVik/structures/Index.h"
#include "KVik/structures/DataFile.h"
#include "KVik/structures/Log.h"
#include "KVik/KeyValue.h"

class Shard {
private:
    template<typename T>
    using ptr = std::unique_ptr<T>;

    ptr<Filter<Key, Index>> filter;
    ptr<Index> index;
    ptr<Log> log;
    ptr<DataFile> data;
    ptr<DataFile> fastData;

public:

};


#endif //KVIK_SHARD_H
