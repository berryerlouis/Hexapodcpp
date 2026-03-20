cmake_minimum_required(VERSION 3.22)
set(
        SRC_FILES
        ../src/Cluster/Button/ClusterButton.cpp
)
set(
        TEST_FILE
        src/clu/button/UT_CLU_BUTTON.cpp
)
set(UT_CLU_BUTTON ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_CLU_BUTTON} PARENT_SCOPE)