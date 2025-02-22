cmake_minimum_required(VERSION 3.22)

if (${CROSS})
    message(INFO " => Compilation for ${TARGET} in ${CMAKE_BUILD_TYPE} mode for ${PROJECT_NAME} using Cross Compiler")
else ()
    message(INFO " => Compilation for ${TARGET} in ${CMAKE_BUILD_TYPE} mode for ${PROJECT_NAME} using native Compiler")
endif ()


if (${TARGET} STREQUAL "X64")
    include(FetchContent)
    FetchContent_Declare(
            TinyWebsockets
            GIT_REPOSITORY https://github.com/berryerlouis/TinyWebsockets.git
            GIT_TAG master
    )
    FetchContent_MakeAvailable(TinyWebsockets)

    # using GCC
    add_compile_definitions(X64)

elseif (${TARGET} STREQUAL "RPI")
    # WiringPi
    include(FetchContent)
    FetchContent_Declare(
            wiringPi
            GIT_REPOSITORY https://github.com/WiringPi/WiringPi.git
            GIT_TAG master
    )
    FetchContent_MakeAvailable(wiringPi)
    include_directories(${wiringpi_SOURCE_DIR})

    # TinyWebsockets
    FetchContent_Declare(
            TinyWebsockets
            GIT_REPOSITORY https://github.com/berryerlouis/TinyWebsockets.git
            GIT_TAG master
    )
    FetchContent_MakeAvailable(TinyWebsockets)
    add_compile_definitions(RPI)

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
        Component/Adc/Ads1115.cpp
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

#include_directories(/opt/poky/5.1/sysroots/arm1176jzfshf-vfp-poky-linux-gnueabi/usr/include/c++/14.2.0)
add_compile_options(-Wall -Wextra -Werror -Wno-cast-align -fno-exceptions -ffunction-sections -fdata-sections -fno-rtti -fno-threadsafe-statics -O -g)
add_executable(${PROJECT_NAME} ${SRC_FILES})
if (${TARGET} STREQUAL "RPI")
    target_link_libraries(${PROJECT_NAME} wiringPi tiny_websockets_lib)
else ()
    target_link_libraries(${PROJECT_NAME} tiny_websockets_lib)
endif ()