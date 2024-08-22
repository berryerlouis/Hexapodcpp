set(
  SRC_FILES 
  ../src/Cluster/Frame/Frame.cpp 
  ../src/Service/Orientation/ServiceOrientation.cpp
)
set(
  TEST_FILE 
  src/srv/orientation/UT_SRV_ORIENTATION.cpp

)
set(UT_SRV_ORIENTATION ${SRC_FILES} ${TEST_FILE} PARENT_SCOPE)