cmake_minimum_required(VERSION 3.22)
add_subdirectory(ads1115)
add_subdirectory(ahrs)
add_subdirectory(battery)
add_subdirectory(button)
add_subdirectory(communication)
add_subdirectory(frame)
add_subdirectory(led)
add_subdirectory(mpu9150)
add_subdirectory(pca9685)
add_subdirectory(protocol)
add_subdirectory(proximity)
add_subdirectory(servo)
add_subdirectory(servos)
add_subdirectory(sound)
add_subdirectory(srf05)
add_subdirectory(vl53l0x)

# Shared test helper sources
set(SRC_FILES
    ../src/Core/Logger.cpp
)

# Cluster sources needed by some component tests (e.g. communication)
set(CMP_CLUSTER_SRC
    ../src/Cluster/Body/ClusterBody.cpp
    ../src/Cluster/Clusters/Clusters.cpp
    ../src/Cluster/ClusterBase.cpp
    ../src/Cluster/ClusterCommand.cpp
)

set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${SRC_FILES} ${CMP_CLUSTER_SRC} PARENT_SCOPE)
