add_executable(
  UT_SRV_GENERAL
  ../../../../src/clu/Frame.cpp
  ../../../../src/srv/ServiceGeneral.cpp
  UT_SRV_GENERAL.cpp
)

target_link_libraries(
  UT_SRV_GENERAL
  GTest::gtest_main
  GTest::gmock_main
)

add_test(
  NAME
  UT_SRV_GENERAL
  COMMAND
    $<TARGET_FILE:UT_SRV_GENERAL>
  )