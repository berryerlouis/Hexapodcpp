set(
  SRC_FILES 
  ../src/clu/Frame.cpp 
  ../src/cmp/SensorProximity.cpp 
)
set(
  TEST_FILE 
  src/cmp/proximity/UT_CMP_PROXIMITY.cpp
)
set(UT_CMP_PROXIMITY ${SRC_FILES} ${TEST_FILE} PARENT_SCOPE)