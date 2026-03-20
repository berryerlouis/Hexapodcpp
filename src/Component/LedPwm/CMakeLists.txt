cmake_minimum_required(VERSION 3.22)
add_library(LedPwm
    LedPwm.cpp
)

target_include_directories(LedPwm
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)