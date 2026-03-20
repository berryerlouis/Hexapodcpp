cmake_minimum_required(VERSION 3.22)
add_subdirectory(Bitmap)
add_subdirectory(Buffer)

add_library(Misc INTERFACE)

target_include_directories(Misc
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Misc
    INTERFACE
        Bitmaps
        Buffer
)
