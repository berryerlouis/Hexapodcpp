set(
        SRC_FILES
        ../src/Component/Imu/Ahrs.cpp
)
set(
        TEST_FILE
        src/cmp/ahrs/UT_CMP_AHRS.cpp
)
set(UT_CMP_AHRS ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_CMP_AHRS} PARENT_SCOPE)