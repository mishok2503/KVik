cmake_minimum_required(VERSION 3.21)
project(HSE_DSS)

set(CMAKE_CXX_STANDARD 17)

add_executable(HSE_DSS
        src/main.cpp)

target_link_libraries(HSE_DSS lz4)
target_link_libraries(HSE_DSS zstd)
