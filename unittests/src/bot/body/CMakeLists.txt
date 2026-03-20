cmake_minimum_required(VERSION 3.22)
set(
        SRC_FILES
        ../src/Bot/Leg/Leg.cpp
        ../src/Bot/Legs/Legs.cpp
        ../src/Bot/Body/Body.cpp
        ../src/Move/Gait/GaitStrategy.cpp
        ../src/Move/Gait/GaitParams.cpp
        ../src/Move/Gait/GaitCycle.cpp
        ../src/Move/Walk/Walk.cpp
)
set(
        TEST_FILE
        src/bot/body/UT_BOT_BODY.cpp
)
set(UT_BOT_BODY ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_BOT_BODY} PARENT_SCOPE)