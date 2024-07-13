cmake_minimum_required(VERSION 3.14...3.29)

project(
  Hexapodcpp
  VERSION 0.1
  DESCRIPTION "Hexapodcpp"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


macro(package_add_test TESTNAME FILES )
    add_executable(${TESTNAME} ${FILES})
    target_link_libraries(${TESTNAME} gtest gmock gtest_main)
    add_test(
      NAME
      ${TESTNAME}
      COMMAND
        $<TARGET_FILE:${TESTNAME}>
      )
endmacro()

add_compile_definitions(GTEST)

enable_testing()

include(GoogleTest)

add_subdirectory(src/clu/battery)
add_subdirectory(src/clu/body)
add_subdirectory(src/clu/frame)
add_subdirectory(src/clu/general)
add_subdirectory(src/clu/imu)
add_subdirectory(src/clu/protocol)
add_subdirectory(src/clu/proximity)
add_subdirectory(src/clu/servo)

add_subdirectory(src/cmp/battery)
add_subdirectory(src/cmp/communication)
add_subdirectory(src/cmp/led)
add_subdirectory(src/cmp/mpu9150)
add_subdirectory(src/cmp/pca9685)
add_subdirectory(src/cmp/proximity)
add_subdirectory(src/cmp/servo)
add_subdirectory(src/cmp/servos)
add_subdirectory(src/cmp/srf05)
add_subdirectory(src/cmp/vl53l0x)

add_subdirectory(src/srv/battery)
add_subdirectory(src/srv/control)
add_subdirectory(src/srv/display)
add_subdirectory(src/srv/general)
add_subdirectory(src/srv/orientation)
add_subdirectory(src/srv/proximity)

