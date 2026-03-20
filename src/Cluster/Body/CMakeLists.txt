cmake_minimum_required(VERSION 3.22)
add_library(ClusterBody 
    ClusterBody.cpp
)

target_include_directories(ClusterBody
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)