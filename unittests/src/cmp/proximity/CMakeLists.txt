add_executable(
  UT_CMP_PROXIMITY
  ../../../../src/clu/Frame.cpp
  ../../../../src/cmp/SensorProximity.cpp
  UT_CMP_PROXIMITY.cpp
)

target_link_libraries(
  UT_CMP_PROXIMITY
  GTest::gtest_main
  GTest::gmock_main
)

add_test(
  NAME
  UT_CMP_PROXIMITY
  COMMAND
    $<TARGET_FILE:UT_CMP_PROXIMITY>
  )