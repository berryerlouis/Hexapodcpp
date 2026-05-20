cmake_minimum_required(VERSION 3.22)
add_library(Buffer
    Buffer.cpp
)

target_include_directories(Buffer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)