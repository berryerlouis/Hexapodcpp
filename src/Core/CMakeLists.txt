cmake_minimum_required(VERSION 3.22)
add_library(Core 
    Logger.cpp
)

target_include_directories(Core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)