add_library(Clusters 
    Clusters.cpp
)

target_include_directories(Clusters
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)