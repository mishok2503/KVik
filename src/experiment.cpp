#include <iostream>
#include <fstream>
#include <functional>
#include <vector>
#include <random>
#include <string>
#include <chrono>

#include "MurmurHash3.h"
#include "SpeedTest.h"
#include "xxhash.h"

static size_t PAGE_SIZE = 4096;
static int DEFAULT_START_BUCKETS_COUNT = 4;
static int DEFAULT_BUCKET_SIZE = PAGE_SIZE - 1;

//struct global_hash_model {
//private:
//    int buckets_count_{};
//    int bucket_size_{};
//    int cnt{};
//    int seed{};
//    char *buffer_;
//public:
//    global_hash_model() : buckets_count_(DEFAULT_START_BUCKETS_COUNT), bucket_size_(DEFAULT_BUCKET_SIZE), buffer_(new char[(1 + bucket_size_) * buckets_count_]), seed(rand()) {}
//
//    void add(char* key) {
//        XXH32_hash_t h = XXH32(key, 16, seed);
//        uint32_t i = h % buckets_count_;
//        size_t offset = i * PAGE_SIZE;
//        uint8_t items_in_ith_buckets = buffer_[offset];
//        if (bucket_size_ == items_in_ith_buckets)
//    }
//
//};

struct global_hash_model {
    std::vector<std::vector<std::string>> ht;
    int bucket_size = 120;
    int cnt = 0;
    int seed = 0;

    global_hash_model() : ht(4), seed(rand()) {}

    [[nodiscard]] uint32_t get_hash(std::string const& key) const {
        uint32_t h = XXH32(key.data(), key.size(), seed);
        return h;
    }

    void rehash() {
        std::vector<std::vector<std::string>> next_ht(ht.size() * 2);
        seed = rand();
        for (const auto& bucket : ht) {
            for (auto element : bucket) {
                next_ht[get_hash(element) % next_ht.size()].push_back(std::move(element));
            }
        }
        ht = std::move(next_ht);
    }

    void tell_fill_factor() {
        std::cout << cnt << ' ' << (double) cnt / ((double) ht.size() * bucket_size) << std::endl;
    }

    void add(std::string const& key) {
        uint32_t h = get_hash(key);
        uint32_t i = h % ht.size();
        ++cnt;
        ht[i].push_back(key);
        if (ht[i].size() > bucket_size) {
            tell_fill_factor();
            rehash();
        }
    }
    
    [[nodiscard]] bool is_in(std::string const& key) {
        uint32_t h = get_hash(key);
        uint32_t i = h % ht.size();
        return std::find(std::begin(ht[i]), std::end(ht[i]), key) != std::end(ht[i]);
    }
};

int main(int argc, char **argv) {
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    auto generate_random_string = [&](){
        std::string ans;
        for (int i = 0; i < 16; ++i) {
            ans += (char) (rng() % 126 + 1);
        }
        return ans;
    };

    auto generate_bad_random_string = [&]() {
        std::string ans;
        for (int i = 0; i < 16; ++i) {
            ans += (char) (rand() % 126 + 1);
        }
        return ans;
    };
    global_hash_model md;
    global_hash_model md_bad;
    for (;;) {
        const std::string key = std::move(generate_random_string());
        if (!md.is_in(key)) md.add(key);
    }
}
