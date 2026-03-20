cmake_minimum_required(VERSION 3.22)
add_subdirectory(Battery)
add_subdirectory(Button)
add_subdirectory(Body)
add_subdirectory(General)
add_subdirectory(Imu)
add_subdirectory(Proximity)
add_subdirectory(Servo)
add_subdirectory(Sound)
add_subdirectory(Clusters)

set(SRC_FILES_CLUSTER
    ${CMAKE_CURRENT_SOURCE_DIR}/ClusterBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ClusterCommand.cpp
)

add_library(Cluster
    ${SRC_FILES_CLUSTER}
)

target_include_directories(Cluster
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Cluster
    PUBLIC
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