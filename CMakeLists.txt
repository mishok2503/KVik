cmake_minimum_required(VERSION 3.10)

project(KVik)

set(CMAKE_CXX_STANDARD 17)
include_directories(src)

include_directories(include/KVik)

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
        src/memory/test/DRAMMemoryTests.cpp
        src/memory/test/FileMemoryTests.cpp
        src/memory/test/MallocDRAMMemoryAllocatorTests.cpp
        src/memory/test/DirectoryFileMemoryAllocatorTests.cpp
        src/memory/DRAMMemory.cpp
        src/memory/FileMemory.cpp
        src/memory/MallocDRAMMemoryAllocator.cpp
        src/memory/DirectoryFileMemoryAllocator.cpp
)
# just an entry point for tests
target_link_libraries(
        KVik_test
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(KVik_test)