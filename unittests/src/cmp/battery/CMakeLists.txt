set(
        SRC_FILES
        ../src/Component/Battery/Battery.cpp
)
set(
        TEST_FILE
        src/cmp/battery/UT_CMP_BATTERY.cpp
)
set(UT_CMP_BATTERY ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_CMP_BATTERY} PARENT_SCOPE)