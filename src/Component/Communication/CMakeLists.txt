add_subdirectory(Decoding)
add_subdirectory(Frame)

set(SRC_FILES_COMMMUNICATION
    ${CMAKE_CURRENT_SOURCE_DIR}/Communication.cpp
)

add_library(Communication 
    ${SRC_FILES_COMMMUNICATION}
    ${SRC_FILES_DECODING}
    ${SRC_FILES_FRAME}
)

target_include_directories(Communication
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR} 
        ${CMAKE_CURRENT_SOURCE_DIR}/Decoding
        ${CMAKE_CURRENT_SOURCE_DIR}/Frame
)

target_link_libraries(Communication
    PUBLIC
        Cluster
)