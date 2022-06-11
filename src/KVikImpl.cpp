#include "KVik/KVikImpl.h"

std::future<void> KVikImpl::remove(Key key) {
    size_t n = getShardNum(key);
    return pools[n].enqueue([&] { shards[n]->remove(key); });
}

std::future<void> KVikImpl::add(Key key, Value value) {
    size_t n = getShardNum(key);
    return pools[n].enqueue([&] { shards[n]->add(key, value); });
}

std::future<Value> KVikImpl::get(Key key) {
    size_t n = getShardNum(key);
    return pools[n].enqueue([&] { return shards[n]->get(key); });
}

std::future<void> KVikImpl::clear() {
    std::vector<std::future<void>> futures(shards.size());
    for (size_t i=0; i < shards.size(); ++i) {
        futures[i] = pools[i].enqueue([&] { return shards[i]->clear(); });
    }
    return pools.begin()->enqueue([&] {
        for (auto& f : futures) {
            f.get();
        }
    });
}

KVikImpl::KVikImpl(size_t shardsCount) : pools(shardsCount, /*threads in pool*/1), shards(shardsCount) {}

size_t KVikImpl::getShardNum(Key key) {

}
