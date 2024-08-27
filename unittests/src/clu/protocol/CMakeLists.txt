set(
  SRC_FILES 
  ../src/Cluster/Frame/Frame.cpp 
  ../src/Cluster/Decoding/Protocol.cpp 
)
set(
  TEST_FILE 
  src/clu/protocol/UT_CLU_PROTOCOL.cpp
)
set(UT_CLU_PROTOCOL ${SRC_FILES} ${TEST_FILE})
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_CLU_PROTOCOL} PARENT_SCOPE)