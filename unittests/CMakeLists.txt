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

# Disable GoogleTest samples and tests to speed up build
set(BUILD_GMOCK ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(gtest_build_samples OFF CACHE BOOL "" FORCE)
set(gtest_build_tests OFF CACHE BOOL "" FORCE)
set(gmock_build_tests OFF CACHE BOOL "" FORCE)

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

# tests
add_subdirectory(src/cmp)
add_subdirectory(src/move)
add_subdirectory(src/cor)
add_subdirectory(src/srv)
add_subdirectory(src/clu)
add_subdirectory(src/bot)

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
        -O0
        -g3
)

# Discover and register tests
include(GoogleTest)
gtest_discover_tests(HexapodcppTest
        DISCOVERY_TIMEOUT 30
)