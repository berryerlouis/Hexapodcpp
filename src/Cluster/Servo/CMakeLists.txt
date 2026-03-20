cmake_minimum_required(VERSION 3.22)
add_library(ClusterServo 
    ClusterServo.cpp
)

target_include_directories(ClusterServo
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)