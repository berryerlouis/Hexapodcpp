#! /bin/sh

sudo modprobe cp210x
sudo chmod a+rw /dev/ttyUSB0
bin/stack/avrdude -c arduino -P /dev/ttyUSB0 -b 500000 -p m1284p -U flash:w:build/avr-debug/Hexapodcpp.elf
