set(
        SRC_FILES
        ../src/Service/Battery/ServiceBattery.cpp
)
set(
        TEST_FILE
        src/srv/battery/UT_SRV_BATTERY.cpp

)
set(UT_SRV_BATTERY ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_SRV_BATTERY} PARENT_SCOPE)