set(
  SRC_FILES 
  ../src/Service/Control/ServiceControl.cpp
)
set(
  TEST_FILE 
  src/srv/control/UT_SRV_CONTROL.cpp

)
set(UT_SRV_CONTROL ${SRC_FILES} ${TEST_FILE} )
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_SRV_CONTROL} PARENT_SCOPE)