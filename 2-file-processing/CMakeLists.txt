cmake_minimum_required(VERSION 3.20)

project(2-file-processing)

set(CMAKE_CXX_STANDARD 20)

add_library(file_utils STATIC
        text-file/file_utils/file_utils.hpp
        text-file/file_utils/file_utils.cpp)

add_executable(text-file text-file/main.cpp)
target_include_directories(text-file PRIVATE text-file/file_utils)
target_link_libraries(text-file PRIVATE file_utils)


add_library(binary_utils STATIC
        binary-file/utils/utils.hpp
        binary-file/utils/utils.cpp)

add_executable(binary-file binary-file/main.cpp)
target_include_directories(binary-file PRIVATE binary-file/utils)
target_link_libraries(binary-file PRIVATE binary_utils)