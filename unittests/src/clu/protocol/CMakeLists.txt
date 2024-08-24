set(
  SRC_FILES 
  ../src/Cluster/Frame/Frame.cpp 
  ../src/Cluster/Decoding/Protocol.cpp 
)
set(
  TEST_FILE 
  src/clu/protocol/UT_CLU_PROTOCOL.cpp
)
set(UT_CLU_PROTOCOL ${SRC_FILES} ${TEST_FILE} PARENT_SCOPE)