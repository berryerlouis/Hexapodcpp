add_library(Sound
    Sound.cpp
)

target_include_directories(Sound
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)