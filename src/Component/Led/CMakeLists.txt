cmake_minimum_required(VERSION 3.22)
add_library(Led
    Led.cpp
)

target_include_directories(Led
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)