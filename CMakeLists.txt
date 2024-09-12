cmake_minimum_required(VERSION 3.22.1)

# Project name
project(Hexapodcpp
        VERSION 1.0
        DESCRIPTION "Hexapodcpp")

if (${GTEST})
    add_subdirectory(unittests)
else ()
    add_subdirectory(src)
endif ()