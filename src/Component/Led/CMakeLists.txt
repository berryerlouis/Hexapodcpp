add_library(Led
    Led.cpp
)

target_include_directories(Led
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)