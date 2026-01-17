set(
        SRC_FILES
        ../src/Cluster/General/ClusterGeneral.cpp
)
set(
        TEST_FILE
        src/clu/general/UT_CLU_GENERAL.cpp
)
set(UT_CLU_GENERAL ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_CLU_GENERAL} PARENT_SCOPE)