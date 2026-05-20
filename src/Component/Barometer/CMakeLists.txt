cmake_minimum_required(VERSION 3.22)
add_library(Barometer 
    Barometer.cpp
)

target_include_directories(Barometer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)