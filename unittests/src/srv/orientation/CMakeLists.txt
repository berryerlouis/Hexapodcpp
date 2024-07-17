set(
  SRC_FILES 
  ../src/clu/Frame.cpp 
  ../src/srv/ServiceOrientation.cpp
)
set(
  TEST_FILE 
  src/srv/orientation/UT_SRV_ORIENTATION.cpp

)
set(UT_SRV_ORIENTATION ${SRC_FILES} ${TEST_FILE} PARENT_SCOPE)