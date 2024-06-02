 [![build](https://github.com/berryerlouis/Hexapodcpp/actions/workflows/test.yaml/badge.svg)](https://github.com/berryerlouis/Hexapodcpp/actions/workflows/Build.yaml)
 
 # Install 
  First install the following tools:
  - install uncrustify
  - install arduino IDE
  - install VS-code
  - install arduino extension
  - install MightyCore (support atmega1284p) ![](https://github.com/MCUdude/MightyCore)
  - install serial-monitor extension
 
 # Config
  - MightyCore:avr:1284,
  - bootloader=uart0,
  - pinout=standard,
  - eeprom=keep,
  - baudrate=default,
  - variant=modelP,
  - BOD=4v3,
  - LTO=Os_flto,
  - clock=20MHz_external
 
 # Compile (alt+crtl+i)
 
 # Upload (alt+crtl+u)

 # Serial 
 Communication settings are :
 115200 8 N 1

 
