add_executable(
  UT_SRV_ORIENTATION
  ../../../../src/clu/Frame.cpp
  ../../../../src/srv/Services.cpp
  ../../../../src/srv/ServiceOrientation.cpp
  UT_SRV_ORIENTATION.cpp
)

target_link_libraries(
  UT_SRV_ORIENTATION
  GTest::gtest_main
  GTest::gmock_main
)

add_test(
  NAME
  UT_SRV_ORIENTATION
  COMMAND
    $<TARGET_FILE:UT_SRV_ORIENTATION>
  )