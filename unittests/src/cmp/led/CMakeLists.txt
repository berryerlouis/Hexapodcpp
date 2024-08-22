set(
  SRC_FILES 
  ../src/Component/Led/Led.cpp 
)
set(
  TEST_FILE 
  src/cmp/led/UT_CMP_LED.cpp
)
set(UT_CMP_LED ${SRC_FILES} ${TEST_FILE} PARENT_SCOPE)
