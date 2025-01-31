cmake_minimum_required(VERSION 3.22)

message(INFO " => Compilation with ${TARGET} in ${CMAKE_BUILD_TYPE} mode for ${PROJECT_NAME}")
if (${TARGET} STREQUAL "X64")
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED True)

    include(FetchContent)

    FetchContent_Declare(
            TinyWebsockets
            GIT_REPOSITORY https://github.com/berryerlouis/TinyWebsockets.git
            GIT_TAG master
    )

    FetchContent_MakeAvailable(TinyWebsockets)

    # using GCC
    add_compile_definitions(GTEST)
    add_compile_definitions(X64)
    add_compile_options(-DF_CPU=${F_CPU} -Wall -Wextra -std=c++17 -fno-exceptions -ffunction-sections -fdata-sections -fno-rtti -fno-threadsafe-statics -O0 -g)
elseif (${TARGET} STREQUAL "RPI")

    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED True)
    #WiringPi
    include(FetchContent)
    FetchContent_Declare(
            wiringPi
            GIT_REPOSITORY https://github.com/WiringPi/WiringPi.git
            GIT_TAG master
    )
    FetchContent_MakeAvailable(wiringPi)
    include_directories(${wiringpi_SOURCE_DIR})

    #TinyWebsockets
    FetchContent_Declare(
            TinyWebsockets
            GIT_REPOSITORY https://github.com/berryerlouis/TinyWebsockets.git
            GIT_TAG master
    )
    FetchContent_MakeAvailable(TinyWebsockets)

    add_compile_definitions(X64)
    add_compile_definitions(RPI)

else ()
    # using AVR GCC
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_CXX_COMPILER avr-g++)
    set(CMAKE_C_COMPILER avr-gcc)
    set(CMAKE_ASM_COMPILER avr-gcc)
    set(F_CPU 20000000UL)
    set(MCU atmega1284p)
    set(CMAKE_EXE_LINKER_FLAGS -mmcu=${MCU})

    add_compile_definitions(AVR)

    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        add_compile_definitions(DEBUG)
    endif ()
    add_compile_options(-mmcu=${MCU} -DF_CPU=${F_CPU} -Wall -Wextra -std=c++17 -fno-exceptions -ffunction-sections -fdata-sections -fno-rtti -fno-threadsafe-statics -O1 -g)
endif ()

# Concatenate all src files
set(SRC_FILES_MAIN Hexapodcpp.cpp)

set(SRC_FILES_SERVICES
        Service/Battery/ServiceBattery.cpp
        Service/Button/ServiceButton.cpp
        Service/Communication/ServiceCommunication.cpp
        Service/Control/ServiceControl.cpp
        Service/Display/ServiceDisplay.cpp
        Service/General/ServiceGeneral.cpp
        Service/Orientation/ServiceOrientation.cpp
        Service/Proximity/ServiceProximity.cpp
        Service/Sound/ServiceSound.cpp
        Service/Services/Services.cpp
        Service/Event/EventListener.cpp
        Service/Body/ServiceBody.cpp
)

set(SRC_FILES_MISC
        Misc/Bitmap/Bitmaps.cpp
        Misc/Buffer/Buffer.cpp
        Misc/Logger/Logger.cpp
)

set(SRC_FILES_DRIVER
        Driver/Adc/Adc${TARGET}.cpp
        Driver/Gpio/Gpio${TARGET}.cpp
        Driver/InputCapture/InputCapture${TARGET}.cpp
        Driver/Socket/Socket${TARGET}.cpp
        Driver/Tick/Tick${TARGET}.cpp
        Driver/Twi/Twi${TARGET}.cpp
        Driver/Uart/Uart${TARGET}.cpp
)

set(SRC_FILES_COMPONENT
        Component/Barometer/Barometer.cpp
        Component/Battery/Battery.cpp
        Component/Sound/Sound.cpp
        Component/Button/Button.cpp
        Component/Communication/Communication.cpp
        Component/Display/Ssd1306.cpp
        Component/Imu/Ahrs.cpp
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
        Cluster/Battery/ClusterBattery.cpp
        Cluster/Button/ClusterButton.cpp
        Cluster/Body/ClusterBody.cpp
        Cluster/General/ClusterGeneral.cpp
        Cluster/Imu/ClusterImu.cpp
        Cluster/Proximity/ClusterProximity.cpp
        Cluster/Servo/ClusterServo.cpp
        Cluster/ClusterBase.cpp
        Cluster/ClusterCommand.cpp
        Cluster/Sound/ClusterSound.cpp
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
        Bot/Walk/Walk.cpp
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

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} tiny_websockets_lib)
if (${TARGET} STREQUAL "RPI")
    target_link_libraries(${PROJECT_NAME} wiringPi)
else ()
    # Rename the output to .elf as we will create multiple files
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}.elf)

    # Strip binary for upload
    add_custom_target(strip ALL avr-strip ${PROJECT_NAME}.elf DEPENDS ${PROJECT_NAME})

    add_custom_target(hex ALL avr-objcopy -R .eeprom -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex DEPENDS strip)

    # Upload the firmware with avrdude
    add_custom_target(upload avrdude.exe -c arduino -P COM4 -b 500000 -p m1284p -U flash:w:build/avr-debug/src/Hexapodcpp.elf DEPEND hex)
endif ()
