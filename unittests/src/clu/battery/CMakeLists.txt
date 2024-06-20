add_executable(
  UT_CLU_BATTERY
  ../../../../src/clu/Frame.cpp
  UT_CLU_BATTERY.cpp
)

target_link_libraries(
  UT_CLU_BATTERY
  GTest::gtest_main
  GTest::gmock_main
)

add_test(
  NAME
  UT_CLU_BATTERY
  COMMAND
    $<TARGET_FILE:UT_CLU_BATTERY>
  )