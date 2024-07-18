set(
  SRC_FILES 
  ../src/clu/Frame.cpp 
  ../src/srv/ServiceGeneral.cpp
)
set(
  TEST_FILE 
  src/srv/general/UT_SRV_GENERAL.cpp

)
set(UT_SRV_GENERAL ${SRC_FILES} ${TEST_FILE} PARENT_SCOPE)