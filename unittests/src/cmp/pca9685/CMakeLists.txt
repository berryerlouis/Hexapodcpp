add_executable(
  UT_CMP_PCA9685
  ../../../../src/cmp/Pca9685.cpp
  UT_CMP_PCA9685.cpp
)

target_link_libraries(
  UT_CMP_PCA9685
  GTest::gtest_main
  GTest::gmock_main
)

add_test(
  NAME
  UT_CMP_PCA9685
  COMMAND
    $<TARGET_FILE:UT_CMP_PCA9685>
  )