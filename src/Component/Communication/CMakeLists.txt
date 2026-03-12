add_subdirectory(Decoding)
add_subdirectory(Frame)

add_library(Communication
    Communication.cpp
)

target_include_directories(Communication
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Communication
    PUBLIC
        Decoding
        Frame
)