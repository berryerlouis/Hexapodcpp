set(
        SRC_FILES
        ../src/Service/Sound/ServiceSound.cpp
)
set(
        TEST_FILE
        src/srv/sound/UT_SRV_SOUND.cpp

)
set(UT_SRV_SOUND ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_SRV_SOUND} PARENT_SCOPE)