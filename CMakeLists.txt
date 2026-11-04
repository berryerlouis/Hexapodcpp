cmake_minimum_required(VERSION 3.22.1)

# Project definition
project(Hexapodcpp
        VERSION 1.0.0.0
        DESCRIPTION "Hexapod robot control software"
        LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type validation
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel)

# Target platform validation
set(TARGET "X64" CACHE STRING "Target platform (X64 or RPI)")
set_property(CACHE TARGET PROPERTY STRINGS X64 RPI)
if(NOT "${TARGET}" STREQUAL "X64" AND NOT "${TARGET}" STREQUAL "RPI")
    message(FATAL_ERROR "Unknown TARGET: '${TARGET}'. Must be X64 or RPI")
endif()
message(STATUS "Target platform: ${TARGET}, build type: ${CMAKE_BUILD_TYPE}")

# Always generate compile_commands.json (used by clangd, clang-tidy and IDEs)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler cache (applies to application, dependencies and unit tests)
option(HEXAPOD_USE_CCACHE "Use ccache when available" ON)
if(HEXAPOD_USE_CCACHE AND NOT CMAKE_CXX_COMPILER_LAUNCHER)
    find_program(HEXAPOD_CCACHE_EXECUTABLE NAMES ccache)
    if(HEXAPOD_CCACHE_EXECUTABLE)
        message(STATUS "Using ccache: ${HEXAPOD_CCACHE_EXECUTABLE}")
        set(CMAKE_CXX_COMPILER_LAUNCHER "${HEXAPOD_CCACHE_EXECUTABLE}")
        set(CMAKE_C_COMPILER_LAUNCHER "${HEXAPOD_CCACHE_EXECUTABLE}")
    endif()
endif()

# Option for tests
option(GTEST "Build unit tests" OFF)

if(GTEST)
    enable_testing()
    add_subdirectory(unittests)
else()
    add_subdirectory(src)
endif()