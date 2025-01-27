#! /bin/sh

if [ $# -eq 0 ]; then
    cmake -DCMAKE_BUILD_TYPE=DEBUG -S . -B build
else
    if [ $2 = "sources" ]; then
        if [ $# -eq 1 ]; then
            cmake -DCMAKE_BUILD_TYPE=DEBUG -DTARGET="${1}" -DGTEST=0 -S . -B build
        else 
            if [ $3 = "CLEAN" ]; then
                cmake --build . --target clean
            else 
                cmake -DCMAKE_BUILD_TYPE=${3} -DTARGET="${1}" -DGTEST=0 -S . -B build
            fi
        fi
    fi
    if [ $2 = "test" ]; then
        cd unittests
        if [ $3 = "all" ] || [ $3 = "clean" ] || [ $3 = "help" ]; then
            cmake -Dgtest_build_samples=ON -DCMAKE_BUILD_TYPE=Debug -DUT_TO_BUILD="all" -S . -B build
        else
            cmake -Dgtest_build_samples=ON -DCMAKE_BUILD_TYPE=Debug -DUT_TO_BUILD=${3} -S . -B build
        fi
    fi
fi
