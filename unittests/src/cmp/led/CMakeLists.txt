add_executable(
  UT_CMP_LED
  ../../../../src/cmp/Led.cpp
  UT_CMP_LED.cpp
)

target_link_libraries(
  UT_CMP_LED
  GTest::gtest_main
  GTest::gmock_main
)

add_test(
  NAME
  UT_CMP_LED
  COMMAND
    $<TARGET_FILE:UT_CMP_LED>
  )