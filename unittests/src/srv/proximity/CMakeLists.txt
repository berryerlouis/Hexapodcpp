add_executable(
  UT_SRV_PROXIMITY
  ../../../../src/clu/Frame.cpp
  ../../../../src/srv/ServiceProximity.cpp
  UT_SRV_PROXIMITY.cpp
)

target_link_libraries(
  UT_SRV_PROXIMITY
  GTest::gtest_main
  GTest::gmock_main
)

add_test(
  NAME
  UT_SRV_PROXIMITY
  COMMAND
    $<TARGET_FILE:UT_SRV_PROXIMITY>
  )