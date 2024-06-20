add_executable(
  UT_CMP_SERVOS
  ../../../../src/clu/Frame.cpp
  ../../../../src/cmp/Servo.cpp
  ../../../../src/cmp/Servos.cpp
  UT_CMP_SERVOS.cpp
)

target_link_libraries(
  UT_CMP_SERVOS
  GTest::gtest_main
  GTest::gmock_main
)

add_test(
  NAME
  UT_CMP_SERVOS
  COMMAND
    $<TARGET_FILE:UT_CMP_SERVOS>
  )