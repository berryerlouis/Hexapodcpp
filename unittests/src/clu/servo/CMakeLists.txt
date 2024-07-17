set(
  SRC_FILES 
  ../src/clu/Frame.cpp 
  ../src/cmp/Servo.cpp 
)
set(
  TEST_FILE 
  src/clu/servo/UT_CLU_SERVO.cpp
)
set(UT_CLU_SERVO ${SRC_FILES} ${TEST_FILE} PARENT_SCOPE)