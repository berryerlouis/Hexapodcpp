add_library(ClusterBattery 
    ClusterBattery.cpp
)

target_include_directories(ClusterBattery
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)