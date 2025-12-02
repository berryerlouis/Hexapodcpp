cmake_minimum_required(VERSION 3.22)

# Test project definition
project(
    HexapodcppTest
    VERSION 0.1.0
    DESCRIPTION "Unit tests for Hexapodcpp robot control"
    LANGUAGES CXX
)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Fetch GoogleTest
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
    GIT_SHALLOW TRUE
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Cluster tests
add_subdirectory(src/clu)
add_subdirectory(src/clu/battery)
add_subdirectory(src/clu/button)
add_subdirectory(src/clu/body)
add_subdirectory(src/clu/frame)
add_subdirectory(src/clu/general)
add_subdirectory(src/clu/imu)
add_subdirectory(src/clu/protocol)
add_subdirectory(src/clu/proximity)
add_subdirectory(src/clu/servo)
add_subdirectory(src/clu/sound)

# Component tests
add_subdirectory(src/cmp/ads1115)
add_subdirectory(src/cmp/ahrs)
add_subdirectory(src/cmp/battery)
add_subdirectory(src/cmp/communication)
add_subdirectory(src/cmp/button)
add_subdirectory(src/cmp/led)
add_subdirectory(src/cmp/mpu9150)
add_subdirectory(src/cmp/pca9685)
add_subdirectory(src/cmp/proximity)
add_subdirectory(src/cmp/servo)
add_subdirectory(src/cmp/servos)
add_subdirectory(src/cmp/sound)
add_subdirectory(src/cmp/srf05)
add_subdirectory(src/cmp/vl53l0x)

# Service tests
add_subdirectory(src/srv/battery)
add_subdirectory(src/srv/button)
add_subdirectory(src/srv/communication)
add_subdirectory(src/srv/control)
add_subdirectory(src/srv/display)
add_subdirectory(src/srv/general)
add_subdirectory(src/srv/orientation)
add_subdirectory(src/srv/proximity)
add_subdirectory(src/srv/sound)

# Bot tests
add_subdirectory(src/bot)
add_subdirectory(src/bot/body)

# Create test executable
add_executable(HexapodcppTest ${EXECUTABLES_SRC})

# Set target compile definitions
target_compile_definitions(HexapodcppTest
    PRIVATE
        X64
        GTEST
)

# Link test libraries
target_link_libraries(HexapodcppTest
    PRIVATE
        GTest::gtest_main
        GTest::gmock_main
)

# Enable compiler warnings for tests
target_compile_options(HexapodcppTest
    PRIVATE
        -Wall
        -Wextra
)

# Discover and register tests
include(GoogleTest)
gtest_discover_tests(HexapodcppTest
    DISCOVERY_TIMEOUT 30
)