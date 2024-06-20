add_executable(
  UT_CLU_IMU
  ../../../../src/clu/Frame.cpp
  UT_CLU_IMU.cpp
)

target_link_libraries(
  UT_CLU_IMU
  GTest::gtest_main
  GTest::gmock_main
)

add_test(
  NAME
  UT_CLU_IMU
  COMMAND
    $<TARGET_FILE:UT_CLU_IMU>
  )