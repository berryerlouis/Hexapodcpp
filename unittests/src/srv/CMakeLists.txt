add_subdirectory(battery)
add_subdirectory(body)
add_subdirectory(button)
add_subdirectory(communication)
add_subdirectory(control)
add_subdirectory(display)
add_subdirectory(general)
add_subdirectory(orientation)
add_subdirectory(proximity)
add_subdirectory(sound)

set(SRC_FILES
    ../src/Core/Logger.cpp
)

# Cluster sources required by many service tests
set(SRV_CLUSTER_SRC
    ../src/Cluster/Battery/ClusterBattery.cpp
    ../src/Cluster/Button/ClusterButton.cpp
    ../src/Cluster/Body/ClusterBody.cpp
    ../src/Cluster/General/ClusterGeneral.cpp
    ../src/Cluster/Imu/ClusterImu.cpp
    ../src/Cluster/Proximity/ClusterProximity.cpp
    ../src/Cluster/Servo/ClusterServo.cpp
    ../src/Cluster/Sound/ClusterSound.cpp
    ../src/Cluster/Clusters/Clusters.cpp
)

set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${SRC_FILES} ${SRV_CLUSTER_SRC} PARENT_SCOPE)
