cmake_minimum_required(VERSION 3.22)
add_library(ClusterGeneral
    ClusterGeneral.cpp
)

target_include_directories(ClusterGeneral
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ClusterGeneral
    PUBLIC
        ClusterBase
)