set(
  SRC_FILES  
  ../src/Component/Proximity/Ultrasound/Srf05.cpp 
)
set(
  TEST_FILE 
  src/cmp/srf05/UT_CMP_SRF05.cpp
)
set(UT_CMP_SRF05 ${SRC_FILES} ${TEST_FILE} )
set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${UT_CMP_SRF05} PARENT_SCOPE)