cmake_minimum_required(VERSION 3.10)

project(KVik)

set(CMAKE_CXX_STANDARD 17)

# should already be installed locally, installation guide that may be useful: https://github.com/Cyan4973/xxHash/tree/dev/cmake_unofficial
find_package(xxHash 0.7 CONFIG REQUIRED)

include_directories(src)

include_directories(include/)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/0320f517fd920866d918e564105d68fd4362040a.zip
        #                                                ^ latest commit hash as for 4th june of 2022
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# here all files containing tests will be
add_executable(
        KVik_test
        test/memory/DRAMMemoryTests.cpp
        test/memory/FileMemoryTests.cpp
        test/memory/MallocDRAMMemoryAllocatorTests.cpp
        test/memory/DirectoryFileMemoryAllocatorTests.cpp
        test/structures/XXH32BloomFilterTests.cpp
        test/structures/HddDataTests.cpp
        test/structures/IndexHashTableTests.cpp
        test/memory/MmapDRAMMemoryAllocatorTests.cpp
        test/TEST.cpp
        src/memory/DRAMMemory.cpp
        src/structures/HddData.cpp
        src/memory/FileMemory.cpp
        src/memory/MallocDRAMMemoryAllocator.cpp
        src/memory/DirectoryFileMemoryAllocator.cpp
        src/memory/MmapDRAMMemoryAllocator.cpp
        src/Shard.cpp
        src/KvikBuilder.cpp
        src/KVikImpl.cpp
        src/KeyValue.cpp
        src/structures/IndexImpl.cpp
        src/structures/IndexHashTable.cpp
        src/structures/Log.cpp
        src/structures/SsdData.cpp
        src/structures/HddData.cpp
        )
# just an entry point for tests
target_link_libraries(
        KVik_test
        gtest_main
)

target_link_libraries(KVik_test xxHash::xxhash)

include(GoogleTest)
gtest_discover_tests(KVik_test)