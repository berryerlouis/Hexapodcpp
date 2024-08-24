set(
  SRC_FILES 
  ../src/Component/Imu/Mpu9150.cpp 
)
set(
  TEST_FILE 
  src/cmp/mpu9150/UT_CMP_MPU9150.cpp
)
set(UT_CMP_MPU9150 ${SRC_FILES} ${TEST_FILE} PARENT_SCOPE)