cmake_minimum_required(VERSION 3.22)
add_library(Proximity
    SensorProximity.cpp
    Laser/Vl53l5x.cpp
    Ultrasound/Srf05.cpp
)

target_include_directories(Proximity
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)