add_library(ClusterGeneral 
    ClusterGeneral.cpp
)

target_include_directories(ClusterGeneral
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)