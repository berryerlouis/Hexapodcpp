set(
        SRC_FILES
        #../src/Cluster/Frame/Frame.cpp
)
set(
        TEST_FILE
        src/clu/frame/UT_CLU_FRAME.cpp
)
set(UT_CLU_FRAME ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_CLU_FRAME} PARENT_SCOPE)