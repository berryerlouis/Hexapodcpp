add_executable(
  UT_CLU_PROTOCOL
  ../../../src/clu/Frame.cpp
  ../../../src/clu/Protocol.cpp
  UT_CLU_PROTOCOL.cpp
)

target_link_libraries(
  UT_CLU_PROTOCOL
  GTest::gtest_main
  GTest::gmock_main
)

add_test(
  NAME
  UT_CLU_PROTOCOL
  COMMAND
    $<TARGET_FILE:UT_CLU_PROTOCOL>
  )