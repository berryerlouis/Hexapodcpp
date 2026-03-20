cmake_minimum_required(VERSION 3.22)
set(
        SRC_FILES
        ../src/Component/Communication/Frame/Frame.cpp
        ../src/Component/Servo/Servo.cpp
        ../src/Component/Servos/Servos.cpp
)
set(
        TEST_FILE
        src/cmp/servos/UT_CMP_SERVOS.cpp
)
set(UT_CMP_SERVOS ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_CMP_SERVOS} PARENT_SCOPE)