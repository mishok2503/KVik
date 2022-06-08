#ifndef KVIK_DEFAULT_BLOOM_FILTER_H
#define KVIK_DEFAULT_BLOOM_FILTER_H

#include "KVik/structures/Filter.h"

#include <random>
#include "xxhash.h"


static constexpr int HASH_COUNT = 4;

static constexpr int SIZES_COUNT = 30;

static constexpr uint64_t PRIME_SIZES[SIZES_COUNT] = {
        7, 17, 31, 61, 127, 241, 487, 967, 1'931, 3'847, 7'681, 15'361,
        30'727, 61'441, 122'887, 245'771, 491'527, 983'063, 1'996'081, 3'932'167,
        7'864'331, 15'728'681, 31'457'287, 60'000'011, 120'000'007, 240'000'011, 480'000'019,
        960'000'011, 1'900'000'043, 3'800'000'003,
};

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
        if (_curSizeIndex + 1 == SIZES_COUNT) {
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
