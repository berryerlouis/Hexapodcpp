add_library(LedPwm
    LedPwm.cpp
)

target_include_directories(LedPwm
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)