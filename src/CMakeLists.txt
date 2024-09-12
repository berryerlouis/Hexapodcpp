cmake_minimum_required(VERSION 3.22.1)

message(INFO " => Compilation with ${TARGET} in ${CMAKE_BUILD_TYPE} mode for ${PROJECT_NAME}")
if (${TARGET} STREQUAL "gcc")
    # using GCC
    add_compile_definitions(GTEST)
    add_compile_definitions(X64)
    add_compile_definitions(DEBUG)
    add_compile_options(-DF_CPU=${F_CPU} -Wall -Wextra -std=c++17 -fno-exceptions -ffunction-sections -fdata-sections -fno-rtti -fno-threadsafe-statics -O0 -g)
else ()
    # using AVR GCC
    set(F_CPU 20000000UL)
    set(MCU atmega1284p)
    set(CMAKE_EXE_LINKER_FLAGS -mmcu=${MCU})
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        add_compile_definitions(DEBUG)
    endif ()
    add_compile_options(-mmcu=${MCU} -DF_CPU=${F_CPU} -Wall -Wextra -std=c++17 -fno-exceptions -ffunction-sections -fdata-sections -fno-rtti -fno-threadsafe-statics -O1 -g)
endif ()

# Concatenate all src files
set(SRC_FILES_MAIN Hexapodcpp.cpp)

set(SRC_FILES_SERVICES
        Service/Battery/ServiceBattery.cpp
        Service/Communication/ServiceCommunication.cpp
        Service/Control/ServiceControl.cpp
        Service/Display/ServiceDisplay.cpp
        Service/General/ServiceGeneral.cpp
        Service/Orientation/ServiceOrientation.cpp
        Service/Proximity/ServiceProximity.cpp
        Service/Services/Services.cpp
        Service/Event/EventListener.cpp
        Service/Event/BufferEvent.cpp
)

set(SRC_FILES_MISC
        Misc/Bitmap/Bitmaps.cpp
        Misc/Buffer/Buffer.cpp
        Misc/Logger/Logger.cpp
)

set(SRC_FILES_DRIVER
        Driver/Adc/Adc.cpp
        Driver/Gpio/Gpio.cpp
        Driver/InputCapture/InputCapture.cpp
        Driver/Tick/Tick.cpp
        Driver/Twi/Twi.cpp
        Driver/Uart/Uart.cpp
)

set(SRC_FILES_DRIVER_X64
        Driver/Adc/AdcX64.cpp
        Driver/Gpio/GpioX64.cpp
        Driver/InputCapture/InputCaptureX64.cpp
        Driver/Tick/TickX64.cpp
        Driver/Twi/TwiX64.cpp
        Driver/Uart/UartX64.cpp
)

set(SRC_FILES_COMPONENT
        Component/Battery/Battery.cpp
        Component/Communication/Communication.cpp
        Component/Display/Ssd1306.cpp
        Component/Imu/Mpu9150.cpp
        Component/Led/Led.cpp
        Component/Proximity/Laser/Vl53l0x.cpp
        Component/Proximity/SensorProximity.cpp
        Component/Proximity/Ultrasound/Srf05.cpp
        Component/Servo/Servo.cpp
        Component/Servos/Servos.cpp
        Component/ServosController/Pca9685.cpp
        Component/Software/Software.cpp
)

set(SRC_FILES_CLUSTER
        Cluster/Clusters/Clusters.cpp
        Cluster/Decoding/Protocol.cpp
        Cluster/Frame/Frame.cpp
)

set(SRC_FILES_BUILDER
        Builder/App.cpp
)

set(SRC_FILES_BOT
        Bot/Body/Body.cpp
        Bot/Leg/Leg.cpp
        Bot/Legs/Legs.cpp
)


set(SRC_FILES
        ${SRC_FILES_BUILDER}
        ${SRC_FILES_BOT}
        ${SRC_FILES_CLUSTER}
        ${SRC_FILES_COMPONENT}
        ${SRC_FILES_DRIVER}
        ${SRC_FILES_MAIN}
        ${SRC_FILES_MISC}
        ${SRC_FILES_SERVICES}
)

set(SRC_FILES_X64
        ${SRC_FILES_BUILDER}
        ${SRC_FILES_BOT}
        ${SRC_FILES_CLUSTER}
        ${SRC_FILES_COMPONENT}
        ${SRC_FILES_DRIVER_X64}
        ${SRC_FILES_MAIN}
        ${SRC_FILES_MISC}
        ${SRC_FILES_SERVICES}
)

if (${TARGET} STREQUAL "gcc")
    # using GCC
    add_executable(${PROJECT_NAME} ${SRC_FILES_X64})
else ()
    # using AVR GCC
    add_executable(${PROJECT_NAME} ${SRC_FILES})
endif ()

# Rename the output to .elf as we will create multiple files
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}.elf)

# Strip binary for upload
add_custom_target(strip ALL avr-strip ${PROJECT_NAME}.elf DEPENDS ${PROJECT_NAME})

add_custom_target(hex ALL avr-objcopy -R .eeprom -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex DEPENDS strip)

# Upload the firmware with avrdude
add_custom_target(upload avrdude.exe -c arduino -P COM4 -b 500000 -p m1284p -U flash:w:build/avr-debug/src/Hexapodcpp.elf DEPEND HEX)