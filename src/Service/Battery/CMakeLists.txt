cmake_minimum_required(VERSION 3.22)
add_library(ServiceBattery 
    ServiceBattery.cpp
)

target_include_directories(ServiceBattery
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)