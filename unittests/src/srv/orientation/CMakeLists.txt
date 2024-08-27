set(
  SRC_FILES 
  ../src/Service/Orientation/ServiceOrientation.cpp
)
set(
  TEST_FILE 
  src/srv/orientation/UT_SRV_ORIENTATION.cpp

)
set(UT_SRV_ORIENTATION ${SRC_FILES} ${TEST_FILE} )
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_SRV_ORIENTATION} PARENT_SCOPE)