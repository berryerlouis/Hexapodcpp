add_subdirectory(Gait)
add_subdirectory(Walk)

add_library(Move 
    ${SRC_FILES_GAIT}
    ${SRC_FILES_WALK}
)

target_include_directories(Move
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Gait
        ${CMAKE_CURRENT_SOURCE_DIR}/Walk
)