set(
  SRC_FILES 
  ../src/Cluster/Frame/Frame.cpp 
)
set(
  TEST_FILE 
  src/clu/imu/UT_CLU_IMU.cpp
)
set(UT_CLU_IMU ${SRC_FILES} ${TEST_FILE} PARENT_SCOPE)