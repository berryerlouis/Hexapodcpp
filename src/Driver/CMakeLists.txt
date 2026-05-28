cmake_minimum_required(VERSION 3.22)
add_subdirectory(Adc)
add_subdirectory(Gpio)
add_subdirectory(InputCapture)
add_subdirectory(Joystick)
add_subdirectory(Socket)
add_subdirectory(Timer)
add_subdirectory(Twi)

add_library(Driver INTERFACE)

target_include_directories(Driver
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Driver
    INTERFACE
        DriverAdc
        DriverGpio
        DriverInputCapture
        DriverJoystick
        DriverSocket
        DriverTimer
        DriverTwi
)