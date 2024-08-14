#! /bin/sh

if [ $# -eq 0 ]; then
    cmake -DCMAKE_BUILD_TYPE=DEBUG -S . -B build
else
    if [ $1 = "source" ]; then
        if [ $# -eq 1 ]; then
            cmake -DCMAKE_BUILD_TYPE=DEBUG -S . -B build
        else 
            cmake -DCMAKE_BUILD_TYPE=${2} -S . -B build
        fi
    fi
    if [ $1 = "test" ]; then
        cd unittests
        cmake -Dgtest_build_samples=ON -DCMAKE_BUILD_TYPE=Debug -S . -B build
    fi
fi
