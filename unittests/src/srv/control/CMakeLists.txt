set(
  SRC_FILES 
  ../src/clu/Frame.cpp 
  ../src/cmp/Servo.cpp 
  ../src/srv/ServiceControl.cpp
)
set(
  TEST_FILE 
  src/srv/control/UT_SRV_CONTROL.cpp

)
set(UT_SRV_CONTROL ${SRC_FILES} ${TEST_FILE} PARENT_SCOPE)