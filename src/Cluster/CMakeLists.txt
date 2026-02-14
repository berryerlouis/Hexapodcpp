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
    ${SRC_FILES_BATTERY}
    ${SRC_FILES_BUTTON}
    ${SRC_FILES_BODY}
    ${SRC_FILES_GENERAL}
    ${SRC_FILES_IMU}
    ${SRC_FILES_PROXIMITY}
    ${SRC_FILES_SERVO}
    ${SRC_FILES_SOUND}
    ${SRC_FILES_CLUSTERS}
)

target_include_directories(Cluster
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR} 
        ${CMAKE_CURRENT_SOURCE_DIR}/Battery
        ${CMAKE_CURRENT_SOURCE_DIR}/Body
        ${CMAKE_CURRENT_SOURCE_DIR}/Button
        ${CMAKE_CURRENT_SOURCE_DIR}/Clusters
        ${CMAKE_CURRENT_SOURCE_DIR}/General
        ${CMAKE_CURRENT_SOURCE_DIR}/Imu
        ${CMAKE_CURRENT_SOURCE_DIR}/Proximity
        ${CMAKE_CURRENT_SOURCE_DIR}/Servo
        ${CMAKE_CURRENT_SOURCE_DIR}/Sound
)

target_link_libraries(Cluster
    PUBLIC
        Communication
)