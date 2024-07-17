set(
  SRC_FILES 
  ../src/clu/Frame.cpp 
  ../src/clu/Protocol.cpp 
  ../src/cmp/Communication.cpp 
)
set(
  TEST_FILE 
  src/cmp/communication/UT_CMP_COMMUNICATION.cpp
)
set(UT_CMP_COMMUNICATION ${SRC_FILES} ${TEST_FILE} PARENT_SCOPE)