#include <gtest/gtest.h>

#include <memory>
#include <random>

#include "KVik/structures/IndexHashTable.h"
#include "KVik/memory/DirectoryFixedFileMemoryAllocator.h"

static Key generateRandomKey() {
    static std::random_device rd;
    static std::mt19937_64 rng(rd());
    static std::uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);

    Key key{};
    uint64_t hi = dist(rng);
    uint64_t lo = dist(rng);
    memcpy(key.data, &hi, sizeof(uint64_t));
    memcpy(key.data + sizeof(uint64_t), &lo, sizeof(uint64_t));

    return key;
}

TEST(IndexHashTableTest, EmptyWorks) {
    auto allocator = std::make_unique<DirectoryFixedFileMemoryAllocator>(".", "index-ht");
    IndexHashTable indexHT(std::move(allocator));
    EXPECT_EQ(indexHT.get(Key{}), 0);
    // TODO : check size of hash table
}

TEST(IndexHashTableTest, AddingSingleKeyWorks) {
    constexpr int64_t correspondingOffset = 123'123;

    auto allocator = std::make_unique<DirectoryFixedFileMemoryAllocator>(".", "index-ht");
    IndexHashTable indexHT(std::move(allocator));
    Key key{};
    EXPECT_EQ(indexHT.get(key), 0);
    indexHT.put(key, correspondingOffset);
    EXPECT_EQ(indexHT.get(key), correspondingOffset);
    // TODO : check size
}

TEST(IndexHashTableTest, AddingSeveralKeysWorks) {
    constexpr int iters = 1000;
    std::vector<std::pair<Key, int64_t>> htSimulation;

    auto allocator = std::make_unique<DirectoryFixedFileMemoryAllocator>(".", "index-ht");
    IndexHashTable indexHT(std::move(allocator));

    auto put = [&](Key const &key, int64_t value) {
        for (auto & i : htSimulation) {
            if (i.first == key) {
                i.second = value;
                return;
            }
        }
        htSimulation.emplace_back(key, value);
    };

    for (int64_t i = 0; i < iters; ++i) {
        Key randomKey = generateRandomKey();
        int64_t value = i;
        put(randomKey, value);
        indexHT.put(randomKey, value);
    }

    for (auto &p : htSimulation) {
        EXPECT_EQ(indexHT.get(p.first), p.second);
    }
}

TEST(IndexHashTableTest, AddingAndRemovingSeveralKeysWorks) {
    constexpr int iters = 1000;
    std::vector<std::pair<Key, int64_t>> htSimulation;

    auto allocator = std::make_unique<DirectoryFixedFileMemoryAllocator>(".", "index-ht");
    IndexHashTable indexHT(std::move(allocator));

    auto put = [&](std::vector<std::pair<Key, int64_t>> &ht, Key const &key, int64_t value) {
        for (auto & i : ht) {
            if (i.first == key) {
                i.second = value;
                return;
            }
        }
        ht.emplace_back(key, value);
    };

    auto remove = [&] (std::vector<std::pair<Key, int64_t>> &ht, Key const &key) {
        for (int i = 0; i < ht.size(); ++i) {
            if (key == ht[i].first) {
                ht.erase(std::begin(ht) + i);
                return;
            }
        }
    };

    auto get = [&] (std::vector<std::pair<Key, int64_t>> &ht, Key const &key) {
        for (int i = 0; i < ht.size(); ++i) {
            if (ht[i].first == key) {
                return ht[i].second;
            }
        }
        return (int64_t) 0;
    };

    for (int64_t i = 0; i < iters; ++i) {
        Key key = generateRandomKey();
        int64_t value = i;
        indexHT.put(key, value);
        put(htSimulation, key, value);
    }

    std::vector<std::pair<Key, int64_t>> htSimulationCopy = htSimulation;

    for (int i = 0; i < htSimulation.size(); ++i) {
        bool shouldBeRemoved = rand() % 2 == 0;
        if (shouldBeRemoved) {
            Key key = htSimulation[i].first;
            remove(htSimulation, key);
            indexHT.remove(key);
        }
    }

    for (int i = 0; i < htSimulationCopy.size(); ++i) {
        EXPECT_EQ(indexHT.get(htSimulationCopy[i].first), get(htSimulationCopy, htSimulationCopy[i].first));
    }
}