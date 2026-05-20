cmake_minimum_required(VERSION 3.22)
add_library(ServiceCommunication 
    ServiceCommunication.cpp
)

target_include_directories(ServiceCommunication
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)