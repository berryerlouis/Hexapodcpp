cmake_minimum_required(VERSION 3.22)
add_library(Decoding 
    Protocol.cpp
)

target_include_directories(Decoding
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)