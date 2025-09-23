set(
        SRC_FILES
        ../src/Bot/Leg/Leg.cpp
        ../src/Bot/Legs/Legs.cpp
        ../src/Bot/Body/Body.cpp
        ../src/Move/Walk/Walk.cpp
        ../src/Move/Gait/GaitBase.cpp
        ../src/Move/Gait/Gaits.cpp
        ../src/Move/Gait/GaitParams.cpp
        ../src/Move/Gait/GaitCycle.cpp
        ../src/Move/Gait/GaitRipple.cpp
        ../src/Move/Gait/GaitTripod.cpp
        ../src/Move/Gait/GaitWave.cpp
        ../src/Move/Gait/GaitDoubleWave.cpp
)
set(
        TEST_FILE
        src/bot/body/UT_BOT_BODY.cpp
)
set(UT_BOT_BODY ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_BOT_BODY} PARENT_SCOPE)