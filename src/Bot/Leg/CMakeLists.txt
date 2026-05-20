cmake_minimum_required(VERSION 3.22)
add_library(Leg 
    Leg.cpp
)

target_include_directories(Leg
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)