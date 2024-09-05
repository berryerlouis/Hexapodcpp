set(
        SRC_FILES
        ../src/Component/Servo/Servo.cpp
)
set(
        TEST_FILE
        src/cmp/servo/UT_CMP_SERVO.cpp
)
set(UT_CMP_SERVO ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_CLU_SERVO} PARENT_SCOPE)