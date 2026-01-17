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

# Target platform validation
if(NOT DEFINED TARGET)
    set(TARGET "X64" CACHE STRING "Target platform (X64 or RPI)" FORCE)
message(STATUS "Target platform: ${TARGET}")
endif()

# Option for tests
option(GTEST "Build unit tests" OFF)

if(GTEST)
    add_subdirectory(unittests)
else()
    add_subdirectory(src)
endif()