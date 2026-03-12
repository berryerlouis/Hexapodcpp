add_library(ClusterProximity 
    ClusterProximity.cpp
)

target_include_directories(ClusterProximity
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)