set(
  SRC_FILES 
  ../src/clu/Frame.cpp 
  ../src/cmp/Servo.cpp 
  ../src/cmp/Servos.cpp 
)
set(
  TEST_FILE 
  src/cmp/servos/UT_CMP_SERVOS.cpp
)
set(UT_CMP_SERVOS ${SRC_FILES} ${TEST_FILE} PARENT_SCOPE)