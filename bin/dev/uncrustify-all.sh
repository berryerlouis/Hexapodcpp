#!/bin/sh
set -eu

list_source_c_file=$(find ./src/ -type f -name "*.cpp")
list_source_h_file=$(find ./src/ -type f -name "*.h")
list_test_c_file=$(find ./unittests/ -type f -name "*.cpp" -not -path "./unittests/build/*")
list_test_h_file=$(find ./unittests/ -type f -name "*.h" -not -path "./unittests/build/*")
for fn in ${list_source_c_file}; do
    uncrustify.exe -c ./uncrustify.cfg --no-backup $fn
done
for fn in ${list_source_h_file}; do
    uncrustify.exe -c ./uncrustify.cfg --no-backup $fn
done
for fn in ${list_test_c_file}; do
    uncrustify.exe -c ./uncrustify.cfg --no-backup $fn
done
for fn in ${list_test_h_file}; do
    uncrustify.exe -c ./uncrustify.cfg --no-backup $fn
done
