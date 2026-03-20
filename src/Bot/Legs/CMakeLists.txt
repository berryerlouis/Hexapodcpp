cmake_minimum_required(VERSION 3.22)
add_library(Legs
    Legs.cpp
)

target_include_directories(Legs
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)