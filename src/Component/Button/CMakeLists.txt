cmake_minimum_required(VERSION 3.22)
add_library(Button 
    Button.cpp
)

target_include_directories(Button
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)