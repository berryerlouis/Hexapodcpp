cmake_minimum_required(VERSION 3.22)
set(
        SRC_FILES
        ../src/Service/Body/ServiceBody.cpp
)
set(
        TEST_FILE
        src/srv/body/UT_SRV_BODY.cpp

)
set(UT_SRV_BODY ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_SRV_BODY} PARENT_SCOPE)