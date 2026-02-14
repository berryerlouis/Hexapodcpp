add_subdirectory(Adc)
add_subdirectory(Gpio)
add_subdirectory(InputCapture)
add_subdirectory(Socket)
add_subdirectory(Timer)
add_subdirectory(Twi)

add_library(Driver 
    ${SRC_FILES_DRIVER_ADC}
    ${SRC_FILES_DRIVER_GPIO}
    ${SRC_FILES_DRIVER_INPUTCAPTURE}
    ${SRC_FILES_DRIVER_SOCKET}
    ${SRC_FILES_DRIVER_TIMER}
    ${SRC_FILES_DRIVER_TWI}
)

target_include_directories(Driver
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Adc
        ${CMAKE_CURRENT_SOURCE_DIR}/Gpio
        ${CMAKE_CURRENT_SOURCE_DIR}/InputCapture
        ${CMAKE_CURRENT_SOURCE_DIR}/Socket
        ${CMAKE_CURRENT_SOURCE_DIR}/Timer
        ${CMAKE_CURRENT_SOURCE_DIR}/Twi
)

target_link_libraries(Driver
    PUBLIC
        tiny_websockets_lib
)

if (${TARGET} STREQUAL "RPI")
    target_include_directories(Driver PUBLIC ${wiringpi_SOURCE_DIR})
    target_link_libraries(Driver
        PUBLIC
            wiringPi
    )
endif ()