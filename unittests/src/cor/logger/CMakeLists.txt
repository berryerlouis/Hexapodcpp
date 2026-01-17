set(
        SRC_FILES
        ../src/Core/Logger.cpp
)
set(
        TEST_FILE
        src/cor/logger/UT_COR_LOGGER.cpp
)
set(UT_COR_LOGGER ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_COR_LOGGER} PARENT_SCOPE)