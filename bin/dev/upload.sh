#! /bin/sh

bin/stack/avrdude.exe -c arduino -P COM4 -b 500000 -p m1284p -U flash:w:build/Hexapodcpp.elf
