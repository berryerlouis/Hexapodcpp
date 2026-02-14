set(
        SRC_FILES
        ../src/Component/Communication/Frame/Frame.cpp
        ../src/Component/Communication/Decoding/Protocol.cpp
        ../src/Component/Communication/Communication.cpp
)

set(
        TEST_FILE
        src/cmp/communication/UT_CMP_COMMUNICATION.cpp
)
set(UT_CMP_COMMUNICATION ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_CMP_COMMUNICATION} PARENT_SCOPE)