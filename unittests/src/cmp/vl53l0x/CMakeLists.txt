add_executable(
  UT_CMP_VL53L0X
  ../../../../src/cmp/Vl53l0x.cpp
  UT_CMP_VL53L0X.cpp
)

target_link_libraries(
  UT_CMP_VL53L0X
  GTest::gtest_main
  GTest::gmock_main
)

add_test(
  NAME
  UT_CMP_VL53L0X
  COMMAND
    $<TARGET_FILE:UT_CMP_VL53L0X>
  )