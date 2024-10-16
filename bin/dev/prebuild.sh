#! /bin/sh

if [ $# -eq 0 ]; then
    cmake -DCMAKE_BUILD_TYPE=DEBUG -S . -B build
else
    if [ $1 = "source" ]; then
        if [ $# -eq 1 ]; then
            cmake -DCMAKE_BUILD_TYPE=DEBUG -DTARGET="avr" -DGTEST=0 -S . -B build
        else 
            if [ $2 = "CLEAN" ]; then
                cmake --build . --target clean
            else 
                cmake -DCMAKE_BUILD_TYPE=${2} -DTARGET="avr" -DGTEST=0 -S . -B build
            fi
        fi
    fi
    if [ $1 = "test" ]; then
        cd unittests
        if [ $2 = "all" ] || [ $2 = "clean" ] || [ $2 = "help" ]; then
            cmake -Dgtest_build_samples=ON -DCMAKE_BUILD_TYPE=Debug -DUT_TO_BUILD="all" -S . -B build
        else
            cmake -Dgtest_build_samples=ON -DCMAKE_BUILD_TYPE=Debug -DUT_TO_BUILD=${2} -S . -B build
        fi
    fi
fi
