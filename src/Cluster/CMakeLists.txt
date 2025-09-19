cmake_minimum_required(VERSION 3.22)

set(SRC_FILES_CLUSTER
        Cluster/Battery/ClusterBattery.cpp
        Cluster/Button/ClusterButton.cpp
        Cluster/Body/ClusterBody.cpp
        Cluster/General/ClusterGeneral.cpp
        Cluster/Imu/ClusterImu.cpp
        Cluster/Proximity/ClusterProximity.cpp
        Cluster/Servo/ClusterServo.cpp
        Cluster/ClusterBase.cpp
        Cluster/ClusterCommand.cpp
        Cluster/Sound/ClusterSound.cpp
        Cluster/Clusters/Clusters.cpp
        Cluster/Decoding/Protocol.cpp
        Cluster/Frame/Frame.cpp
        PARENT_SCOPE
)