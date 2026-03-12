add_library(Battery 
    Battery.cpp
)

target_include_directories(Battery
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)