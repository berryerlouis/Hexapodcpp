set(
  SRC_FILES 
../src/Cluster/Frame/Frame.cpp 
  ../src/Component/Servo/Servo.cpp 
  ../src/Service/Control/ServiceControl.cpp
)
set(
  TEST_FILE 
  src/srv/control/UT_SRV_CONTROL.cpp

)
set(UT_SRV_CONTROL ${SRC_FILES} ${TEST_FILE} PARENT_SCOPE)