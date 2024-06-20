add_executable(
  UT_CLU_GENERAL
  ../../../../src/clu/Frame.cpp
  UT_CLU_GENERAL.cpp
)

target_link_libraries(
  UT_CLU_GENERAL
  GTest::gtest_main
  GTest::gmock_main
)

add_test(
  NAME
  UT_CLU_GENERAL
  COMMAND
    $<TARGET_FILE:UT_CLU_GENERAL>
  )