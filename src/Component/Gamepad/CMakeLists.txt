cmake_minimum_required(VERSION 3.22)
add_library(Gamepad
    Gamepad.cpp
)

target_include_directories(Gamepad
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Gamepad
    PUBLIC
        DriverJoystick
)
