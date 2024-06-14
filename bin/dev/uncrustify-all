#!/bin/sh
set -eu

echo $(dirname -- "$0")

list_c_file=$(find ./ -type f -name "*.cpp")
list_h_file=$(find ./ -type f -name "*.h")
for fn in ${list_c_file}; do
    uncrustify.exe -c ./uncrustify.cfg --no-backup $fn
done
for fn in ${list_h_file}; do
    uncrustify.exe -c ./uncrustify.cfg --no-backup $fn
done
