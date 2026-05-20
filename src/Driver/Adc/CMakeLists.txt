cmake_minimum_required(VERSION 3.22)
add_library(DriverAdc 
    Adc${TARGET}.cpp
)

target_include_directories(DriverAdc
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)