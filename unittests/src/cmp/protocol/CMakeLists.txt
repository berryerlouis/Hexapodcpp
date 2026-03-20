cmake_minimum_required(VERSION 3.22)
set(
        SRC_FILES
        ../src/Component/Communication/Decoding/Protocol.cpp
)
set(
        TEST_FILE
        src/cmp/protocol/UT_CMP_PROTOCOL.cpp
)
set(UT_CMP_PROTOCOL ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_CMP_PROTOCOL} PARENT_SCOPE)