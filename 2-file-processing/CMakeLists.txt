cmake_minimum_required(VERSION 3.20)

project(2-file-processing)

set(CMAKE_CXX_STANDARD 11)

add_executable(process-text-file
        process-text-file/lib/file.cpp
        process-text-file/lib/file.h
        process-text-file/main.cpp)
