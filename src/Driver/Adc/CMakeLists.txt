add_library(DriverAdc 
    Adc${TARGET}.cpp
)

target_include_directories(DriverAdc
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)