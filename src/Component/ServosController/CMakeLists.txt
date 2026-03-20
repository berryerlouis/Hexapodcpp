cmake_minimum_required(VERSION 3.22)
add_library(ServosController
    Pca9685.cpp
)

target_include_directories(ServosController
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)