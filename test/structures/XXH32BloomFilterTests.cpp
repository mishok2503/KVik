#include "KVik/structures/XXH32BloomFilter.h"
#include "KVik/KeyValue.h"


#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

TEST(XXH32BloomFilterTests, EmptyFilterWorks) {
    XXH32BloomFilter<Key, std::vector<Key>> filter;
    EXPECT_EQ(filter.filled(), 0);
    EXPECT_EQ(filter.check(Key{}), false);
}

Key generateRandomKey() {
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

TEST(XXH32BloomFilterTests, FilterWorksCorrectlyForSmallAmountOfObjects) {
    int recordsCount = 100;
    XXH32BloomFilter<Key, std::vector<Key>> filter;
    EXPECT_EQ(filter.filled(), 0);
    std::vector<Key> buf;
    for (int i = 0; i < recordsCount; ++i) {
        // this is pretty much scenario that should be used inside Shard to invoke rebuild operation
        if (filter.filled() * 2 > filter.size()) {
            filter.rebuildFrom(buf);
        }
        Key key = generateRandomKey();
        filter.add(key);
        buf.push_back(key);
    }
    for (int i = 0; i < recordsCount; ++i) {
        EXPECT_TRUE(filter.check(buf[i]));
    }

    int correctlyNegative = 0;
    for (int i = 0; i < recordsCount; ++i) {
        Key key = generateRandomKey();
        correctlyNegative += 1 - filter.check(key);
    }

    // 50% was chosen just so that it passes almost always
    EXPECT_GT(correctlyNegative, recordsCount / 2)
                        << "Correctly negative were detected " << correctlyNegative << " items out of " << recordsCount;
}

TEST(XXH32BloomFilterTests, FilterWorksCorrectlyForBigAmountOfObjects) {
    int recordsCount = 1'000'000;
    XXH32BloomFilter<Key, std::vector<Key>> filter;
    EXPECT_EQ(filter.filled(), 0);
    std::vector<Key> buf;
    for (int i = 0; i < recordsCount; ++i) {
        // this is pretty much scenario that should be used inside Shard to invoke rebuild operation
        if (filter.filled() * 2 > filter.size()) {
            filter.rebuildFrom(buf);
        }
        Key key = generateRandomKey();
        filter.add(key);
        buf.push_back(key);
    }
    for (int i = 0; i < recordsCount; ++i) {
        EXPECT_TRUE(filter.check(buf[i]));
    }

    int correctlyNegative = 0;
    for (int i = 0; i < recordsCount; ++i) {
        correctlyNegative += 1 - filter.check(generateRandomKey());
    }

    // 50% was chosen just so that it passes almost always
    EXPECT_GT(correctlyNegative, recordsCount / 2)
                        << "Correctly negative were detected " << correctlyNegative << " items out of " << recordsCount;
}
