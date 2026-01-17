set(
        SRC_FILES
        ../src/Cluster/Frame/Frame.cpp
        ../src/Cluster/ClusterCommand.cpp
        ../src/Cluster/ClusterBase.cpp
        ../src/Core/Logger.cpp
)
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${SRC_FILES} PARENT_SCOPE)