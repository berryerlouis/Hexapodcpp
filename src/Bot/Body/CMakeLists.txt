cmake_minimum_required(VERSION 3.22)
add_library(Body 
    Body.cpp
)

target_include_directories(Body
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Body
    PUBLIC
        Move
)
