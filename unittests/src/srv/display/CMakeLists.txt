set(
  SRC_FILES 
  ../src/cor/Bitmaps.cpp 
  ../src/clu/Frame.cpp 
  ../src/srv/ServiceDisplay.cpp
)
set(
  TEST_FILE 
  src/srv/display/UT_SRV_DISPLAY.cpp

)
set(UT_SRV_DISPLAY ${SRC_FILES} ${TEST_FILE} PARENT_SCOPE)