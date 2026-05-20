cmake_minimum_required(VERSION 3.22)
add_library(DriverTwi 
    Twi${TARGET}.cpp
)

target_include_directories(DriverTwi
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if (${TARGET} STREQUAL "RPI")

    target_include_directories(DriverTwi PUBLIC
        ${pigpio_SOURCE_DIR}
    )
    target_link_libraries(DriverTwi
        PUBLIC
            pigpio
    )
endif ()