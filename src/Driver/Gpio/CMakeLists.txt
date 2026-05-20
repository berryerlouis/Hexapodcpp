cmake_minimum_required(VERSION 3.22)
add_library(DriverGpio 
    Gpio${TARGET}.cpp
)

target_include_directories(DriverGpio
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if (${TARGET} STREQUAL "RPI")

    target_include_directories(DriverGpio PUBLIC
        ${wiringpi_SOURCE_DIR}
    )
    target_link_libraries(DriverGpio
        PUBLIC
            wiringPi
    )
endif ()