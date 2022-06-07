#include <gtest/gtest.h>
#include "KVik/KVikBuilder.h"

TEST(ASDLK, ABOBA) {
    KVikBuilder builder;

    auto kvs = builder.build();

    auto v = kvs->get({0});

    Value expV = {'a', 'b', 'c'};
    for (int i=0; i < 100; ++i) {
        std::cout << (expV.data[i] == '\0') << ' ';
    }
    std::cout << "\nDATA: " << std::string(expV.data) << '\n';
    EXPECT_EQ(std::string(v.get().data), std::string(expV.data));
}