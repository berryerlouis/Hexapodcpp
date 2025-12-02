# Gait unit tests

# Source files under test
set(SRC_FILES
    ../src/Move/Gait/GaitParams.cpp
    ../src/Move/Gait/GaitCycle.cpp
)

# Test files
set(TEST_FILES
    src/move/gait/UT_MOVE_GAIT_PARAMS.cpp
    src/move/gait/UT_MOVE_GAIT_CYCLE.cpp
)

# Combine and export to parent scope
set(UT_MOVE_GAIT ${SRC_FILES} ${TEST_FILES})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_MOVE_GAIT} PARENT_SCOPE)
