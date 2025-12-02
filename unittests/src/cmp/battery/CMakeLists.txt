# Battery component unit tests

# Source files under test
set(SRC_FILES
    ../src/Component/Battery/Battery.cpp
)

# Test files
set(TEST_FILES
    src/cmp/battery/UT_CMP_BATTERY.cpp
)

# Combine and export to parent scope
set(UT_CMP_BATTERY ${SRC_FILES} ${TEST_FILES})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_CMP_BATTERY} PARENT_SCOPE)