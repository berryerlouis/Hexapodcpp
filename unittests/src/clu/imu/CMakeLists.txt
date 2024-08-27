set(
  SRC_FILES 
  ../src/Cluster/Frame/Frame.cpp 
)
set(
  TEST_FILE 
  src/clu/imu/UT_CLU_IMU.cpp
)
set(UT_CLU_IMU ${SRC_FILES} ${TEST_FILE} )
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_CLU_IMU} PARENT_SCOPE)