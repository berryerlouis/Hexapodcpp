add_executable(
  UT_CLU_BODY
  ../../../../src/clu/Frame.cpp
  UT_CLU_BODY.cpp
)

target_link_libraries(
  UT_CLU_BODY
  GTest::gtest_main
  GTest::gmock_main
)

add_test(
  NAME
  UT_CLU_BODY
  COMMAND
    $<TARGET_FILE:UT_CLU_BODY>
  )