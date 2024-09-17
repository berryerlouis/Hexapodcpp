set(
        SRC_FILES
        ../src/Cluster/Frame/Frame.cpp
)
set(
        TEST_FILE
        src/clu/proximity/UT_CLU_PROXIMITY.cpp
)
set(UT_CLU_PROXIMITY ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_CLU_PROXIMITY} PARENT_SCOPE)