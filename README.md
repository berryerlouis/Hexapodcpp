[![Make](https://github.com/berryerlouis/Hexapodcpp/actions/workflows/build.yaml/badge.svg)](https://github.com/berryerlouis/Hexapodcpp/actions/workflows/build.yaml)
 
 # Install 
  First install the following tools:
  - install uncrustify
  - install VS-code
  - install serial-monitor extension
  - install avr-gcc [(link windows)](https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/SoftwareTools/avr8-gnu-toolchain-3.7.0.1796-win32.any.x86_64.zip) [(link linux)](https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/SoftwareTools/avr8-gnu-toolchain-3.7.0.1796-linux.any.x86_64.tar.gz)
 
 # Uncrustify
  - parse all project files
  - run `./bin/dev/uncrustify-all`
 
 # Compile
  - run `make -j16`
 
 # Upload
  - run `make flash`

 # Serial 
 Communication settings are :
 115200 8 N 1

 
