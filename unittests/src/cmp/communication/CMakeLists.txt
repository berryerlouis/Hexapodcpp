add_executable(
  UT_CMP_COMMUNICATION
  ../../../../src/clu/Frame.cpp
  ../../../../src/clu/Protocol.cpp
  ../../../../src/cmp/Communication.cpp
  UT_CMP_COMMUNICATION.cpp
)

target_link_libraries(
  UT_CMP_COMMUNICATION
  GTest::gtest_main
  GTest::gmock_main
)

add_test(
  NAME
  UT_CMP_COMMUNICATION
  COMMAND
    $<TARGET_FILE:UT_CMP_COMMUNICATION>
  )