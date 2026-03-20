cmake_minimum_required(VERSION 3.22)
add_library(Servo
    Servo.cpp
)

target_include_directories(Servo
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)