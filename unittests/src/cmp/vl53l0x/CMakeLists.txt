set(
        SRC_FILES
        ../src/Component/Proximity/Laser/Vl53l0x.cpp
)
set(
        TEST_FILE
        src/cmp/vl53l0x/UT_CMP_VL53L0X.cpp
)
set(UT_CMP_VL53L0X ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_CMP_VL53L0X} PARENT_SCOPE)