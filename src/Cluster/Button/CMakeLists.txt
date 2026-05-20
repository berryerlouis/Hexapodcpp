cmake_minimum_required(VERSION 3.22)
add_library(ClusterButton
    ClusterButton.cpp
)

target_include_directories(ClusterButton
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ClusterButton
    PUBLIC
        ClusterBase
)