add_executable(
  UT_CMP_SFR05
  ../../../../src/cmp/Srf05.cpp
  UT_CMP_SFR05.cpp
)

target_link_libraries(
  UT_CMP_SFR05
  GTest::gtest_main
  GTest::gmock_main
)

add_test(
  NAME
  UT_CMP_SFR05
  COMMAND
    $<TARGET_FILE:UT_CMP_SFR05>
  )