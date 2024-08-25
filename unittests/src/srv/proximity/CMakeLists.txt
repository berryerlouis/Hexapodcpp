set(
  SRC_FILES 
  ../src/Service/Proximity/ServiceProximity.cpp
)
set(
  TEST_FILE 
  src/srv/proximity/UT_SRV_PROXIMITY.cpp

)
set(UT_SRV_PROXIMITY ${SRC_FILES} ${TEST_FILE} PARENT_SCOPE)