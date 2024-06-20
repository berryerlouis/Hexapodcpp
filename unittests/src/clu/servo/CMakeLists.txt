add_executable(
  UT_CLU_SERVO
  ../../../../src/clu/Frame.cpp
  ../../../../src/cmp/Servo.cpp
  UT_CLU_SERVO.cpp
)

target_link_libraries(
  UT_CLU_SERVO
  GTest::gtest_main
  GTest::gmock_main
)

add_test(
  NAME
  UT_CLU_SERVO
  COMMAND
    $<TARGET_FILE:UT_CLU_SERVO>
  )