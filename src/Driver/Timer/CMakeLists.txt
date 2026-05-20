cmake_minimum_required(VERSION 3.22)
add_library(DriverTimer 
    Tick${TARGET}.cpp
)

target_include_directories(DriverTimer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)