set(
  SRC_FILES 
  ../src/Cluster/Frame/Frame.cpp 
  ../src/Component/Proximity/SensorProximity.cpp 
)
set(
  TEST_FILE 
  src/cmp/proximity/UT_CMP_PROXIMITY.cpp
)
set(UT_CMP_PROXIMITY ${SRC_FILES} ${TEST_FILE} )
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_CMP_PROXIMITY} PARENT_SCOPE)