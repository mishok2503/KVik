#ifndef KVIK_DEFAULT_BLOOM_FILTER_H
#define KVIK_DEFAULT_BLOOM_FILTER_H

#include "KVik/structures/Filter.h"

#include <random>
#include "KVik/Constants.h"
#include "xxhash.h"


static constexpr int HASH_COUNT = 4;

template<typename K, typename Iterable>
class XXH32BloomFilter : Filter<K, Iterable> {
public:

    explicit XXH32BloomFilter() : _bits(PRIME_SIZES[_curSizeIndex]) {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::uint32_t> dist(0, UINT32_MAX);
        for (auto &seed: _seeds) {
            seed = dist(rng);
        }
    }

    void add(const K &key) override {
        for (auto seed : _seeds) {
            auto hash = XXH32(&key, sizeof(K), seed);
            uint64_t index = hash % PRIME_SIZES[_curSizeIndex];
            if (!_bits[index]) {
                ++_ones;
            }
            _bits[index] = true;
        }
    }

    size_t size() override {
        return PRIME_SIZES[_curSizeIndex];
    }

    bool check(const K &key) override {
        for (auto seed : _seeds) {
            auto hash = XXH32(&key, sizeof(K), seed);
            uint64_t index = hash % PRIME_SIZES[_curSizeIndex];
            if (!_bits[index]) {
                return false;
            }
        }
        return true;
    }

    size_t filled() override {
        return _ones;
    }

    void rebuildFrom(Iterable const &collection) override {
        if (_curSizeIndex + 1 == PRIME_SIZES_COUNT) {
            throw FilterException("error while rebuilding filter, filter can't become any bigger");
        }
        _bits = std::vector<bool>(PRIME_SIZES[++_curSizeIndex]);
        _ones = 0;
        for (const K &element: collection) {
            add(element);
        }
    }

private:

    int _curSizeIndex = 0;

    size_t _ones = 0;

    std::vector<bool> _bits;

    uint32_t _seeds[HASH_COUNT] = {};

};

#endif //KVIK_DEFAULT_BLOOM_FILTER_H
