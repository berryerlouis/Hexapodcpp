#! /bin/sh

if [ $# -eq 0 ]; then
    cmake -S . -B build
else
    if [ $1 = "source" ]; then
        cmake -S . -B build
    fi
    if [ $1 = "test" ]; then
        cd unittests
        cmake -S . -B build
    fi
fi
