set(
        SRC_FILES
        ../src/Bot/Leg/Leg.cpp
        ../src/Bot/Legs/Legs.cpp
        ../src/Bot/Body/Body.cpp
        ../src/Bot/Walk/Walk.cpp
        ../src/Bot/Gait/GaitBase.cpp
        ../src/Bot/Gait/Gaits.cpp
        ../src/Bot/Gait/GaitParams.cpp
        ../src/Bot/Gait/GaitCycle.cpp
        ../src/Bot/Gait/GaitRipple.cpp
        ../src/Bot/Gait/GaitTripod.cpp
        ../src/Bot/Gait/GaitWave.cpp
        ../src/Bot/Gait/GaitDoubleWave.cpp
)
set(
        TEST_FILE
        src/Bot/Body/UT_BOT_BODY.cpp
)
set(UT_BOT_BODY ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_BOT_BODY} PARENT_SCOPE)