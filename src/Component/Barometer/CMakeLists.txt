add_library(Barometer 
    Barometer.cpp
)

target_include_directories(Barometer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)