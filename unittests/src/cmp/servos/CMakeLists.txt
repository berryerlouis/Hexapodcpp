set(
  SRC_FILES 
../src/Cluster/Frame/Frame.cpp 
  ../src/Component/Servo/Servo.cpp 
  ../src/Component/Servos/Servos.cpp 
)
set(
  TEST_FILE 
  src/cmp/servos/UT_CMP_SERVOS.cpp
)
set(UT_CMP_SERVOS ${SRC_FILES} ${TEST_FILE} PARENT_SCOPE)