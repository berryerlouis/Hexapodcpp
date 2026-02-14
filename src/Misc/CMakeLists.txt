add_subdirectory(Bitmap)
add_subdirectory(Buffer)


add_library(Misc 
    ${SRC_FILES_BITMAP}
    ${SRC_FILES_BUFFER}
)

target_include_directories(Misc
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Bitmap
        ${CMAKE_CURRENT_SOURCE_DIR}/Buffer
)