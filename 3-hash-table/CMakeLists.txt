cmake_minimum_required(VERSION 3.20)

project(3-hash-table)

include_directories(AFTER hash_table/ third_party/ binary/ ./)

set(CMAKE_CXX_STANDARD 20)

add_library(binary STATIC
        binary/binary.hpp
        binary/binary.cpp controller.hpp)

add_library(hash_table STATIC
        hash_table/hash_map.hpp hash_table/hash_map.cpp)

add_library(catch2 SHARED
        third_party/catch2/catch2.hpp
        third_party/catch2/catch2.cpp)


add_executable(main main.cpp controller.hpp)

target_link_libraries(main PUBLIC hash_table binary)

add_executable(test tests/test_hash_map.cpp controller.hpp)
target_link_libraries(test PUBLIC catch2)

add_executable(benchmark tests/benchmark.cpp)
target_link_libraries(benchmark PUBLIC catch2 binary hash_table)