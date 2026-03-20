cmake_minimum_required(VERSION 3.22)
set(
        SRC_FILES
        ../src/Cluster/Battery/ClusterBattery.cpp
)
set(
        TEST_FILE
        src/clu/battery/UT_CLU_BATTERY.cpp
)
set(UT_CLU_BATTERY ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_CLU_BATTERY} PARENT_SCOPE)