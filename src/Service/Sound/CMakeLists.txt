cmake_minimum_required(VERSION 3.22)
add_library(ServiceSound 
    ServiceSound.cpp
)

target_include_directories(ServiceSound
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)