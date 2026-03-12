add_library(ClusterSound 
    ClusterSound.cpp
)

target_include_directories(ClusterSound
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)