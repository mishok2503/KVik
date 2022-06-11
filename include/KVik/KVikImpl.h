#ifndef KVIK_KVIKIMPL_H
#define KVIK_KVIKIMPL_H

#include <vector>
#include <memory>

#include "KVik/KVik.h"
#include "KVik/structures/ThreadPool.h"
#include "KVik/Shard.h"

class KVikImpl : public KVik {
private:
    std::vector<ThreadPool> pools;

    std::vector<std::unique_ptr<Shard>> shards;

    explicit KVikImpl(size_t shardsCount);

    size_t getShardNum(Key);

public:
    std::future<void> remove(Key) override;
    std::future<void> add(Key, Value) override;
    std::future<Value> get(Key) override;
    std::future<void> clear() override;

    friend KVikBuilder;
};

#endif //KVIK_KVIKIMPL_H
