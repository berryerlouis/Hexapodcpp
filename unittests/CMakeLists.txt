cmake_minimum_required(VERSION 3.22.1)

# Project name
project(
        HexapodcppTest
        VERSION 0.1
        DESCRIPTION "Unit test for Hexapodcpp"
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

add_compile_definitions(GTEST)

enable_testing()

add_subdirectory(src/clu/battery)
add_subdirectory(src/clu/body)
add_subdirectory(src/clu/frame)
add_subdirectory(src/clu/general)
add_subdirectory(src/clu/imu)
add_subdirectory(src/clu/protocol)
add_subdirectory(src/clu/proximity)
add_subdirectory(src/clu/servo)

add_subdirectory(src/cmp/ahrs)
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
add_subdirectory(src/srv/communication)
add_subdirectory(src/srv/control)
add_subdirectory(src/srv/display)
add_subdirectory(src/srv/general)
add_subdirectory(src/srv/orientation)
add_subdirectory(src/srv/proximity)

add_executable(
        HexapodcppTest
        ${EXECUTABLES_SRC}
)

target_link_libraries(
        HexapodcppTest
        GTest::gtest_main
        GTest::gmock_main
)

include(GoogleTest)

gtest_discover_tests(HexapodcppTest)