set(
        SRC_FILES
        ../src/Component/Button/Button.cpp
)
set(
        TEST_FILE
        src/cmp/button/UT_CMP_BUTTON.cpp
)
set(UT_CMP_BUTTON ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_CMP_BUTTON} PARENT_SCOPE)