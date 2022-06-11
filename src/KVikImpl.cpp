#include "KVik/KVikImpl.h"

#include "xxhash.h"

std::future<void> KVikImpl::remove(Key key) {
    size_t n = getShardNum(key);
    return pools[n]->enqueue([&] { shards[n]->remove(key); });
}

std::future<void> KVikImpl::add(Key key, Value value) {
    size_t n = getShardNum(key);
    return pools[n]->enqueue([&] { shards[n]->add(key, value); });
}

std::future<Value> KVikImpl::get(Key key) {
    size_t n = getShardNum(key);
//    return pools[n]->enqueue([&] { return shards[n]->get(key); });
    return pools[n]->enqueue([this, key, n] { return shards[n]->get(key); });
}

std::future<void> KVikImpl::clear() {
    std::vector<std::future<void>> futures(shards.size());
    for (size_t i=0; i < shards.size(); ++i) {
        futures[i] = pools[i]->enqueue([&] { return shards[i]->clear(); });
    }
    return pools[0]->enqueue([&] {
        for (auto& f : futures) {
            f.get();
        }
    });
}

KVikImpl::KVikImpl(size_t shardsCount) : shards(shardsCount) {
    for (size_t i=0; i < shardsCount; ++i) {
        pools.emplace_back(std::make_unique<ThreadPool>(1));
    }
}

size_t KVikImpl::getShardNum(Key key) {
    constexpr uint32_t seed = 239;
    return XXH32(&key, sizeof(key), seed) % shards.size();
}
