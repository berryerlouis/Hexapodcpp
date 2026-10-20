cmake_minimum_required(VERSION 3.22)
add_library(ServiceGamepad
    ServiceGamepad.cpp
)

target_include_directories(ServiceGamepad
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
