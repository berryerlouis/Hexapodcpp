cmake_minimum_required(VERSION 3.22)
set(
        SRC_FILES
        ../src/Bot/Leg/Leg.cpp
)
set(
        TEST_FILE
        src/bot/leg/UT_BOT_LEG.cpp
)
set(UT_BOT_LEG ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_BOT_LEG} PARENT_SCOPE)
