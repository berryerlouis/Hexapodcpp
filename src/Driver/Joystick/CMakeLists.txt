cmake_minimum_required(VERSION 3.22)
add_library(DriverJoystick
    Joystick.cpp
)

target_include_directories(DriverJoystick
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
