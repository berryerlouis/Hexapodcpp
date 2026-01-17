set(
        SRC_FILES
        ../src/Component/Sound/Sound.cpp
)
set(
        TEST_FILE
        src/cmp/sound/UT_CMP_SOUND.cpp
)
set(UT_CMP_SOUND ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_CMP_SOUND} PARENT_SCOPE)