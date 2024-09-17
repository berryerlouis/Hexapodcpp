set(
        SRC_FILES
        ../src/Service/Communication/ServiceCommunication.cpp
)
set(
        TEST_FILE
        src/srv/communication/UT_SRV_COMMUNICATION.cpp

)
set(UT_SRV_COMMUNICATION ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_SRV_COMMUNICATION} PARENT_SCOPE)