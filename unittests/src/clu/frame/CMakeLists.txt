add_executable(
  UT_CLU_FRAME
  ../../../../src/clu/Frame.cpp
  UT_CLU_FRAME.cpp
)

target_link_libraries(
  UT_CLU_FRAME
  GTest::gtest_main
  GTest::gmock_main
)

add_test(
  NAME
  UT_CLU_FRAME
  COMMAND
    $<TARGET_FILE:UT_CLU_FRAME>
  )