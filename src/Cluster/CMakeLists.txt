cmake_minimum_required(VERSION 3.22)

# Base cluster library with ClusterBase and ClusterCommand
set(SRC_FILES_CLUSTER_BASE
    ${CMAKE_CURRENT_SOURCE_DIR}/ClusterBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ClusterCommand.cpp
)

add_library(ClusterBase
    ${SRC_FILES_CLUSTER_BASE}
)

target_include_directories(ClusterBase
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ClusterBase
    PUBLIC
        Frame
)

# Add specific cluster subdirectories
add_subdirectory(Battery)
add_subdirectory(Button)
add_subdirectory(Body)
add_subdirectory(General)
add_subdirectory(Imu)
add_subdirectory(Proximity)
add_subdirectory(Servo)
add_subdirectory(Sound)
add_subdirectory(Clusters)

# Aggregate cluster library
add_library(Cluster INTERFACE)

target_link_libraries(Cluster
    INTERFACE
        ClusterBattery
        ClusterBody
        ClusterButton
        Clusters
        ClusterGeneral
        ClusterImu
        ClusterProximity
        ClusterServo
        ClusterSound
        Frame
)