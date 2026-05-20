cmake_minimum_required(VERSION 3.22)
add_library(Services 
    Services.cpp
)

target_include_directories(Services
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)