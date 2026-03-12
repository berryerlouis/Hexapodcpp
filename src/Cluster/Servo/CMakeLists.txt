add_library(ClusterServo 
    ClusterServo.cpp
)

target_include_directories(ClusterServo
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)