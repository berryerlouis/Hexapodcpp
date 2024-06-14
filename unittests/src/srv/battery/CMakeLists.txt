add_executable(
  UT_SRV_BATTERY
  ../../../../src/clu/Frame.cpp
  ../../../../src/srv/Services.cpp
  ../../../../src/srv/ServiceBattery.cpp
  UT_SRV_BATTERY.cpp
)

target_link_libraries(
  UT_SRV_BATTERY
  GTest::gtest_main
  GTest::gmock_main
)

add_test(
  NAME
  UT_SRV_BATTERY
  COMMAND
    $<TARGET_FILE:UT_SRV_BATTERY>
  )