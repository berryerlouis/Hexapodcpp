add_executable(
  UT_CMP_BATTERY
  ../../../../src/cmp/Battery.cpp
  UT_CMP_BATTERY.cpp
)

target_link_libraries(
  UT_CMP_BATTERY
  GTest::gtest_main
  GTest::gmock_main
)

add_test(
  NAME
  UT_CMP_BATTERY
  COMMAND
    $<TARGET_FILE:UT_CMP_BATTERY>
  )