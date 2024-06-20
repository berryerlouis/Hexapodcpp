add_executable(
  UT_SRV_CONTROL
  ../../../../src/clu/Frame.cpp
  ../../../../src/cmp/Servo.cpp
  ../../../../src/srv/ServiceControl.cpp
  UT_SRV_CONTROL.cpp
)

target_link_libraries(
  UT_SRV_CONTROL
  GTest::gtest_main
  GTest::gmock_main
)

add_test(
  NAME
  UT_SRV_CONTROL
  COMMAND
    $<TARGET_FILE:UT_SRV_CONTROL>
  )