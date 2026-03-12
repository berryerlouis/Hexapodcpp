add_library(Display
    Ssd1306.cpp 
)

target_include_directories(Display
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)