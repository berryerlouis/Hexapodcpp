cmake_minimum_required(VERSION 3.22)
add_library(Walk 
    Walk.cpp
)

target_include_directories(Walk
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Walk
    INTERFACE
        Legs
)