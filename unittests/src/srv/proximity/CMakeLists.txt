set(
        SRC_FILES
        ../src/Service/Proximity/ServiceProximity.cpp
)
set(
        TEST_FILE
        src/srv/proximity/UT_SRV_PROXIMITY.cpp

)
set(UT_SRV_PROXIMITY ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_SRV_PROXIMITY} PARENT_SCOPE)