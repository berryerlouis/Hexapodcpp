add_library(ClusterImu 
    ClusterImu.cpp
)

target_include_directories(ClusterImu
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)