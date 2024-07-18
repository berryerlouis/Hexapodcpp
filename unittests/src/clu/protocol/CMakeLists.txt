set(
  SRC_FILES 
  ../src/clu/Frame.cpp 
  ../src/clu/Protocol.cpp 
)
set(
  TEST_FILE 
  src/clu/protocol/UT_CLU_PROTOCOL.cpp
)
set(UT_CLU_PROTOCOL ${SRC_FILES} ${TEST_FILE} PARENT_SCOPE)