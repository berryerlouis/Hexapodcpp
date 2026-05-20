cmake_minimum_required(VERSION 3.22)
set(
        SRC_FILES
        ../src/Cluster/Sound/ClusterSound.cpp
)
set(
        TEST_FILE
        src/clu/sound/UT_CLU_SOUND.cpp
)
set(UT_CLU_SOUND ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_CLU_SOUND} PARENT_SCOPE)