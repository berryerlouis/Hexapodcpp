cmake_minimum_required(VERSION 3.22)
add_library(ServiceGeneral 
    ServiceGeneral.cpp
)

target_include_directories(ServiceGeneral
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)