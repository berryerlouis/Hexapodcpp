add_executable(
  UT_CMP_SERVO
  ../../../../src/cmp/Servo.cpp
  UT_CMP_SERVO.cpp
)

target_link_libraries(
  UT_CMP_SERVO
  GTest::gtest_main
  GTest::gmock_main
)

add_test(
  NAME
  UT_CMP_SERVO
  COMMAND
    $<TARGET_FILE:UT_CMP_SERVO>
  )