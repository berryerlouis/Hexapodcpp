cmake_minimum_required(VERSION 3.22)
add_library(ServiceEvent 
    EventDispatcher.cpp
)

target_include_directories(ServiceEvent
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)