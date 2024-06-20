add_executable(
  UT_CLU_PROXIMITY
  ../../../../src/clu/Frame.cpp
  UT_CLU_PROXIMITY.cpp
)

target_link_libraries(
  UT_CLU_PROXIMITY
  GTest::gtest_main
  GTest::gmock_main
)

add_test(
  NAME
  UT_CLU_PROXIMITY
  COMMAND
    $<TARGET_FILE:UT_CLU_PROXIMITY>
  )