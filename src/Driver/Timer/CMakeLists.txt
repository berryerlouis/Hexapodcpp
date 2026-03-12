add_library(DriverTimer 
    Tick${TARGET}.cpp
)

target_include_directories(DriverTimer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)