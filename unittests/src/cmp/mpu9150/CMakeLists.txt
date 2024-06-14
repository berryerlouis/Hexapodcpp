add_executable(
  UT_CMP_MPU9150
  ../../../../src/cmp/Mpu9150.cpp
  UT_CMP_MPU9150.cpp
)

target_link_libraries(
  UT_CMP_MPU9150
  GTest::gtest_main
  GTest::gmock_main
)

add_test(
  NAME
  UT_CMP_MPU9150
  COMMAND
    $<TARGET_FILE:UT_CMP_MPU9150>
  )