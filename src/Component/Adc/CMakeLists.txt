add_library(Adc 
    Ads1115.cpp
)

target_include_directories(Adc
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)