set(
  SRC_FILES 
  ../src/cmp/Pca9685.cpp 
)
set(
  TEST_FILE 
  src/cmp/pca9685/UT_CMP_PCA9685.cpp
)
set(UT_CMP_PCA9685 ${SRC_FILES} ${TEST_FILE} PARENT_SCOPE)