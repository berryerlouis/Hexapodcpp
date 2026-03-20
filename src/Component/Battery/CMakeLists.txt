cmake_minimum_required(VERSION 3.22)
add_library(Battery 
    Battery.cpp
)

target_include_directories(Battery
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)