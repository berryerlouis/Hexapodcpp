set(
  SRC_FILES 
  ../src/Service/General/ServiceGeneral.cpp
)
set(
  TEST_FILE 
  src/srv/general/UT_SRV_GENERAL.cpp

)
set(UT_SRV_GENERAL ${SRC_FILES} ${TEST_FILE} PARENT_SCOPE)