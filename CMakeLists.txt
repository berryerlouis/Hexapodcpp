cmake_minimum_required(VERSION 3.22.1)

# Project name
project(Hexapodcpp
        VERSION 1.0
        DESCRIPTION "Hexapodcpp")

if (${GOOGLE_TEST} EQUAL 1)
    add_subdirectory(unittests)
else()
    set(PRODUCT_NAME Hexapodcpp)
    if (${TARGET} STREQUAL "gcc")
        # using GCC
        add_compile_options( -DGTEST -DF_CPU=${F_CPU} -Wall -Wextra -std=c++17 -fno-exceptions -ffunction-sections -fdata-sections -fno-rtti -fno-threadsafe-statics -O1 -g)
    else()
        # using AVR GCC
        set(F_CPU 20000000UL)
        set(MCU atmega1284p)
        set(CMAKE_CXX_COMPILER avr-g++)
        set(CMAKE_C_COMPILER avr-gcc)
        set(CMAKE_ASM_COMPILER avr-gcc)
        set(CMAKE_EXE_LINKER_FLAGS -mmcu=${MCU})
        add_compile_options(-mmcu=${MCU} -DF_CPU=${F_CPU} -Wall -Wextra -std=c++17 -fno-exceptions -ffunction-sections -fdata-sections -fno-rtti -fno-threadsafe-statics -O1 -g)
    endif()
    # Get all src files
    file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.h)

    # Create one target
    add_executable(${PRODUCT_NAME} ${SRC_FILES})

    # Rename the output to .elf as we will create multiple files
    set_target_properties(${PRODUCT_NAME} PROPERTIES OUTPUT_NAME ${PRODUCT_NAME}.elf)

    # Strip binary for upload
    add_custom_target(strip ALL avr-strip ${PRODUCT_NAME}.elf DEPENDS ${PRODUCT_NAME})

    add_custom_target(hex ALL avr-objcopy -R .eeprom -O ihex ${PRODUCT_NAME}.elf ${PRODUCT_NAME}.hex DEPENDS strip)

    add_custom_target(upload avrdude -P COM4 -c arduino -b 500000 -p ${MCU} -U flash:w:${PRODUCT_NAME}.hex DEPENDS hex)
endif()