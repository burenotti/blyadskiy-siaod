cmake_minimum_required(VERSION 3.20)

project(2-file-processing)

set(CMAKE_CXX_STANDARD 20)

add_library(file_utils STATIC
        2-1-divide-by-min/file_utils/file_utils.hpp
        2-1-divide-by-min/file_utils/file_utils.cpp)

add_executable(2-1-divide-by-min 2-1-divide-by-min/main.cpp)
target_include_directories(2-1-divide-by-min PRIVATE 2-1-divide-by-min/file_utils)
target_link_libraries(2-1-divide-by-min PRIVATE file_utils)
