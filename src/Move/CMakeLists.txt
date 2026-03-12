add_subdirectory(Gait)
add_subdirectory(Walk)

add_library(Move STATIC
    Move.cpp
)

target_include_directories(Move
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Move
    PUBLIC
        Walk
        Gait
)