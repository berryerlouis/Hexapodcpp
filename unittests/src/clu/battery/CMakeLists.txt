set(
        SRC_FILES
        ../src/Cluster/Frame/Frame.cpp
)
set(
        TEST_FILE
        src/clu/battery/UT_CLU_BATTERY.cpp

)
set(UT_CLU_BATTERY ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_CLU_BATTERY} PARENT_SCOPE)