cmake_minimum_required(VERSION 3.22)
add_library(ClusterBattery 
    ClusterBattery.cpp
)

target_include_directories(ClusterBattery
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ClusterBattery
    PUBLIC
        ClusterBase
)