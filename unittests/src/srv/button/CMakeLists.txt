set(
        SRC_FILES
        ../src/Service/Button/ServiceButton.cpp
)
set(
        TEST_FILE
        src/srv/button/UT_SRV_BUTTON.cpp

)
set(UT_SRV_BUTTON ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_SRV_BUTTON} PARENT_SCOPE)