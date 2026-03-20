cmake_minimum_required(VERSION 3.22)
add_library(DriverInputCapture 
    InputCapture${TARGET}.cpp
)

target_include_directories(DriverInputCapture
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if (${TARGET} STREQUAL "RPI")

    target_include_directories(DriverInputCapture PUBLIC
        ${wiringpi_SOURCE_DIR}
    )
    target_link_libraries(DriverInputCapture
        PUBLIC
            wiringPi
    )
endif ()