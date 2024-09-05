set(
        SRC_FILES
        ../src/Cluster/Frame/Frame.cpp
        ../src/Component/Servo/Servo.cpp
)
set(
        TEST_FILE
        src/clu/servo/UT_CLU_SERVO.cpp
)
set(UT_CLU_SERVO ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_CLU_SERVO} PARENT_SCOPE)