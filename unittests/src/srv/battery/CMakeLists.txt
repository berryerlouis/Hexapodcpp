set(
  SRC_FILES 
  ../src/Cluster/Frame/Frame.cpp 
  ../src/Service/Battery/ServiceBattery.cpp
)
set(
  TEST_FILE 
  src/srv/battery/UT_SRV_BATTERY.cpp

)
set(UT_SRV_BATTERY ${SRC_FILES} ${TEST_FILE} PARENT_SCOPE)