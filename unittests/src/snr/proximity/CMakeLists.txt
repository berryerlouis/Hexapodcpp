add_executable(
  UT_SNR_PROXIMITY
  ../../../../src/clu/Frame.cpp
  ../../../../src/snr/SensorProximity.cpp
  UT_SNR_PROXIMITY.cpp
)

target_link_libraries(
  UT_SNR_PROXIMITY
  GTest::gtest_main
  GTest::gmock_main
)

add_test(
  NAME
  UT_SNR_PROXIMITY
  COMMAND
    $<TARGET_FILE:UT_SNR_PROXIMITY>
  )