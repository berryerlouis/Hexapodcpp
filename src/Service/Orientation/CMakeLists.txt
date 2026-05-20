cmake_minimum_required(VERSION 3.22)
add_library(ServiceOrientation 
    ServiceOrientation.cpp
)

target_include_directories(ServiceOrientation
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)