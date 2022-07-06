#include <xxhash.h>
#include <bitset>
#include <cstring>
#include "KVik/structures/IndexHashTable.h"

static constexpr size_t HT_VALUE_SIZE = KEY_SIZE + OFFSET_SIZE;
static constexpr size_t VALUES_IN_BUCKET = (8 * BUCKET_SIZE) / (1 + 8 * HT_VALUE_SIZE);
static constexpr size_t HEADER_SIZE = (VALUES_IN_BUCKET + 7) / 8;
static constexpr size_t HEADER_SIZE_BITS = HEADER_SIZE * 8;

static bool compareKeys(const char *key1, const char *key2) {
    for (size_t i = 0; i < KEY_SIZE; i++) {
        if (key1[i] != key2[i]) {
            return false;
        }
    }
    return true;
}

IndexHashTable::IndexHashTable(std::unique_ptr<ZeroedMemoryAllocator> &&allocatorPtr, std::unique_ptr<Memory> &&dumpedIndexHT)
: _memory(std::move(dumpedIndexHT)), _allocator(std::move(allocatorPtr)) {
    _memory->read(0, sizeof(_bucketCnt), &_bucketCnt);
}

IndexHashTable::IndexHashTable(std::unique_ptr<ZeroedMemoryAllocator> &&allocatorPtr) : _bucketCnt(PRIME_SIZES[0]), _allocator(std::move(allocatorPtr)) {
    _memory = _allocator->alloc(sizeof(_bucketCnt) + _bucketCnt * BUCKET_SIZE);
    _memory->write(0, _bucketCnt, &_bucketCnt);
}

void IndexHashTable::resize() {
    auto oldMemory = std::move(_memory);
    auto oldBucketCnt = _bucketCnt;
    size_t primesIndex = std::lower_bound(PRIME_SIZES, PRIME_SIZES + PRIME_SIZES_COUNT, _bucketCnt) - PRIME_SIZES;

    _bucketCnt = PRIME_SIZES[++primesIndex];
    _memory = _allocator->alloc(sizeof(_bucketCnt) + _bucketCnt * BUCKET_SIZE);
    _memory->write(0, sizeof(_bucketCnt), &_bucketCnt);

    // used instead of '_buffer' in order to keep bucket data same after call to 'put'
    char resizeBuffer[BUCKET_SIZE];
    for (uint64_t bucketIndex = 0; bucketIndex < oldBucketCnt; bucketIndex++) {
        oldMemory->read(sizeof(_bucketCnt) + bucketIndex * BUCKET_SIZE, BUCKET_SIZE, resizeBuffer);
        auto mask = *(std::bitset<VALUES_IN_BUCKET> *) resizeBuffer;
        for (size_t pos = 0; pos < VALUES_IN_BUCKET; pos++) {
            if (!mask[pos]) {
                continue;
            }
            Key key{};
            memcpy(&key, resizeBuffer + HEADER_SIZE + pos * HT_VALUE_SIZE, KEY_SIZE);
            int64_t value;
            memcpy(&value, resizeBuffer + HEADER_SIZE + pos * HT_VALUE_SIZE + KEY_SIZE, sizeof(value));
            put(key, value);
        }
    }

    _allocator->dealloc(std::move(oldMemory));
}

void IndexHashTable::put(const Key &key, int64_t value) {
    uint64_t hash = XXH3_64bits(&key, KEY_SIZE);
    uint64_t bucketIndex = hash % _bucketCnt;
    // TODO : create function that accepts just bucketIndex and reads given bucket to _buffer
    _memory->read(sizeof(_bucketCnt) + bucketIndex * BUCKET_SIZE, BUCKET_SIZE, _buffer);
    auto mask = *(std::bitset<VALUES_IN_BUCKET> *) _buffer;
    size_t pos = 0;
    for (; pos < VALUES_IN_BUCKET; ++pos) {
        if (mask[pos] && compareKeys(key.data, _buffer + HEADER_SIZE + pos * HT_VALUE_SIZE)) {
            break;
        }
    }
    if (pos == VALUES_IN_BUCKET) {
        if (mask.all()) {
            resize();
            put(key, value);
            return;
        }
        pos = mask.flip()._Find_first();
        if (pos >= VALUES_IN_BUCKET) {
            resize();
            put(key, value);
            return;
        }
    }
    (*(std::bitset<VALUES_IN_BUCKET> *) _buffer)[pos] = true;
    memcpy(_buffer + HEADER_SIZE + pos * HT_VALUE_SIZE, &key, KEY_SIZE);
    memcpy(_buffer + HEADER_SIZE + pos * HT_VALUE_SIZE + KEY_SIZE, &value, sizeof(value));
    _memory->write(sizeof(_bucketCnt) + bucketIndex * BUCKET_SIZE, BUCKET_SIZE, _buffer);
}

int64_t IndexHashTable::get(Key const &key) {
    uint64_t hash = XXH3_64bits(&key, KEY_SIZE);
    uint64_t bucketIndex = hash % _bucketCnt;

    _memory->read(sizeof(_bucketCnt) + bucketIndex * BUCKET_SIZE, BUCKET_SIZE, _buffer);
    auto mask = *(std::bitset<VALUES_IN_BUCKET> *) _buffer;
    for (size_t pos = 0; pos < VALUES_IN_BUCKET; pos++) {
        if (!mask[pos]) {
            continue;
        }
        if (!compareKeys(key.data, _buffer + HEADER_SIZE + pos * HT_VALUE_SIZE)) {
            continue;
        }
        return *(int64_t *) (_buffer + HEADER_SIZE + pos * HT_VALUE_SIZE + KEY_SIZE);
    }
    //didn't find, we expect offsets to be non-zero integers
    return 0;
}

void IndexHashTable::remove(Key const &key) {
    uint64_t hash = XXH3_64bits(&key, KEY_SIZE);
    uint64_t bucketNum = hash % _bucketCnt;

    _memory->read(sizeof(_bucketCnt) + bucketNum * BUCKET_SIZE, BUCKET_SIZE, _buffer);
    auto mask = *(std::bitset<HEADER_SIZE_BITS> *) _buffer;
    // TODO : make some template function that loops over given bucket and executes certain function if certain predicate is true
    for (size_t pos = 0; pos < VALUES_IN_BUCKET; pos++) {
        if (!mask[pos]) {
            continue;
        }
        if (!compareKeys(key.data, _buffer + HEADER_SIZE + pos * HT_VALUE_SIZE)) {
            continue;
        }
        (*(std::bitset<HEADER_SIZE_BITS> *) _buffer)[pos] = false;
        _memory->write(sizeof(_bucketCnt) + bucketNum * BUCKET_SIZE, HEADER_SIZE, _buffer);
        return;
    }
}
